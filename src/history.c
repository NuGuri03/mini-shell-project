#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

struct history* create_history(){
    struct history* history_list = (struct history*)malloc(sizeof(struct history));
    
    history_list->head = (struct node*)malloc(sizeof(struct node));
    history_list->tail = (struct node*)malloc(sizeof(struct node));
    history_list->head->command = strdup("");
    history_list->tail->command = strdup("");
    history_list->head->prev = NULL;
    history_list->head->next = history_list->tail;
    history_list->tail->prev = history_list->head;
    history_list->tail->next = NULL;

    return history_list;
}

// history_list에 명령어 추가하기
void add_history_node(struct history** history_list, char* command) {
    struct node* x = (struct node*)malloc(sizeof(struct node));
    x->command = strdup(command);

    // tail 앞에 삽입
    x->prev = (*history_list)->tail->prev;
    x->next = (*history_list)->tail;

    (*history_list)->tail->prev->next = x;
    (*history_list)->tail->prev = x;

    // 새 명령어가 추가되었으므로 curr는 tail로 초기화
    (*history_list)->curr = (*history_list)->tail;
}

void free_history_list(struct history* history_list) {
    struct node* curr = history_list->head->next;

    while (curr != NULL && curr != history_list->tail) {
        struct node* next = curr->next;

        if (curr->command != NULL) {
            free(curr->command); 
        }

        free(curr);
        curr = next;
    }

    free(history_list);
}

void save_history_to_file(struct history* history_list, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    struct node* curr = history_list->head->next;
    while (curr != history_list->tail) {
        fprintf(fp, "%s\n", curr->command);
        curr = curr->next;
    }

    fclose(fp);
}

void load_history_from_file(struct history** history_list, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        add_history_node(history_list, buffer);
    }

    fclose(fp);
}
