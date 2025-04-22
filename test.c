#include "hash.h"
#include "parser.h"
#include "segment.h"
#include "cpu.h"
#include <stdio.h>

int main(){
    CPU* cpu = cpu_init(1024);
    Instruction* inst = parse_data_instruction("MOV AX 10,20,30,40", cpu->memory_handler->allocated);
    
    allocate_variables(cpu, &inst, 1);
    print_data_segment(cpu);


    return 0;
}