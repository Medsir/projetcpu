#include "hash.h"
#include "parser.h"
#include <stdio.h>

int main(){
    HashMap* memory_locations = hashmap_create();
    printf("Hashmap cree.\n");

    parse_data_instruction("X DW 15", memory_locations);

    char* x = (char*)hashmap_get(memory_locations, "X");
    printf("X = %d\n", x);


    return 0;
}