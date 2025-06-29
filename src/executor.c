#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "executor.h"
#include "parser.h"
#include "redirect.h"

// 내부 명령어 처리
int handle_internal_command(struct command cmd) {
    // cd
    if (strcmp(cmd.name, "cd") == 0) {
        if (cmd.args[1] == NULL) {
            fprintf(stderr, "cd: 경로를 입력하세요.\n");
        } 
        else if (chdir(cmd.args[1]) != 0) {
            perror("cd 실패");
        }
        return 1; // 내부 명령 처리 완료
    }

    // pwd
    if (strcmp(cmd.name, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd 실패");
        }
        return 1; // 내부 명령 처리 완료
    }

    // export
    if (strcmp(cmd.name, "export") == 0) {
        if (cmd.args[1] == NULL) {
            fprintf(stderr, "export: 변수=값 형식으로 입력하세요.\n");
        } 
        else {
            char* equal_sign = strchr(cmd.args[1], '=');
            if (!equal_sign) {
                fprintf(stderr, "export: 변수=값 형식이 아닙니다.\n");
            } 
            else {
                *equal_sign = '\0';
                char* key = cmd.args[1];
                char* value = equal_sign + 1;
                if (setenv(key, value, 1) != 0) {
                    perror("환경변수 설정 실패");
                }
            }
        }
        return 1; // 내부 명령 처리 완료
    }

    // unset
    if (strcmp(cmd.name, "unset") == 0) {
        if (cmd.args[1] == NULL) {
            fprintf(stderr, "unset: 삭제할 환경 변수를 입력하세요.\n");
        } 
        else {        
            char* key = cmd.args[1];

            if (unsetenv(key) != 0) {
                perror("환경 변수 제거 실패");
            }
            return 1;
        }
    }

    return 0; // 외부 명령 실행 필요
}


int execute_commands(struct command* cmds, int num_cmds) {
 
    int pipe_fd[2] = {-1,-1};
    int prev_read_fd = -1;

    for(int i = 0; i < num_cmds; i++){
        // 마지막 명령이 아닐 때만 pipe 생성
        if (i < num_cmds - 1) {
            if(pipe(pipe_fd) == -1){
                perror("pipe()");
                return -1;
            }
        }

        // 내부 명령어 처리
        if (handle_internal_command(cmds[i])) {
            if (prev_read_fd != -1) close(prev_read_fd);
            if (pipe_fd[0] != -1) close(pipe_fd[0]);
            if (pipe_fd[1] != -1) close(pipe_fd[1]);
            continue;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return -1;
        }

       if (pid == 0) {
            //리디렉션 처리
            if (cmds[i].redirect_type == REDIRECT_INPUT && cmds[i].input_file) {
                if (handle_input_redirect(cmds[i].input_file) != 0){
                    perror("<");
                    exit(EXIT_FAILURE);
                }
            } 
            else if (cmds[i].redirect_type == REDIRECT_OUTPUT && cmds[i].output_file) {
                if (handle_output_redirect(cmds[i].output_file) != 0){
                    perror(">");
                    exit(EXIT_FAILURE);
                }
            } 
            else if (cmds[i].redirect_type == REDIRECT_APPEND && cmds[i].output_file) {
                if (handle_append_redirect(cmds[i].output_file) != 0){
                    perror(">>");
                    exit(EXIT_FAILURE);
                }
            }

            //파이프 연결
            if (prev_read_fd != -1) {
                dup2(prev_read_fd, STDIN_FILENO);
                close(prev_read_fd);
            }

            if (i < num_cmds - 1) {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            //외부 명령 실행
            if (execvp(cmds[i].name, cmds[i].args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        else {
            if (cmds[i].is_background == 0) {
                int status;
                if (waitpid(pid, &status, 0) == -1) {
                    perror("waitpid failed");
                    return -1;
                }
            } 
            else {
                printf("[background] %d\n", pid);
            }

            if (prev_read_fd != -1) {
                close(prev_read_fd);
            }

            if (i < num_cmds - 1) {
                close(pipe_fd[1]);
                prev_read_fd = pipe_fd[0];
            }
        }
    }

    if (prev_read_fd != -1) {
        close(prev_read_fd);
    }
    return 0;
}
