/**
 * Project No.3 Cluster analysis,
 * @Author Jozef Urbanovsky, xurban66@stud.fit.vutbr.cz,
 * Group 39 (1BIB),
 * Assistant Ing. Gabriela Necasova,
 * @Date 11/12/2015,
 * File name proj3.h,
 */

/**
 * @defgroup Structures
 * @{
 */

/**
 * A structure to represent 2D vectors.
 */
struct obj_t {
    int id; /**< ID of object */
    float x; /**< the X coordinate */
    float y; /**< the Y coordinate */
};


/**
 * A structure to represent array of object with their size and capacity.
 */
struct cluster_t {
    int size; /**< Size of cluster */
    int capacity; /**< Maximum capacity of clusters */
    struct obj_t *obj; /**< Array of objects in cluster */
};

/**
 * @}
 */

/**
 * @defgroup Functions
 * @{
 */

/**
 * @brief Initialization and memory allocation for cluster.
 *
 * Function initializes object in cluster to value NULL,
 * sets size of cluster to 0 and copies capacity from parameter.
 * If capacity is positive number, allocates memory for number of cap
 * times size of object.
 *
 * @param *c Pointer to cluster_t structure
 * @param cap Desired capacity of given cluster
 *
 * @return Void.
 */
void init_cluster(struct cluster_t *c, int cap);


/**
 * @brief Deallocation of array of object in cluster and initialization to empty cluster.
 * @pre Cluster given to clear has to exist.
 *
 * Function checks if given cluster has any capacity, if so, it deallocates each object
 * and calls function init_cluster to initialize an empty cluster.
 *
 * @param *c Pointer to of cluster_t structure
 *
 * @return Void.
 */
void clear_cluster(struct cluster_t *c);


/**
 * Chunk of cluster objects. Value recommended for reallocation.
 */
extern const int CLUSTER_CHUNK;


/**
 * @brief Modification of capacity of cluster 'c' to a 'new_cap'.
 * @pre new_cap value has to be higher than current capacity of cluster
 *
 * Function calculates size of memory needed to allocate cluster with new_cap
 * and reallocates given cluster with new value. On success realloc returns pointer
 * to the beginning of newly allocated memory, which has to be assigned into object.
 * If realloc fails, it returns NULL, but array stays allocated in memory and
 * has to be deallocated with free(), or there is a possibility of a memory leak.
 * new_cap is assigned as an update of an old cap before reallocation.
 *
 *
 * @param *c Pointer to cluster_t structure
 * @param new_cap New value of capacity to be set
 *
 * @return c resized cluster with new_cap as capacity
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);


/**
 * @brief  Adds objects at the end of cluster 'c'.
 * @pre If size exceeds capacity, cluster has to be enlarged by function resize_cluster.
 *
 * Function checks if capacity of cluster is sufficient to append object.
 * If so, object is added at the of end cluster and size is incremented.
 *
 * @param *c Pointer to cluster_t structure
 * @param obj Structure obj_t
 *
 * @return Void.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);


/**
 * @brief Copies content of cluster2 into cluster1.
 * @pre Cluster1 nor cluster2 is empty.
 *
 * Function calls append_cluster to copy objects of cluster2 into cluster1,
 * until every object is copied. Afterward function sort_cluster is called to
 * sort newly created combination of both clusters merged.
 *
 * @param *c1 Pointer to cluster_t structure
 * @param *c2 Pointer to cluster_t structure
 *
 * @return Void.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);


/**
 * @brief Removes cluster from array of clusters at given index.
 * @pre If 'carr' (array of clusters) is empty functions returns 'narr' (number of arrays).
 * @pre Idx to be deleted has to be lower than narr.
 *
 * Function copies cluster which we want to remove to temporary cluster.
 * Thereafter shoves every cluster backwards by 1 index and
 * copies unwanted cluster to last index possible. Then clear_cluster is called to clear
 * this cluster.
 *
 * @param *carr Pointer to array of clusters
 * @param narr Number of clusters in array
 * @param idx Index of array to be removed
 *
 * @return narr Number of arrays left in 'carr'
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);


/**
 * @brief Calculates distance between 2 objects.
 *
 * Function calculated distance between 2 objects according to
 * Euclidean distance formula.
 *
 * @param *o1 Pointer to object
 * @param *o2 Pointer to object
 *
 * @return  The Euclidean distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is
  \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);


/**
 * @brief Calculates minimum distance between clusters.
 * @pre Cluster1 and cluster2 exist.
 *
 * Function sets distance between 1st object in cluster1 and
 * 1st object in cluster2 with obj_distance. Then it starts to look for
 * new lowest distance between these clusters. If that occurs new minimum
 * distance between these clusters is set as found value.
 *
 * @param *c1 Pointer to cluster_t structure
 * @param *c2 Pointer to cluster_t structure
 *
 * @return min_d Distance between 2 closest neighbours of different clusters
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);


/**
 * @brief Finds index of clusters which are closest.
 *
 * Function sets 1st and 2nd cluster as closest neighbours and also loads
 * their indexes into c1 and c2. Then it starts to look for the new lowest distance
 * between other clusters. If different group of clusters has lower distance than
 * previously set minimum they are chosen as closest neighbours and their indexes are
 * parsed into c1 and c2.
 *
 * @post c1 has to be lower than c2, otherwise switches these numbers.
 * @param *carr Pointer to array of clusters
 * @param narr Number of clusters in array
 * @param[OUT] *c1 Index of 1st cluster neighbour
 * @param[OUT] *c2 Index of 2nd cluster neighbour
 *
 * @return Void.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);


/**
 * @brief Sorts objects in given cluster.
 *
 * Function sorts cluster with Quick sort algorithm.
 *
 * @param *c Pointer to cluster_t structure
 *
 * @return Void.
 */
void sort_cluster(struct cluster_t *c);


/**
 * @brief Prints content of objects in given cluster.
 *
 * Functions prints every object (ID, X, Y),
 * that can be found in cluster 'c'.
 *
 * @param *c Pointer to cluster_t structure
 *
 * @return Void.
 */
void print_cluster(struct cluster_t *c);


/**
 * @brief Parses input file into each object separately.
 * @pre Text file exists and given name is correct.
 *
 * Function checks if input file is in a required format, if so,
 * allocates memory for array of clusters of entered count.
 * Thereafter converts and parses ID,X and Y coordinate into each object,
 * calling append_cluster. If any error occurs functions is terminated,
 * file is closed and allocated memory is freed.
 *
 * @param *filename Pointer to name of input file
 * @param **arr is a pointer that points to another pointer which will be set to point at the array of clusters
 *
 * @return count Number of objects parsed from file
 */
int load_clusters(char *filename, struct cluster_t **arr);


/**
 * @brief Prints every cluster with their array of objects.
 *
 *
 * Function calls print_cluster to print content of cluster with index i,
 * up until all clusters are printed.
 *
 * @param *carr Pointer to array of clusters
 * @param narr Number of clusters in array
 *
 * @return Void.
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */
