#include <stdio.h>
#include <string.h>
#include <time.h>

#include "utils/commands.h"
#include "vm.h"

#define COMMAND(_name, _usage) {.name = #_name, .usage = _usage, .func = __##_name}

struct VM vm;
int stack_size = 64;
int heap_size = 8192;

void __set_used_callback() {
    vm_realloc(&vm);
}

bool __gcstats() {
    return !(heap.debug = true);
}

bool __gcpurge() {
    return !(heap.purge = true);
}

bool __version(int argc, char** argv, int *index) {
    time_t currentTime = time(NULL);
    struct tm* timeInfo = localtime(&currentTime);
    char timeInfoString[64];
    strftime(timeInfoString, sizeof(timeInfoString), "%b %m %Y, %H:%M:%S", timeInfo);
    printf("Ninja Virtual Machine version %u (compiled %s)\n", VERSION, timeInfoString);
    return true;
}

bool __stack(int argc, char** argv, int *index) {
    *index += 1;
    if (*index >= argc) {
        printf("%s\n", "Usage: --stack n - to set the stack size in KiB");
        return true;
    }
    stack_size = atoi(argv[*index]);
    return false;
}

bool __heap(int argc, char** argv, int *index) {
    *index += 1;
    if (*index >= argc) {
        printf("%s\n", "Usage: --heap n - to set the heap size in KiB");
        return true;
    }
    heap_size = atoi(argv[*index]);
    return false;
}

Command cmds[] = {
    COMMAND(version, "show version and exit"),
    COMMAND(stack, "set stack size to n KBytes (default: n = 64)"),
    COMMAND(heap, "set heap size to n KBytes (default: n = 8192)"),
    COMMAND(gcstats, "show garbage collection statistics"),
    COMMAND(gcpurge, "purge old objects after collection"),
};    

int main(int argc, char* argv[]) {
    if (pass_arguments(argc, argv, cmds, sizeof(cmds)/sizeof(Command)))
        return EXIT_FAILURE;

    if (argc < 2) {
        printf("Error no code file specified\n");
        return EXIT_FAILURE;
    }

    // filepath is the last argument
    char* program = argv[argc - 1];

    vm_init(&vm, stack_size);

    int result;
    if ((result = vm_load_binary(&vm, program)) != 0) {
        vm_delete(&vm);
        return result;
    }

    heap_init(__set_used_callback, heap_size);
    vm_run(&vm);
    heap_delete();

    vm_delete(&vm);
    return EXIT_SUCCESS;
}