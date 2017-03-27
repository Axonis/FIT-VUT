/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

#define	E_FILE -10 // chyba pri otvarani suboru
#define	E_COUNT -20 // chyba pri nacitani countu alebo nevhodny vyraz
#define	E_MALLOC -30 // chyba pri alokacii pamate
#define	E_FORMAT -40 // chyba najdena v pozadovanom formate

#define MAX_INPUT 255

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct objt_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t
{
    int id;
    float x;
    float y;
};

struct cluster_t
{
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

	c->obj = NULL; // inicializuje na NULL
	c->size = 0; // priradi kapacitu 0
	c->capacity = cap; // nacita cap z hora

	if (cap > 0)
		c->obj = malloc(sizeof(struct obj_t) * cap); // alokuje velkost objektu (id+x+y) * kolkokrat bolo zadane aby alokovalo.

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
	if (c->capacity) // ak ma nejaku velkost
		free(c->obj); // tak ho uvolni
	init_cluster(c, 0); // a inicializuje uplne nanovo

}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap) // nemoze zmenit velkost ak zadana velkost je vacsia alebo rovna ako momentalna
        return c;

    size_t size = sizeof(struct obj_t) * new_cap; // size_t sa pouziva pre velkost objektov podla ISO C-99, spocita nanovo jeho velkost potrebnu na alokaciu

    void *arr = realloc(c->obj, size); // realloc vracia ukazatel na void

    if (arr == NULL) // ak nenajde miesto vrati NULL
        return NULL;

    c->obj = arr; // priradi nove veci do c
    c->capacity = new_cap; // priradi sa do capacity zvacsena hodnota

    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{

	if (c->size >= c->capacity) // ak je velkost vacsia alebo rovna ako kapacita
            resize_cluster(c, (c->capacity > 0) ? 2*c->capacity : CLUSTER_CHUNK); // zvacsi cluster o dvojnasobok ak existuje

	if (c->size < c->capacity) // ak je velkost dostacujuca
	{
		c->obj[c->size] = obj; // tak sa doplni kolko potrebuje
		c->size++; // inkrementuje sa jeho velkost za kazdy append
	}

}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    if (c1 != NULL && c2 != NULL)
    {
        int size = c2->size;

        while (--size >= 0)
            append_cluster(c1, c2->obj[size]); // vklada na koniec jednotky az kym nepride na koniec dvojky
        sort_cluster(c1); // nakonci ich usporiada
    }
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);


    if (carr == NULL) //ak nechce nic ostranit tak skonci
        return narr;

    struct cluster_t tmp;

    tmp = carr[idx]; // ulozim si ho do pomocnej premmenej

    for (int i = idx; i < narr-1; i++) // posuva az na koniec -1
        carr[i] = carr[i+1];

    carr[narr-1] = tmp; // priradi na posledne miesto to co chceme odstranit

    clear_cluster(&carr[narr-1]); // vymaze presne ten index

    return narr-1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    return sqrtf(powf(o1->x - o2->x, 2) + powf(o1->y - o2->y, 2)); // euklidovska vzdialenost
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    struct obj_t *o1 = c1->obj; // nacita si objekt 1 do premennej
    struct obj_t *o2 = c2->obj; // nacita si objekt 2 do premennej

    float min_d = obj_distance(&o1[0], &o2[0]); // stanovi prvu minimalnu vzdialenost medzi prvymi objektami
    float new_d;

    for (int i = 0; i < c1->size; ++i) // matica na indexy
        for (int j = 0; j < c2->size; ++j)
        {
            new_d = obj_distance(&o1[i], &o2[j]); // pyta sa ci tato vzdialenost je mensia ako predtym stanovena, az kym prejde vsetky objekty
            if (min_d > new_d) // ak najde taku
                min_d = new_d; // tak je to nove minimum
        }

    return min_d;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float min_d = cluster_distance(&carr[0], &carr[1]); // stanovi si ako minimum prvu moznu vzdialenost medzi zhlukmi
    float tmp_d = 0.0;
    int tmp = 0;

    *c1 = 0;
    *c2 = 1;

    for (int i = 0; i < narr; ++i) // matica na indexy
        for (int j = i+1; j < narr; ++j)
        {
            tmp_d = cluster_distance(&carr[i], &carr[j]); // ulozi si hodnotu medzi I a J clusterom do premennej
            if (min_d > tmp_d) // a pyta sa ci nahodou dana vzdialenost nieje mensia ako minimum
            {
                min_d = tmp_d; // ak je minimum zhodne s naratanou hodnotou znamena to ze su susedia
                *c1 = i; // priradi tieto indexy na ukazatel na int
                *c2 = j;
            }
        }
    if (*c1 > *c2) // pokial je prvy vacsi ako druhy tak ich vymeni cez pomocnu premennu
    {
        tmp = *c1;
        *c1 = *c2;
        *c2 = tmp;
    }
}


// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/

int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    FILE *file = fopen(filename, "r");
    int count = 0;
    char tmp[MAX_INPUT]; // pomocna premenna na nacitanie stringu
    char id[MAX_INPUT]; // pomocna premenna na nacitanie ID do stringu a nasledne overovanie
    char x[MAX_INPUT]; // pomocna premenna na nacitanie X do stringu a nasledne overovanie
    char y[MAX_INPUT]; // pomocna premenna na nacitanie Y do stringu a nasledne overovanie
    char *ptr = NULL;
    struct obj_t obj;
    struct cluster_t *clus; // nech sa nemusim stale odkazovat na arr[0][i]

    if (file == NULL)
    {
        *arr = NULL;
        return E_FILE; // CANNOT OPEN FILE
    }

    fgets(tmp, MAX_INPUT, file);
    if (sscanf(tmp, "count=%d", &count) == EOF)
    {
        *arr = NULL;
        fclose(file);
        return E_COUNT; // COUNT IS IN WRONG FORMAT
    }

    clus = *arr = malloc(sizeof(struct cluster_t) * count);

    if (*arr == NULL && clus == NULL)
    {
        fclose(file);
        return E_MALLOC; // FAILED TO ALLOC
    }

    fscanf(file, "count=%d\n", &count);


    for (int i = 0; i < count ; i++)
    {
        fgets(tmp, 100, file); // nacita si cely riadok do pomocnej premennej ako string
        if (sscanf(tmp, "%s %s %s", id,x,y) != 3) // rozdeli si ho na mensie stringy reprezentujuce dane vyrazy
        {
            *arr = NULL; // pri akejkolvek chybe ma byt *arr = NULL
            fclose(file);
            return E_FORMAT; // WRONG FILE FORMAT
        }
        errno = 0; // po kazdou pracou s errno ho musim resetnut na 0
        obj.id = strtol(id, &ptr, 10); // prevedie a priradi hodnotu uz do objektu
        if (errno == ERANGE || strcmp("",ptr)) // pokial pretecie dany datovy typ alebo prevedenie je s ostatkom ukonci sa
        {
            *arr = NULL;
            fclose(file);
            return E_FORMAT; // WRONG FILE FORMAT
        }

        errno = 0;
        obj.x = strtof(x,&ptr);
        if(obj.x > 1000.0 || obj.x < 0.0 || errno == ERANGE || strcmp("",ptr))
        {
            *arr = NULL;
            fclose(file);
            return E_FORMAT; // WRONG FILE FORMAT
        }

        errno = 0;
        obj.y = strtof(y,&ptr);
        if(obj.y > 1000.0 || obj.y < 0.0 || errno == ERANGE || strcmp("",ptr))
        {
            *arr = NULL;
            fclose(file);
            return E_FORMAT; // WRONG FILE FORMAT
        }

        init_cluster(&clus[i], CLUSTER_CHUNK); // inicializujem dany cluster

        if (clus[i].obj == NULL) // pokial sa mi nieco pokazilo tak uvolnim alokovanu pamat
        {
            while (--i >= 0)
                clear_cluster(&clus[i]);
            free(clus); // potrebujem samotny cluster aby som ho mohol uvolnovat v pripade chyby
            fclose(file);
            return E_MALLOC;
        }

        append_cluster(&clus[i], obj); // mozem vyuzit zapisovanie na koniec novovytvoreneho clusteru
    }
    fclose(file);

    return count;
}




/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}



int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    int count = 0;
    int arg_clusters = 1;
    char *ptr = NULL;

    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "This program works as simple cluster analysis.\n"
                "Instructions : \n\n"
                "./proj3 name_of_file.txt number_of_clusters_desired\n");
        return EXIT_FAILURE;
    }

    errno = 0;
    count = load_clusters(argv[1], &clusters);

    if (argc == 3)
    {
        arg_clusters = strtol(argv[2], &ptr, 10);
        if (arg_clusters > count || arg_clusters <= 0 || errno == ERANGE || strcmp(ptr,"")) // ak je zadany pocet clusterov vacsi ako ten v subore ukonci program
        {
            fprintf(stderr,"[Error] : Number of clusters desired is higher than in input file or not in a required format!\n");
            while (--count >= 0) // pokial sa vsetko zdari alokovat a count je stale nenulovy tak dealokujem
					clear_cluster(&clusters[count]); // alokujem ich na NULL a 0
				if (clusters != NULL) // dealokujem samotne clustery
					free(clusters);

            return EXIT_FAILURE;
        }
    }

    if (count < 0) // ak nahodou funkcia load vrati zaporne cislo vypise sa prisluchajuca chyba
    {
        switch (count)
        {
		case E_FILE :
            fprintf(stderr, "[Error] : Failed to open file!\n");
            break;
        case E_COUNT :
            fprintf(stderr, "[Error] : Count in input file is in wrong format!\n");
            break;
        case E_MALLOC :
            fprintf(stderr, "[Error] : Memory allocation for array of objects failed!\n");
            break;
        case E_FORMAT :
            fprintf(stderr, "[Error] : Data in input file are in wrong format!\n");
            break;
        }
        return EXIT_FAILURE;
    }


    int c1,c2; // naplnam to z find neighbours cez ukazatel

    while (count > arg_clusters) // pokial je pocet pozadovanych clusterov mensi ako v subore
    {
        find_neighbours(clusters, count, &c1, &c2); // najdem najblizsich susedov moznych
        merge_clusters(&clusters[c1], &clusters[c2]); // ktorych nasledne spojim do prveho clusteru s indexom c1
        remove_cluster(clusters, count, c2); // a odstranim to co ostane po poli s indexom c2, ktore uz je v c1
        count--;
    }

    print_clusters(clusters, arg_clusters);

    while (--count >= 0) // pokial sa vsetko zdari a count je stale nenulovy tak dealokujem
        clear_cluster(&clusters[count]); // alokujem ich na NULL a 0
    if (clusters != NULL) // dealokujem samotne clustery
        free(clusters);

    return EXIT_SUCCESS;
}
