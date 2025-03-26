#ifndef PAR
#define PAR
#include "segment.h"

typedef struct{
    char *mnemonic;
    char *operand1;
    char *operand2;
} Instruction;

typedef struct{
    Instruction** data_instructions; //Tableau des instructions de .DATA
    int data_count; //Taille du tab d'instructions DATA
    Instruction** code_instructions; //Tableau des instructions de .CODE
    int code_count; // Taille du tab d'instructions CODE
    HashMap* labels; // labels -> indices dans code_instructions
    HashMap* memory_locations; // noms de variables -> adresse memoire
} ParserResult;

Instruction* parse_data_instruction(const char *line, HashMap* memory_locations);
#endif