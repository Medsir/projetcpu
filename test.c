#include "hash.h"
#include "parser.h"
#include "cpu.h"
#include <stdio.h>

int main(){
    ParserResult* result = parse("test_parse.txt");
    free_parser_result(result);
    return 0;
}