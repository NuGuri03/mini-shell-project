#ifndef HISTORY_H
#define HISTORY_H

struct node {
    char* command;
    struct node* prev;
    struct node* next;
};

struct history {
    struct node* head;
    struct node* tail;
    struct node* curr;
};

struct history* create_history();

void add_history_node(struct history** history, char* commad);

void free_history(struct history* history);

void save_history_to_file(struct history *history, const char *filename);

void load_history_from_file(struct history **history, const char *filename);

#endif