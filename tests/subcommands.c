#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argparse.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

static const char *const usages[] = {
    "subcommands [options] [cmd] [args]",
    NULL,
};

struct cmd_struct {
    const char *cmd;
    int (*fn) (int, const char **);
};

int
cmd_foo(int argc, const char **argv)
{
    printf("executing subcommand foo\n");
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, *(argv + i));
    }
    int force = 0;
    int test = 0;
    const char *path = NULL;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_BOOLEAN('f', "force", &force, "force to do", NULL, 0, 0),
        OPT_BOOLEAN('t', "test", &test, "test only", NULL, 0, 0),
        OPT_STRING('p', "path", &path, "path to read", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argc = argparse_parse(&argparse, argc, argv);
    printf("after argparse_parse:\n");
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, *(argv + i));
    }
    return 0;
}

int
cmd_bar(int argc, const char **argv)
{
    printf("executing subcommand bar\n");
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, *(argv + i));
    }
    return 0;
}

static struct cmd_struct commands[] = {
    {"foo", cmd_foo},
    {"bar", cmd_bar},
};

int
main(int argc, const char **argv)
{
    struct argparse argparse;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_END(),
    };
    argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
    argc = argparse_parse(&argparse, argc, argv);
    if (argc < 1) {
        argparse_usage(&argparse);
        return -1;
    }

    /* Try to run command with args provided. */
    struct cmd_struct *cmd = NULL;
    for (int i = 0; i < ARRAY_SIZE(commands); i++) {
        if (!strcmp(commands[i].cmd, argv[0])) {
            cmd = &commands[i];
        }
    }
    if (cmd) {
        return cmd->fn(argc, argv);
    }
    return 0;
}
