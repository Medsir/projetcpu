#include "hash.h"
#include "parser.h"
#include <stdio.h>

int main(){
    HashMap* memory_locations = hashmap_create();
    parse_code_instruction("loop: MOV AX,6", memory_locations, 10);

    return 0;
}