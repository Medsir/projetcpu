#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

CPU *cpu_init(int memory_size){
    CPU* cpu = (CPU*)malloc(sizeof(CPU));
    cpu->memory_handler = memory_init(memory_size);
    cpu->context = hashmap_create();
    int AX = 0;
    int BX = 0;
    int CX = 0;
    int DX = 0;
    hashmap_insert(cpu->context, "AX", &AX);
    hashmap_insert(cpu->context, "BX", &BX);
    hashmap_insert(cpu->context, "CX", &CX);
    hashmap_insert(cpu->context, "DX", &DX);
}

CPU* cpu_destroy(CPU* cpu){
    free_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    free(cpu);
    return NULL;
}

void* store(MemoryHandler *handler, const char *segment_name, int pos, void *data){
    Segment* seg = hashmap_get(handler->allocated, segment_name);
    if(seg){
        handler->memory[seg->start+pos] = data;
        return data;
    }
    return NULL;
    
}

void* load(MemoryHandler *handler, const char* segment_name, int pos){
    Segment* seg = hashmap_get(handler->allocated, segment_name);
    if(seg){
        return handler->memory[seg->start+pos];
    }
    return NULL;
}