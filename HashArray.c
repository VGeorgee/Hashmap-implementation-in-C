#include "include/HashArray.h"
#define NextNPrime 5
#define MAXLOADFACTOR 0.75

Map * new_map(int n, int (* equals)(const void *a, const void*b), int (* hashcode)(const void *a))
{
    int i;
    Map *new_map = malloc(sizeof(Map));
    new_map->harray = malloc(sizeof(LinkedList *) * n);

    for(i = 0; i < n; i++){
        new_map->harray[i] = new_linked_list(equals, hashcode);
    }
    new_map->tablesize = n;
    new_map->maxloadfactor = MAXLOADFACTOR;
    new_map->elements = 0;

    new_map->equals = equals;
    new_map->hashcode = hashcode;

    return new_map;
};

/**
 *
 * make call to map_put_node by creating node from arguments
 *
 * */

void map_put(Map *this_map, void *key, void *value)
{

    if(get_load_factor(this_map) >= this_map->maxloadfactor)
        rehash(this_map);

    int hash = this_map->hashcode(key);

    hash %= this_map->tablesize;

    add_new_node(this_map->harray[hash], key, value);

    this_map->elements++;

};

void map_put_node(Map *this_map, NODE *node_pointer){

    if(get_load_factor(this_map) >= this_map->maxloadfactor)
        rehash(this_map);

    int hash = this_map->hashcode(node_pointer->key);

    hash %= this_map->tablesize;
    add_node_pointer(this_map->harray[hash], node_pointer);

    this_map->elements++;
}

void *map_get(Map *this_map, void *key){
    return get_node(this_map->harray[this_map->hashcode(key) % this_map->tablesize], key);
};

NODE *map_get_node(Map *this_map, void *key){
    return get_node_pointer(this_map->harray[this_map->hashcode(key) % this_map->tablesize], key);
};

void *map_remove(Map *this_map, void *key){
    void *removed_element = remove_node(this_map->harray[this_map->hashcode(key) % this_map->tablesize], key);

    if(removed_element != NULL)
        this_map->elements--;

    return removed_element;
};

int map_contains(Map *this_map, void *key){
    return map_get_node(this_map->harray[this_map->hashcode(key) % this_map->tablesize], key) == -1 ? 0 : 1;
};

int map_isempty(Map *this_map){
    return this_map->elements == 0;
};


void rehash(Map *this_map){
    int i, nextprime = generate_prime(this_map->tablesize);

    LinkedList **newLL = malloc(sizeof(LinkedList *) *nextprime);

    for(i = 0; i < nextprime; i++)
        newLL[i] = new_linked_list();

    for(i = 0; i < this_map->tablesize; i++)
    {
        NODE *f = this_map->harray[i]->HEAD;
        while(f != NULL)
        {
            map_put_node(newLL[this_map->hashcode(f->key) % nextprime], f);
            NODE *elozo = f;
            f = f->next;
            elozo->next = NULL;
        }
        free(this_map->harray[i]);
    }

    this_map->tablesize = nextprime;
    this_map->harray = newLL;

};

double  get_load_factor(Map *this_map){
    if(this_map->elements > 0)
        return (double)this_map->elements / this_map->tablesize;

    return 0.0;
};

void print_all_nodes(Map *this_map){
    for(int i = 0; i < this_map->tablesize; i++){
        if(this_map->harray[i]){
            get_iterators(this_map->harray[i]);
            NODE **t = this_map->harray[i]->Iterator;
            for(int j = 0; j < this_map->harray[i]->elements; j++)
                printf("%15s %8d %2d\n", t[j]->key, t[j]->value, j);
        }
    }
}

int is_prime(int n){
    int i;
    for(i = 2; i <= n/2; i++)
    {
        if(n % i == 0)
            return 0;
    }
    return 1;
};

int generate_prime(int n){
    int k = 0, np = n;
    while(k < NextNPrime)
    {
        if(is_prime(++np))
            k++;
    }
    return np;
};
