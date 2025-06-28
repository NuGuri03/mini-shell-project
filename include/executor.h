#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

int handle_internal_command(Command cmd);
int execute_commands(Command* cmds, int num_cmds);

#endif