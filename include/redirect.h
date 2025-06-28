#ifndef REDIRECT_H
#define REDIRECT_H

enum redirect_type {
    NO_REDIRECT,
    REDIRECT_INPUT,
    REDIRECT_OUTPUT,
    REDIRECT_APPEND
};

int handle_input_redirect(char* input_file);
int handle_output_redirect(char* output_file);
int handle_append_redirect(char* output_file);

#endif