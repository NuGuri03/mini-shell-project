#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "parser.h"
#include "executor.h"
#include "history.h"
#include "raw_io.h"

/**
 * @brief Print prompt with flush.
 */
void print_prompt() {
    char hostname[256];
    char *user;
    char cwd[1024];

    // get user information
    user = getenv("USER");
    if (user != NULL) {
        struct passwd *pw = getpwuid(getuid());
        user = pw ? pw->pw_name : "unknown";
    }

    // get host
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strcpy(hostname, "unknown");
    }

    // get directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // mysh(보라), user@host(파랑), 경로(초록)
        printf(
            "\033[1;35mmysh\033[0m::"
            "\033[1;34m%s@%s\033[0m:"
            "\033[1;32m%s\033[0m> ",
            user, hostname, cwd
        );
    } 
    else {
        printf(
            "\033[1;35mmysh\033[0m::"
            "\033[1;34m%s@%s\033[0m::~> ",
            user, hostname
        );
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