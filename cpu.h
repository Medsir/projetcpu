#ifndef _CPU_
#define _CPU_

#include "segment.h"
#include "parser.h"
#include "hash.h"

typedef struct{
    MemoryHandler* memory_handler;
    HashMap* context;
} CPU;


CPU *cpu_init(int memory_size);
CPU* cpu_destroy(CPU* cpu);
#endif