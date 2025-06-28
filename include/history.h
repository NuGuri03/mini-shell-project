#ifndef HISTORY_H
#define HISTORY_H

typedef struct Node {
    char* command;
    struct Node* prev;
    struct Node* next;
} history_node;

typedef struct History {
    history_node* head;
    history_node* tail;
    history_node* curr;
} history;

history* create_history();
void add_history_node(history** history_list, char* commad);
void free_history_list(history* history_list);

#endif