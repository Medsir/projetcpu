#include "hash.h"
#include "parser.h"
#include "segment.h"
#include "cpu.h"
#include <stdio.h>

int run_program(CPU* cpu){
    int running = 1;
    char input= malloc(1);

    printf("Affichage des registres :\n");
    
    int *ax = (int *)hashmap_get(cpu->context, "AX");
    int *bx = (int *)hashmap_get(cpu->context, "BX");
    int *cx = (int *)hashmap_get(cpu->context, "CX");
    int *dx = (int *)hashmap_get(cpu->context, "DX");
    int *ip = (int *)hashmap_get(cpu->context, "IP");
    int *zf = (int *)hashmap_get(cpu->context, "ZF");
    int *sf = (int *)hashmap_get(cpu->context, "SF");

    printf("AX : %d\nBX : %d\nCX : %d\nDX : %d\nIP : %d\nZF : %d\nSF : %d\n", *ax, *bx, *cx, *dx, *ip, *zf, *sf);

    printf("\n");
    printf(".DATA segment :\n");
    print_data_segment(cpu);
    printf("\n");

    /* Parser le data segment avec la fonction parse()*/

    Segment* code_seg = hashmap_get(cpu->memory_handler->allocated, "CS");
    if(!code_seg){
        printf("Segment code non initialise\n");
        return 0;
    }
    else{
        printf("Affichage des instructions dans le segment .CODE\n");
        Instruction* instr;
        for(int i=code_seg->start; i<code_seg->start + code_seg->size; i++){
            instr = (Instruction*)cpu->memory_handler->memory[i];
            if(instr){
                printf("%s %s %s\n", instr->mnemonic, instr->operand1, instr->operand2);
            }
        }
    }
    Instruction* curr_inst = (Instruction*)cpu->memory_handler->memory[*ip];
    printf("probleme : Nous n'avons pas pu tester les fonctions d'execution du code car la librairie regex.h n'est pas fournie dans notre libc sur windows\n");
    while (running){
        printf("Quitter (q) | Avancer (Entree)\n");
        input = getchar();
        switch (input)
        {
        case 'q':
            printf("AX : %d BX : %d CX : %d DX : %d IP : %d ZF : %d SF : %d\n", *ax, *bx, *cx, *dx, *ip, *zf, *sf);
            printf("Sortie du programme\n");
            return 0;
            break;
        case '\n':
            printf("--------------\n");
            if(curr_inst){
                //resolve constants
                //execute instruction

                /*continuer sur la prochaine instruction*/
                printf("%s %s %s\n", curr_inst->mnemonic, curr_inst->operand1, curr_inst->operand2);
                curr_inst = fetch_next_instruction(cpu);
            }
            else{
                printf("Fin du code \n");
            }
        default:
            break;
        }
    }
    
}

int main(){
    CPU* cpu = setup_test_environnement();
    run_program(cpu);
    return 0;
}