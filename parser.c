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
    if(sscanf(inst->operand2, "%s,%s", buffer, buffer2)>1){
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
    }
    else{
        elements_count = 1;
    }
    hashmap_insert(memory_locations, inst->mnemonic, last_adress_used);
    last_adress_used = elements_count;
    return inst;
}


Instruction* parse_code_instruction(const char* line, HashMap *labels, int code_count){
    Instruction* inst = (Instruction*)malloc(sizeof(Instruction));
    char* label = malloc(20);

    sscanf(line, "%s: %s %s,%s\n", label, inst->mnemonic, inst->operand1, inst->operand2);


    return inst;

}