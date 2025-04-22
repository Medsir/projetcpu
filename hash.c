#include "hash.h"
#include <stdlib.h>
#include <string.h>


unsigned long simple_hash(const char* str){
    int sum=0;
    int i=0;
    while(str[i] != '\0'){
        sum+=(int)(str[i]);
        i++;
    }
    return sum%TABLESIZE;
}

HashMap* hashmap_create(){

    HashMap* new = (HashMap*)malloc(sizeof(HashMap));
    /*?? peut etre initialiser a 0 ?*/
    new->size = TABLESIZE;
    new->table = calloc(new->size, sizeof(HashEntry));
    return new;
}

int hashmap_insert(HashMap* map, const char* key, void *value){
    int index = simple_hash(key);
    int i = index;
    while(map->table[i].key != 0){
        if(i=index){return 0;}
        i=(i+1)%map->size;
    }
    map->table[i].key = strdup(key);
    map->table[i].value = value;
    return i;
}

void* hashmap_get(HashMap* map, const char* key){
    int index = simple_hash(key);
    if(map->size > index){
        void *data = map->table[index].value;
        return data;
    }
    return NULL;
}

int hashmap_remove(HashMap *map, const char* key){
    int index = simple_hash(key);
    free(map->table[index].key);
    free(map->table[index].value);
    map->table[index].key = NULL;
    map->table[index].value = NULL;
    return 1;
}

void hashmap_destroy(HashMap* map){
    for(int i; i<map->size; i++){
        free(map->table[i].key);
        free(map->table[i].value);
    }
    free(map->table);
    free(map);
}
