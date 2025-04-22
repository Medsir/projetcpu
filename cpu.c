#include "cpu.h"
#include <stdio.h>
#include <string.h>

CPU *cpu_init(int memory_size){
    CPU* cpu = (CPU*)malloc(sizeof(CPU));
    cpu->memory_handler = memory_init(memory_size);
    cpu->context = hashmap_create();
    hashmap_insert(cpu->context, "AX", calloc(sizeof(int), 1));
    hashmap_insert(cpu->context, "BX", calloc(sizeof(int), 1));
    hashmap_insert(cpu->context, "CX", calloc(sizeof(int), 1));
    hashmap_insert(cpu->context, "DX", calloc(sizeof(int), 1));
    cpu->constant_pool = hashmap_create();
    return cpu;
}

CPU* cpu_destroy(CPU* cpu){
    free_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
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


void allocate_variables(CPU *cpu, Instruction** data_instructions, int data_count){
    /*Calcul de l'espace nécessaire :*/
    int necessary_space = 0;
    
    /*Ce tableau servira a stocker temporairement les poineurs des éléments*/
    void** datas = calloc(cpu->memory_handler->total_size,sizeof(void*));

    /*On parcourt chaque instruction*/
    Instruction* curr;
    char* content;
    char* token;
    for(int i=0; i<data_count; i++){
        curr = data_instructions[i];
        content = strdup(curr->operand2);
        /*On compte les éléments et on ajoute le pointeur vers chaque élément dans la liste temporaire*/
        char* token = strdup(strtok(content, ","));
        while(token!=NULL){ 
            datas[necessary_space] = (void*)token;
            necessary_space++;
            token = strdup(strtok(NULL, ","));
        }
        free(content);
    }
    /* Declaration du segment DS nécessaire*/
    create_segment(cpu->memory_handler, "DS", last_adress_used, last_adress_used+necessary_space);
    last_adress_used+= necessary_space;
    /*Enregistrer les pointeurs dans le memory handler*/
    int i = 0;
    while(datas[i]){
        store(cpu->memory_handler, "DS", i, datas[i]);
        i++;
    }
    /*Libérer le tableau ayant servi a stocker les pointeurs temporairement*/
    free(datas);
}

void print_data_segment(CPU* cpu){
    Segment* data = (Segment*)hashmap_get(cpu->memory_handler->allocated, "DS");
    if(!data) return;
    printf("---- PRINTING .DATA SEGMENT ---- \n");
    for(int i=data->start; i<data->size; i++){
        printf("[%s] ", (char*)cpu->memory_handler->memory[i]);
    }
    printf("\nDimensions : [%d, %d]\n", data->start, data->start+data->size-1);
}

CPU* setup_test_environnement(){
    //Initialiser le CPU
    CPU* cpu = cpu_init(1024);
    if(!cpu){
        fprintf(stderr, "Error : CPU initialization failed\n");
        return NULL;
    }
    //Initialiser les registres avec des valeurs specifiques
    int *ax = (int *)hashmap_get(cpu->context, "AX");
    int *bx = (int *)hashmap_get(cpu->context, "BX");
    int *cx = (int *)hashmap_get(cpu->context, "CX");
    int *dx = (int *)hashmap_get(cpu->context, "DX");

    *ax = 3;
    *bx = 6;
    *cx = 100;
    *dx = 0;
    //Creer et initialiser le segment de données
    if(!hashmap_get(cpu->memory_handler->allocated, "DS")){
        create_segment(cpu->memory_handler, "DS", 0, 20);
        
        //Initialiser le segment de donnees avec des valeurs de test
        for(int i=0; i<10; i++){
            int* value = (int*)malloc(sizeof(int));
            *value = i*10 + 5; //Valeurs 5, 15, 25, 35
            store(cpu->memory_handler, "DS", i, value);
        }

    }
    printf("Test environnement initialized.\n");
    return cpu;
}