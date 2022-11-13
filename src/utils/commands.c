#include <string.h>
#include <stdio.h>
#include "commands.h"

void __help(char** argv, Command* cmds, int length) {
    printf("usage: %s [options] <code file> ...\n", argv[0]);
    printf("  --%s\t%s\n", "help", "shows this and exit");
    for (int i = 0; i < length; i++) {
        printf("  --%s\t%s\n", cmds[i].name, cmds[i].usage);
    }
}

bool __cmd_fallback(int argc, char** argv, int* index) {
    printf("unknown command line argument '%s', try '%s --help'\n", argv[*index], argv[0]);
    return true;
}

bool pass_arguments(int argc, char** argv, Command* cmds, int length) {
    for (int i = 1; i < argc; i++) {
        bool isCommand = false;
        for (int k = 0; k < length; k++) {
            if (strcmp(argv[i], "--help") == 0) {
                __help(argv, cmds, length);
                return true;
            }
            if (strncmp("--", argv[i], 2) == 0 && strcmp(cmds[k].name, argv[i] + 2) == 0) {
                isCommand = true;
                if (cmds[k].func(argc, argv, &i)) {
                    return true;
                }
            }
        }
        if (!isCommand &&  i != argc - 1)
            return __cmd_fallback(argc, argv, &i);
    }
    return false;
} 