#include "addressing.h"
#include <string.h>
int matches(const char *pattern, const char *string){
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    if(result) {
        fprintf(stderr, "Regex compilation failed for pattern : %s\n", pattern);
        return 0;
    }
    result = regexec(&regex, string, 0, NULL, 0);
    regfree(&regex);
    return result == 0;
}

void* immediate_addressing(CPU* cpu, char* operand){
    if(matches("[0-9]*", operand)){
        void* data = hashmap_get(cpu->constant_pool, operand);
        if(!data){
            /*Cas où la valeur n'y est pas déjà stockée*/
            int* constant = (int*)malloc(sizeof(int));
            *constant = atoi(operand);
            hashmap_insert(cpu->constant_pool, operand, constant);
            return operand;
        }
        else{
            return data;
        }
    }else{
        fprintf(stderr, "Erreur dans immediate_addressing(): %s ne match pas avec le pattern [0-9]*\n", operand);
        return NULL;
    }
}

void* register_addressing(CPU* cpu, const char* operand){
    if(matches("[A-Z]{2}", operand)){
        void* data = hashmap_get(cpu->context, operand);
        if(data){
            return data;
        }
        else{
            return NULL;
        }
    }
    return NULL;
}

void* memory_direct_addressing(CPU* cpu, const char* operand){
    if(matches("\[[0-9]*\]", operand)){
        /*Extraction et conversion de l'adresse*/
        char* tmp = strdup(operand);
        char* symbol = strdup(strtok(tmp, "[]"));
        int index = atoi(symbol);
        free(tmp);
        free(symbol);

        /*Récupérer le data segment si il existe*/
        Segment* data_segment = (Segment*)hashmap_get(cpu->memory_handler->allocated, "DS");
        if(!data_segment){
            printf("Pas de data segment\n");
            return NULL;
        }

        /*Trouver l'adresse dans le segment data*/
        void* data = cpu->memory_handler->memory[data_segment->start+index];

        /*renvoyer le pointeur*/
        return data;
    }
    return NULL;
}

void* register_indirect_addressing(CPU* cpu, const char* operand){
    if(matches("\[[A-Z]{2}\]", operand)){
        char* tmp = strdup(operand);
        char* nom_registre = strdup(strtok(tmp, "[]"));
        void* data = hashmap_get(cpu->context, nom_registre);
        if(data){
            return data;
        }
        return NULL;
    }
    return NULL;
}

void handle_MOV(CPU* cpu, void* src, void* dest){
    /*Copier le contenu (supposé ici un entier) à l'emplacement src vers dest*/
    if(dest && src){
        memcpy(dest, src, sizeof(int));
    }
}

void* resolve_addressing(CPU* cpu, const char* operand){
    if(matches("[0-9]*", operand)) return immediate_addressing(cpu, operand);
    if(matches("[A-Z]{2}", operand)) return register_addressing(cpu, operand);
    if(matches("\[[0-9]*\]", operand)) return memory_direct_addressing(cpu, operand);
    if(matches("\[[A-Z]{2}\]", operand)) return register_indirect_addressing(cpu, operand);
    return NULL;
}

int handle_instruction(CPU* cpu, Instruction* instr, void* src, void* dest){
    if(strcmp(instr->mnemonic, "MOV")==0){
        handle_MOV(cpu, src, dest);
        return 1;
    }
    if(strcmp(instr->mnemonic, "ADD")==0){
        *(int*)dest = *(int*)src + *(int*)dest;
        return 1;
    }
    if(strcmp(instr->mnemonic, "CMP")==0){
        int* zf = (int*)hashmap_get(cpu->context,"ZF");
        int* sf = (int*)hashmap_get(cpu->context,"SF");
        int res = *(int*)dest - *(int*)src;
        if(res == 0){
            *zf = 1;
        }else if (res < 0){
            *sf = 1;
        }
        return 1;
    }
    if(strcmp(instr->mnemonic, "JMP")==0){
        int address = atoi(instr->operand1);
        int* ip = (int*)hashmap_get(cpu->context,"IP");
        *ip = address;
        return 1;
    }
    if(strcmp(instr->mnemonic, "JZ")==0){
        int address = atoi(instr->operand1);
        int* zf = (int*)hashmap_get(cpu->context,"ZF");
        int* ip = (int*)hashmap_get(cpu->context,"IP");
        if(*zf == 1){
            *ip = address;
        }
        return 1;
    }
    if(strcmp(instr->mnemonic, "JNZ")==0){
        int address = atoi(instr->operand1);
        int* zf = (int*)hashmap_get(cpu->context,"ZF");
        int* ip = (int*)hashmap_get(cpu->context,"IP");
        if(*zf == 0){
            *ip = address;
        }
        return 1;
    }
    if(strcmp(instr->mnemonic, "HALT")==0){
        int* ip = (int*)hashmap_get(cpu->context,"IP");
        Segment* cs = hashmap_get(cpu->memory_handler->allocated, "CS");
        *ip = cs->start+cs->size;
        return 1;    
    }
    if(strcmp(instr->mnemonic, "PUSH")==0){
        if(instr->operand1 == NULL){
            int* ax = (int*)hashmap_get(cpu->context,"AX");
            push_value(cpu, *ax);
        }
        else{
            int* constant = (int*)malloc(sizeof(int));
            *constant = atoi(instr->operand2);
            push_value(cpu, *constant);
        }
        return 1;
    }
    if(strcmp(instr->mnemonic, "POP")==0){
        if(instr->operand1 == NULL){
            int* ax = (int*)hashmap_get(cpu->context,"AX");
            pop_value(cpu, ax);
        }
        else{
            pop_value(cpu, (int*)hashmap_get(cpu->context, instr->operand1));  
        }
        return 1;
    }
    return 0;
}

int execute_instruction(CPU* cpu, Instruction *instr){
    void* addr1 = resolve_addressing(cpu, instr->operand1);
    void* addr2 = resolve_addressing(cpu, instr->operand2);
    handle_instruction(cpu, instr, addr1, addr2);
    return 0;
}
