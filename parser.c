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

    printf("%s %s %s\n", inst->mnemonic, inst->operand1, inst->operand2);

    /*parser un potentiel array*/
    char buffer[20];
    char buffer2[20];
    int elements_count = 0;
    char c = ' ';
    int i=0;
    while(c != '/0'){
        /*theoriquement : il y'a 1 element de plus que de virgules
        donc on compte le nombre de virgules et on ajoute 1 à la fin */
        if(c == ','){
            elements_count++;
        }
        i++;
        c = inst->operand2[i];
    }
    elements_count++;
    hashmap_insert(memory_locations, inst->mnemonic, last_adress_used);
    last_adress_used = elements_count;
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
        inst->mnemonic = strdup(strtok(tmp, separateurs));
        inst->operand1 = strdup(strtok(NULL, separateurs));
        inst->operand2 = strdup(strtok(NULL, separateurs));
        printf("Ligne parsée avec label : %s %s %s %s\n", label, inst->mnemonic, inst->operand1, inst->operand2);
    }
    else{
        inst->mnemonic = strdup(strtok(tmp, separateurs));
        inst->operand1 = strdup(strtok(NULL, separateurs));
        inst->operand2 = strdup(strtok(NULL, separateurs));
        printf("Ligne parsée sans label : %s %s %s\n", inst->mnemonic, inst->operand1, inst->operand2);
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
        printf("%s\n", buffer);
        /*Après la ligne .DATA*/
         while(strcmp(buffer, ".CODE\n")!=0){
            result->data_instructions[result->data_count] = parse_data_instruction(buffer, result->memory_locations);
            result->data_count++;
        }
    }
    else{
        printf(stderr, "Erreur dans parse(%s)\n", filename);
        return NULL;
    }
    /*Après la ligne .CODE*/

    while(fgets(buffer, 100, file)){
        result->code_instructions[result->code_count] = parse_code_instruction(buffer, result->labels, code_count);
        result->code_count++;
    }



    fclose(file);
    return result;
}