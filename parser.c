#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int last_adress_used = 0;

Instruction* parse_data_instruction(const char *line, HashMap* memory_locations){
    Instruction* inst = (Instruction*)malloc(sizeof(Instruction));
    inst->mnemonic = (char*)malloc(20);
    inst->operand1 = (char*)malloc(20);
    inst->operand2 = (char*)malloc(20);
    sscanf(line, "%s %s %s\n", inst->mnemonic, inst->operand1, inst->operand2);

    printf("mnemonic : %s | operand1 : %s | operand2 : %s\n", inst->mnemonic, inst->operand1, inst->operand2);

    /*parser un potentiel array (trouver le nombre d'éléments)*/
    int elements_count = 0;
 
    char* tmp = strdup(inst->operand2);
    char* token = strdup(strtok(tmp, ","));
    while(token!=NULL){
        elements_count++;
        token = strdup(strtok(NULL, ","));
        free(token);
    }
    free(tmp);
    hashmap_insert(memory_locations, inst->mnemonic, last_adress_used);
    last_adress_used += elements_count;
    return inst;
}


Instruction* parse_code_instruction(const char* line, HashMap *labels, int code_count){
    Instruction* inst = (Instruction*)malloc(sizeof(Instruction));
    inst->mnemonic = NULL;
    int i=0;
    char c = line[i];
    int is_label = 0;
    while((c != ' ') && (c != '\0')){
        c = line[i];
        if(c == ':') {
            is_label = 1;
            break;
        }
        i++;
    }

    const char* separateurs = " :,";
    char* tmp = strdup(line);
    if(is_label){
        //Parser le reste de la ligne
        char* label = strtok(tmp, separateurs);
        hashmap_insert(labels, label, &code_count);
        inst->mnemonic = strdup(strtok(NULL, separateurs));
        inst->operand1 = strdup(strtok(NULL, separateurs));
        inst->operand2 = strdup(strtok(NULL, separateurs));
        printf("Ligne parsee avec label : %s %s %s %s\n", label, inst->mnemonic, inst->operand1, inst->operand2);
    }
    else{
        inst->mnemonic = strdup(strtok(tmp, separateurs));
        inst->operand1 = strdup(strtok(NULL, separateurs));
        inst->operand2 = strdup(strtok(NULL, separateurs));
        printf("Ligne parsee sans label : %s %s %s\n", inst->mnemonic, inst->operand1, inst->operand2);
    }
    free(tmp);
    return inst;
}


ParserResult* init_result(){
    ParserResult* result = (ParserResult*)malloc(sizeof(ParserResult));
    result->data_instructions = (Instruction**)malloc(sizeof(Instruction*)*20);
    result->data_count = 0;
    result->code_instructions = (Instruction**)malloc(sizeof(Instruction*)*20);
    result->code_count = 0;
    result->memory_locations = hashmap_create();
    result->labels = hashmap_create();
    return result;
}

ParserResult* parse(const char* filename){
    FILE* file = fopen(filename, "r");

    ParserResult* result = init_result();
    char buffer[256];
    fgets(buffer, 7, file);
    
    if(strcmp(buffer, ".DATA\n")==0){
        /*Après la ligne .DATA*/
        fgets(buffer, 256, file);
        printf("Entree dans .DATA\n");
         while(strcmp(buffer, ".CODE\n")!=0){
            
            result->data_instructions[result->data_count] = parse_data_instruction(buffer, result->memory_locations);
            result->data_count++;
            fgets(buffer, 256, file);
        }
        printf("Entree dans .CODE\n");
    }
    else{
        printf(stderr, "Erreur dans parse(%s)\n", filename);
        return NULL;
    }
    /*Après la ligne .CODE*/
    while(fgets(buffer, 256, file)){
        result->code_instructions[result->code_count] = parse_code_instruction(buffer, result->labels, result->code_count);
        result->code_count++;
    }
    fclose(file);
    return result;
}

void free_instruction(Instruction* inst){
    if(inst){
        free(inst->mnemonic);
        free(inst->operand1);
        free(inst->operand2);
    }
}

void free_parser_result(ParserResult *result){
    for(int i=0; i<result->data_count; i++) free_instruction(result->data_instructions[i]);
    free(result->data_instructions);
    for(int i=0; i<result->code_count; i++) free_instruction(result->code_instructions[i]);
    free(result->code_instructions);
    hashmap_destroy(result->labels);
    hashmap_destroy(result->memory_locations);
}
