#include "argparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

int inspect(int argc, const char** argv)
{
    int force = 0;
    int test = 0;
    const char* path = NULL;
    static const char* const usages[] = {
        "inspect [options] [cmd] [args]",
        NULL,
    };
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_BOOLEAN('f', "force", &force, "force to do", NULL, 0, 0),
        OPT_BOOLEAN('t', "test", &test, "test only", NULL, 0, 0),
        OPT_STRING('p', "path", &path, "path to read", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, NULL, usages);
    if (!argparse_parse(&argparse, argc, argv)) {
        if (force != 0)
            printf("force: %d\n", force);
        if (test != 0)
            printf("test: %d\n", test);
        if (path != NULL)
            printf("path: %s\n", path);
    }
    return 0;
}

int print_argv(int argc, const char** argv)
{
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, *(argv + i));
    }
    return 0;
}

int main(int argc, const char** argv)
{
    static const char* const usages[] = {
        "subcommands [options] [cmd] [args]",
        NULL,
    };
    struct argparse argparse;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_END(),
    };
    struct argparse_command commands[] = {
        CMD(inspect, "inspects the system"),
        CMD(print_argv, "prints argv"),
        CMD_END(),
    };
    argparse_init(&argparse, options, commands, usages);
    return argparse_parse(&argparse, argc, argv);
}
