#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "executor.h"
#include "history.h"
#include "raw_io.h"

/**
 * @brief Print prompt with flush.
 */
void print_prompt() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("mysh::%s> ", cwd);
    } else {
        printf("mysh::~> ");
    }
    fflush(stdout);
}

int main() {

    struct history* history = create_history();
    load_history_from_file(&history, "history.txt");

    while (1) {
        print_prompt();
        char *input = read_input(history);

        if (input == NULL) {
            save_history_to_file(history, "history.txt");
            printf("\nBye!\n");
            break;
        }

        int num_cmds;
        struct command* cmds = parse_input(input, &num_cmds);

        if (!strcmp(input, "exit")) {
            free(input);
            free_commands(cmds, num_cmds);
            save_history_to_file(history, "history.txt");
            free_history(history);
            printf("Bye!\n");
            break;
        }
        
        free(input);
        execute_commands(cmds, num_cmds);
        free_commands(cmds, num_cmds);
    }

    return 0;
}