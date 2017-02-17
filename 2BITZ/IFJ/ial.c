/*********************************************
 *   Program :   ial.c
 *   Authors :   Roman Dobiáš     - xdobia11
 *               Adrián Tomašov   - xtomas32
 *               Jozef Urbanovský - xurban66
 *               Adam Šulc        - xsulca00
 *               Kristián Barna   - xbarna02
 *   Skupina :   2BIB(2016)
 *   Created :   01.10.2016
 *   Compiled:   gcc 4.9.2
 *   Project :   IFJ16
 *
 *   Notes   :   Implementation of IAL algorithms
 ********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ial.h"


/*
 * Algoritmy
 */

interval separate(char *arr, int left, int right){

    char pom;
    char pmed = arr[(left + right) / 2];
    interval limit;
    limit.i = left;
    limit.j = right;

    while (limit.i < limit.j) {
        while (arr[limit.i] < pmed)
            limit.i++;
        while (arr[limit.j] > pmed)
            limit.j--;

        if (limit.i <= limit.j) {
            pom = arr[limit.i];
            arr[limit.i] = arr[limit.j];
            arr[limit.j] = pom;
            limit.i++;
            limit.j--;
        }
    }

    return limit;
}

void ial_qsort_valid(char *arr, int left, int right) {

    interval pom = separate(arr, left, right);

    if (left < pom.j)
        ial_qsort(arr, left, pom.j);
    if (pom.i < right)
        ial_qsort(arr, pom.i, right);

    return;
}

void ial_qsort(char *arr, int left, int right) {
    if (left < right && arr != NULL)
        ial_qsort_valid(arr, left, right);
    return;
}

void KMP_spracovanie_podretazca(char *podretazec, int *Table){

    //index zaciatku opetovneho porovnavania pri nezhode podretazca s retazcom
    int ret_index = 0;

    Table[0] = -1;

    for (int i = 1; i < strlen(podretazec); i++){
        ret_index = Table[i-1];
        while (ret_index > -1 && podretazec[i-1] != podretazec[ret_index])
            ret_index = Table[ret_index];
        Table[i] = ret_index + 1;
    }


}

int KMP_hladaj_valid(char *retazec, char *podretazec){

    int *Table;

    if ((Table = malloc(sizeof(int) * strlen(podretazec))) == NULL)
        return -1;

    KMP_spracovanie_podretazca(podretazec, Table);

    int Retazec_index = 0;
    int Podretazec_index = 0;
    while (Podretazec_index < strlen(podretazec) && Retazec_index < strlen(retazec)){
        if (retazec[Retazec_index] == podretazec[Podretazec_index]) {
            Retazec_index++;
            Podretazec_index++;
        }
        else
        if (Podretazec_index == 0)
            Retazec_index++;
        else
            Podretazec_index = Table[Podretazec_index];
    }
    free(Table);

    return (Podretazec_index >= strlen(podretazec)) ? ((int)(Retazec_index - strlen(podretazec))) : (Retazec_index);

}

int KMP_hladaj(char *retazec, char *podretazec){
    if (!retazec)
        return -1;
    if (!podretazec)
        return (int)strlen(retazec);
    return KMP_hladaj_valid(retazec, podretazec);;
}

/*
 * Tabulka symbolov
 */

unsigned hash_fun_ptr(char *id, unsigned stab_size) {
    int i = 0, pom = 0;

    while (id[i] != '\0') {
        pom += (int) id[i];
        i++;
    }
    return pom % stab_size;
}

//alokuje priestor pre tabulku a zaroven ho nuluje
stab_t *stable_init(unsigned size) {
    stab_t *pom;
    if ((pom = calloc(1, sizeof(stab_t) + sizeof(stab_element_t) * size)) ==
        NULL) //TODO : kiko pozri ci tu nema byt *stab_element_t
        return NULL;
    pom->stab_size = size;
    pom->hash_fun_ptr = hash_fun_ptr;
    pom->stab_count = 0;

    return pom;
}

/*
//rekurzivne prejde jednosmerne viazany zoznam elementov a uvolni pamat
void stable_destroy_element(stab_element_t *p_element){

    if (p_element->stab_next->stab_next != NULL)
        stable_destroy_element(p_element->stab_next);

    free(p_element->stab_next);
    p_element->stab_next = NULL;
    return;
}
*/

//zavola funkciu na destrukciu prvkov v tabulke a potm uvolni tabulku
void stable_destroy(stab_t **p_table) {
    stab_element_t *current, *next;

    if (*p_table != NULL) {
        for (unsigned i = 0; i < p_table[0]->stab_size; i++) {
            current = p_table[0]->arr[i];
            while (current != NULL) {
                next = current->stab_next;
                free(current->stab_key);
                if (current->stab_content.data.arg_type == STRING) {
                    str_destroy(&current->stab_content.data.data.s);
                }
                free(current);
                current = next;
            }
        }
        free(*p_table);
        *p_table = NULL;
    }
    return;
}

//pridava polozku do zoznamu
data_t *stable_add_var(stab_t *p_stable, char *id, data_t p_var) {

    static int rank = 1;
    p_var.rank = rank++;
    
    unsigned index = hash_fun_ptr(id, p_stable->stab_size);
    stab_element_t **pom = &p_stable->arr[index];

    while ((*pom) != NULL) {
        if (!strcmp((*pom)->stab_key, id))
            return NULL;
        pom = &((*pom)->stab_next);
    }

    if (((*pom) = malloc(sizeof(stab_element_t))) == NULL) {
        return NULL;
    }
    (*pom)->stab_content = p_var;

    char *new_key = NULL;
    if ((new_key = malloc(sizeof(char) * strlen(id) + 1)) == NULL)
        return NULL;

    strcpy(new_key, id);
    (*pom)->stab_key = new_key;
    (*pom)->stab_next = NULL;

    return &(*pom)->stab_content;
}

//vrati polozku zo zoznamu
data_t *stable_get_var(stab_t *p_stable, char *id) {
    if (p_stable != NULL) {
        unsigned index = hash_fun_ptr(id, p_stable->stab_size);
        stab_element_t *pom = p_stable->arr[index];

        while (pom != NULL) {
            if (!strcmp(pom->stab_key, id)) {
                return &pom->stab_content;
            }
            pom = pom->stab_next;
        }
    }
    return NULL;
}

//odstrni polozkiu zo zoznamu
int stable_remove_var(stab_t *p_stable, char *id) {
    unsigned index = hash_fun_ptr(id, p_stable->stab_size);
    stab_element_t *pom = p_stable->arr[index];
    stab_element_t *vymaz, *previous;

    if (pom) {
        //ak je to 1. polozka
        if (!strcmp(pom->stab_key, id)) {
            p_stable->arr[index] = pom->stab_next;
            vymaz = pom;
            free(vymaz->stab_key);
            free(vymaz);
            return true;
        } else {
            //ak je to 2. az n polozka
            //najde polozku pred polozkou s id
            previous = pom;
            pom = pom->stab_next;
            while (pom != NULL) {
                if (!strcmp(pom->stab_key, id)) {
                    previous->stab_next = pom->stab_next;
                    free(pom->stab_key);
                    free(pom);
                    return true;
                }
                pom = pom->stab_next;
            }
        }
    }
    return false;
}

int stable_search(stab_t *p_stable, char *srch_el) {
    if (stable_get_var(p_stable, srch_el) == NULL) {
        return false;
    } else {
        return true;
    }
}

int stable_add_concatenate(stab_t *p_stable, char *clss, char *fnct, char *local, data_t data) {
    char *pom;
    int j = 0, i = 0;
    int size = 0;

    if (clss != NULL) {
        size += strlen(clss);
        if (fnct != NULL) {
            size += strlen(fnct);
            if (local != NULL)
                size += strlen(local);
        }
    }

    if ((pom = malloc(size * sizeof(char) + 2 + 1)) == NULL) //+2 lebo sa vkadaju max 2x '.' +1 lebo '\0'
        return false;

    if (clss != NULL)
        while (clss[i] != '\0')
            pom[j++] = clss[i++];

    i = 0;
    if (fnct != NULL) {
        pom[j++] = '.';
        while (fnct[i] != '\0')
            pom[j++] = fnct[i++];
    }

    i = 0;
    if (local != NULL) {
        pom[j++] = '.';
        while (local[i] != '\0')
            pom[j++] = local[i++];
    }

    pom[j] = '\0';

    if (stable_add_var(p_stable, pom, data) == NULL) {
        free(pom);
        return false;
    }
    free(pom);
    return true;

}

void stable_print(stab_t *stable) {
    stab_element_t *current;
    printf("************** STABLE ****************\n");
    for (int i = 0; i < stable->stab_size; i++) {
        current = stable->arr[i];
        while (current != NULL) {
            printf("KEY : %-30s CONTENT : ", current->stab_key);
            switch (current->stab_content.data.arg_type) {
                case INTEGER:
                    printf("%20d  || Integer ||", current->stab_content.data.data.i);
                    break;
                case DOUBLE:
                    printf("%20g  || Double  ||", current->stab_content.data.data.d);
                    break;
                case STRING:
                    printf("%20s  || String  ||", current->stab_content.data.data.s.str);
                    break;
                case STACK_EBP:
                    printf("At stack >> %8d", current->stab_content.data.data.i);
                    break;
                case INSTRUCTION:
                    printf("FUNCTION: %10p", current->stab_content.data.data.instruction);
                    break;
                default:
                    printf("on top of stack     ");
                    break;
            }

            printf("    || %p \n", (void *) &current->stab_content.data);
            current = current->stab_next;
        }
    }
    printf("********** STABLE END ****************\n");
}

// Searching a key of format 1.2.3......count
data_t *stable_search_variadic(stab_t *p_stable, int count, ...) {
    if (count == 0)
        return NULL;
    data_t *result = NULL;
    string_t str = str_init();

    va_list args;
    va_start(args, count);
    char *ptr = va_arg(args, char*);
    if (ptr != NULL) {
        // for (count-1): append strings at the end of str
        str_append_chars(&str, ptr);
        while (--count) {
            ptr = va_arg(args, char*);
            if (ptr == NULL)
                break;
            str_add_char(&str, '.');
            str_append_chars(&str, ptr);
        }
        // now search
        result = stable_get_var(p_stable, str.str);
    }
    va_end(args);
    str_destroy(&str);
    return result;
}

data_t *stable_add_variadic(stab_t *p_stable, data_t data, int count, ...) {
    if (count == 0)
        return NULL;
    data_t *result = NULL;
    string_t str = str_init();

    va_list args;
    va_start(args, count);
    char *ptr = va_arg(args, char*);
    if (ptr != NULL) {
        // for (count-1): append strings at the end of str
        str_append_chars(&str, ptr);
        while (--count) {
            ptr = va_arg(args, char*);
            if (ptr == NULL)
                break;
            str_add_char(&str, '.');
            str_append_chars(&str, ptr);
        }
        // now add
        result = stable_add_var(p_stable, str.str, data);
    }
    va_end(args);
    str_destroy(&str);
    return result;
}
