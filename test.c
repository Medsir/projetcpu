#include "hash.h"
#include "parser.h"
#include "segment.h"
#include "cpu.h"
#include <stdio.h>

int main(){
    /*
    CPU* cpu = setup_test_environnement();
    cpu_destroy(cpu);
    */
    HashMap* h = hashmap_create();
    printf("%d\n",h->table[0]);
    //le calloc ne marche pas.
    
    return 0;
}