#include "hash.h"
#include "parser.h"
#include "segment.h"
#include "cpu.h"
#include <stdio.h>

int main(){
    CPU* cpu = setup_test_environnement();
    cpu_destroy(cpu);
    return 0;
}