#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

int handle_internal_command(struct command cmd);

int execute_commands(struct command* cmds, int num_cmds);

#endif