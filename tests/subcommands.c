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
    int len;
    int (*fn) (int, const char **);
    const char *help;
};

int
cmd_foo(int argc, const char **argv)
{
    printf("executing subcommand foo\n");
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
    {"foo", 3, cmd_foo, NULL},
    {"bar", 3, cmd_bar, NULL},
};

int
main(int argc, const char **argv)
{
    struct argparse argparse;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_END(),
    };
    argparse_init(&argparse, options, usages, 0);
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
