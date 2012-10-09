#ifndef ARGPARSE_H
#define ARGPARSE_H
/**
 * Command-line arguments parsing library.
 *
 * This module is inspired by parse-options.c (git) and python's argparse
 * module.
 *
 * Arguments parsing is common task in cli program, but traditional `getopt`
 * libraries are not easy to use. This library provides high-level arguments
 * parsing solutions.
 *
 * The program defines what arguments it requires, and `argparse` will figure
 * out how to parse those out of `argc` and `argv`, it also automatically
 * generates help and usage messages and issues errors when users give the
 * program invalid arguments.
 *
 * Features:
 *  - handles both optional and positional arguments
 *  - produces highly informative usage messages
 *  - issures errors when given invalid arguments
 *
 * Author: Yecheng Fu <cofyc.jackson@gmail.com>
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct argparse;
struct argparse_option;

typedef int argparse_callback(struct argparse *this,
                              const struct argparse_option *option);

enum argparse_flag {
    ARGPARSE_STOP_AT_NON_OPTION = 1,
};

enum argparse_option_type {
    /* special */
    OPTION_END,
    /* options with no arguments */
    OPTION_BOOLEAN,
    OPTION_SETBIT,
    /* options with arguments (optional or required) */
    OPTION_INTEGER,
    OPTION_STRING,
};

/*
 *  `type`:
 *    holds the type of the option, you must have an OPTION_END last in your
 *    array.
 *
 *  `short_name`:
 *    the character to use as a short option name, '\0' if none.
 *
 *  `long_name`:
 *    the long option name, without the leading dash, NULL if none.
 *
 *  `value`:
 *    stores pointer to the value to be filled.
 *
 *  `help`:
 *    the short help message associated to what the option does.
 *    Must never be NULL (except for OPTION_END).
 *
 *  `callback`:
 *    function is called when corresponding argument is parsed.
 *
 *  `data`:
 *    associated data. Callbacks can use it like they want.
 */
struct argparse_option {
    enum argparse_option_type type;
    const char short_name;
    const char *long_name;
    void *value;
    const char *help;
    argparse_callback *callback;
    intptr_t data;
};

/*
 * argpparse
 */
struct argparse {
    // user supplied
    const struct argparse_option *options;
    const char *const *usage;
    int flags;
    // internal context
    int argc;
    const char **argv;
    const char **out;
    int cpidx;
    const char *optvalue;       // current option value
};

// builtin callbacks
int argparse_help_cb(struct argparse *this,
                     const struct argparse_option *option);

// helper macros
#define OPT_END()                       { OPTION_END }
#define OPT_BOOLEAN(s, l, v, h, c)      { OPTION_BOOLEAN, (s), (l), (v), (h), (c), 0 }
#define OPT_SETBIT(s, l, v, h, c, d)    { OPTION_SETBIT, (s), (l), (v), (h), (c), (d) }
#define OPT_INTEGER(s, l, v, h, c)      { OPTION_INTEGER, (s), (l), (v), (h), (c), 0 }
#define OPT_STRING(s, l, v, h, c)       { OPTION_STRING, (s), (l), (v), (h), (c), 0 }
#define OPT_HELP()                      OPT_BOOLEAN('h', "help", NULL, "show this help message and exit", argparse_help_cb)

int argparse_init(struct argparse *this, struct argparse_option *options,
                  const char *const *usage, int flags);
int argparse_parse(struct argparse *this, int argc, const char **argv);
void argparse_usage(struct argparse *this);

#endif
