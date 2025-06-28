#ifndef RAW_IO_H
#define RAW_IO_H

#include <termios.h>

#include "history.h"

struct termios enable_raw_mode();


void disable_raw_mode(struct termios *orig_termios);

/**
 * @brief Reads a line of input from the user.
 * @return A pointer to a string containing the input line, or NULL on EOF or error.
 */
char *read_input(struct history *history_list);

#endif