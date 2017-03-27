
#include <stdlib.h>
#include <stdio.h>
#include "ial.h"

int main(){

    data_t cont1, cont2, cont3;
    data_t *pom = NULL;
    cont1.type = 1;
    cont1.inter_table = 1;
    cont2.type = 2;
    cont2.inter_table = 2;
    cont3.type = 3;
    cont3.inter_table = 3;


    char string1[] = "string1";
    char string2[] = "string2";
    char string3[] = "string3";
    char string_concat1[] = "string1.string2";
    char string_concat2[] = "string1.string2.";
    char string_blank[] = "";


    printf("\n################### TEST_INIT ####################\n");
    stab_t *sym_table = stable_init(2);
    if(sym_table == NULL)
        printf("\nstable_init FAILED (calloc)!\n");
     else
        printf("\nstable_init OK !\n");




    printf("\n################### TEST_ADD_VAR ##################\n");
    printf("\nAdding: %s", string1);
    if(stable_add_var(sym_table, string1, cont1) == NULL)
        printf("\nstable_add_var FAILED (malloc)!\n");
    else {
        printf("\nAdding: %s\n", string2);
        stable_add_var(sym_table, string2, cont2);
        printf("Adding: %s\n", string3);
        stable_add_var(sym_table, string3, cont3);
        printf("\nstable add_var OK !\n");
    }



    printf("\n################### TEST_GET_VAR ##################\n");
    pom = stable_get_var(sym_table, string1);
    if  (pom == NULL)
        printf("\nstable_get_var FAILED ! (null ptr)\n");
    else {
        printf("\nstable_get_var OK ! \n");
        printf("\nstring1:  %d\n", pom->type);
        pom = stable_get_var(sym_table, string3);
        printf("string3:  %d\n", pom->type);
    }



    printf("\n################### TEST_ADD_CONCATENATE ##################\n");
    printf("\nTo concatenate: %s + %s\n", string1, string2);
    if(!stable_add_concatenate(sym_table, string1, string2, NULL, cont2))
        printf("\nstable_add_concatenate FAILED ! (malloc)\n");
    else {
        printf("\nstable_add_concatenate OK !\n");
        pom = stable_get_var(sym_table, string_concat1);
        printf("\nstring1.string2:  %d\n", pom->type);
        stable_add_concatenate(sym_table, string1, string2, string_blank, cont3);
        pom = stable_get_var(sym_table, string_concat2);
        printf("string1.string2.:  %d\n", pom->type);
    }



    printf("\n################### TEST_SEARCH ##################\n");
    printf("\nLooking for: %s\n", string1);
    if(stable_search(sym_table, string1))
        printf("string 1 FOUND\n");
    else
        printf("string 1 NOT FOUND\n");

    printf("\nLooking for: %s\n", string2);
    if(stable_search(sym_table, string2))
        printf("string 2 FOUND\n");
    else
        printf("string 2 NOT FOUND\n");




    printf("\n################### TEST_REMOVE_VAR ##################\n");
    printf("\nRemoving: %s\n", string1);
    stable_remove_var(sym_table, "2");
    stable_remove_var(sym_table, "!");

    if(stable_remove_var(sym_table, string1) == false)
        printf("\nstable_remove_var FAILED !\n");
    else {
        stable_remove_var(sym_table, string2);
        stable_remove_var(sym_table, string2);
        stable_remove_var(sym_table, string2);
        printf("Removing: %s\n", string2);
        stable_remove_var(sym_table, string3);
        printf("Removing: %s\n", string3);
        printf("\nstable_remove_var OK !\n");
    }

    printf("\n################### TEST_SEARCH(removed data) ##################\n");
    printf("\nLooking for: %s\n", string1);
    if(stable_search(sym_table, string1))
        printf("string 1 FOUND\n");
    else
        printf("string 1 NOT FOUND\n");



    stable_destroy(&sym_table);

    printf("\n################### TEST_SEARCH(removed stable) ##################\n");
    printf("\nLooking for: %s\n", string1);
    if(stable_search(sym_table, string1))
        printf("string 1 FOUND\n");
    else
        printf("string 1 NOT FOUND\n");





    return 0;
}
