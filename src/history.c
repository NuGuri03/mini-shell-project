#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

struct history* create_history(){
    struct history* history = (struct history*)malloc(sizeof(struct history));
    
    history->head = (struct node*)malloc(sizeof(struct node));
    history->tail = (struct node*)malloc(sizeof(struct node));

    history->head->command = strdup("");
    history->tail->command = strdup("");

    history->head->prev = NULL;
    history->head->next = history->tail;

    history->tail->prev = history->head;
    history->tail->next = NULL;

    return history;
}

// history_list에 명령어 추가하기
void add_history_node(struct history** history, char* command) {
    struct node* x = (struct node*)malloc(sizeof(struct node));
    x->command = strdup(command);

    // tail 앞에 삽입
    x->prev = (*history)->tail->prev;
    x->next = (*history)->tail;

    (*history)->tail->prev->next = x;
    (*history)->tail->prev = x;

    // 새 명령어가 추가되었으므로 curr는 tail로 초기화
    (*history)->curr = (*history)->tail;
}

void free_history(struct history* history) {
    struct node* curr = history->head->next;

    while (curr != NULL && curr != history->tail) {
        struct node* next = curr->next;

        if (curr->command != NULL) {
            free(curr->command); 
        }

        free(curr);
        curr = next;
    }

    free(history);
}

void save_history_to_file(struct history* history, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    struct node* curr = history->head->next;
    while (curr != history->tail) {
        fprintf(fp, "%s\n", curr->command);
        curr = curr->next;
    }

    fclose(fp);
}

void load_history_from_file(struct history** history, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        add_history_node(history, buffer);
    }

    fclose(fp);
}
