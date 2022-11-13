#ifndef COMMANDS_H
#define COMMANDS_H

#define CMD_LENGTH 256

#include <stdbool.h>

/**
 * @brief returns true if the program has to stop after the argument is passed
 * 
 */
typedef bool(*cmd_func_ptr)(int argc, char** argv, int* currentIndex);

typedef struct Command {
    char name[CMD_LENGTH];
    cmd_func_ptr func;
    char usage[CMD_LENGTH];
} Command;

/**
 * @return bool if the program has to stop or can continue
 */
bool pass_arguments(int argc, char** argv, Command* cmds, int length);

void __help(char** argv, Command* cmds, int length);
bool __cmd_fallback(int argc, char** argv, int* index);

#endif