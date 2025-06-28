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

void add_history_node(struct history** history_list, char* commad);

void free_history_list(struct history* history_list);

void save_history_to_file(struct history *history_list, const char *filename);

void load_history_from_file(struct history **history_list, const char *filename);

#endif