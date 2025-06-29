#ifndef PARSER_H
#define PARSER_H

#include "redirect.h"
#include "history.h"

#define MAX_COMMAND_SIZE 1024

struct command {
    char* name;
    char** args;
    int argc;

    enum redirect_type redirect_type;
    char* input_file;
    char* output_file;
    
    int is_background;
};

// 사용자 입력 파싱하기
struct command parse_single_command(char *input);

// 메모리 공간 생성
char* create_char_mem();

struct command* parse_input(char *input, int* num_cmds);


// 메모리 해제
void free_command(struct command* cmd);

void free_commands(struct command* cmds, int num_cmds);

#endif