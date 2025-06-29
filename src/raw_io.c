#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "raw_io.h"

// raw mode 실행
struct termios enable_raw_mode() {
    struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    return orig_termios;
}

// raw mode 종료
void disable_raw_mode(struct termios* orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

/**
* @brief Reads a line of input from the user.
* @return A pointer to a string containing the input line, or NULL on EOF or error.
*/
char* read_input(struct history* history_list) {
    struct termios orig_termios = enable_raw_mode();

    char* buffer = (char*)malloc(1024 * sizeof(char));
    int len = 0;
    char c;
    while (1) {
        read(STDIN_FILENO, &c, 1);

        // EOF (Ctrl + D)
        if (c == 0x04) {
            disable_raw_mode(&orig_termios);
            free(buffer);
            return NULL;
        }

        // Press Enter('\n)
        else if (c == '\n') {
            buffer[len] = '\0';
            if (strcmp(buffer, "")) {
                add_history_node(&history_list, buffer);
            }
            printf("\n");
            break;
        }
        
        // Press Backspace(DEL)
        else if (c == 127 || c == '\b') {
            if (len > 0) {
                len--;
                // 커서 뒤로 -> 공백 -> 커서 뒤로
                write(STDOUT_FILENO, "\b \b", 3);
            }
        }

        // Escape sequence(방향 키)
        else if (c == 27) { 
            char seq[2];
            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);
            if (seq[0] == '[') {

                if (history_list->head->next == history_list->tail) {
                     continue;
                }

                // 위쪽 화살표
                if (seq[1] == 'A') {
                    if (history_list->curr->prev != history_list->head) {
                        // 기존 입력 지우기
                        for (int i = 0; i < strlen(history_list->curr->command); i++) {
                            write(STDOUT_FILENO, "\b \b", 3);
                        }
                        history_list->curr = history_list->curr->prev;
                        strcpy(buffer, history_list->curr->command);
                        len = strlen(buffer);
                        write(STDOUT_FILENO, buffer, len);
                    }
                }
                
                // 아래쪽 화살표
                else if (seq[1] == 'B') { 

                    if (history_list->curr == history_list->tail) {
                        continue;;
                    }

                    if (history_list->curr->next != history_list->tail) {
                        // 기존 입력 지우기
                        for (int i = 0; i < strlen(history_list->curr->command); i++) {
                            write(STDOUT_FILENO, "\b \b", 3);
                        }
                        history_list->curr = history_list->curr->next;
                        strcpy(buffer, history_list->curr->command);
                        len = strlen(buffer);
                        write(STDOUT_FILENO, buffer, len);
                    } else {
                        for (int i = 0; i < strlen(history_list->curr->command); i++) {
                            write(STDOUT_FILENO, "\b \b", 3);
                        }
                        history_list->curr = history_list->curr->next;
                        strcpy(buffer, "");
                        len = strlen("");
                    }
                }
            }
        } 

        // 입력
        else {
            buffer[len++] = c;
            write(STDOUT_FILENO, &c, 1);
        }
    }

    disable_raw_mode(&orig_termios);
    return buffer;
}