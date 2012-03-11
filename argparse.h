#ifndef ARGPARSE_H
#define ARGPARSE_H
/**
 * Command-line arguments parsing library.
 *
 * This module is inspired by parse-options.c (git) and python's argparse
 * module.
 *
 * The program defines what arguments it requires, and `argparse` will figure out
 * how to parse those out of `argc` and `argv`, it also automatically generates
 * help and usage messages and issues errors when users give the program invalid
 * arguments:
 *
 * Features:
 *  - handles both optional and positional arguments
 *  - produces highly informative usage messages
 *  - issures errors when given invalid arguments
 *
 * @author Yecheng Fu <cofyc.jackson@gmail.com>
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum argparse_opt_type {
    /* special */
    OPTION_END,
    /* options with no arguments */
    OPTION_BOOLEAN,
    /* options with arguments */
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
 *  `defval`:
 *    default value to fill.
 */
struct argparse_option {
    enum argparse_opt_type type;
    char short_name;
    const char *long_name;
    void *value;
    const char *help;
    void *defval;
};

#define OPT_END()                   { OPTION_END }
#define OPT_BOOLEAN(s, l, v, h)     { OPTION_BOOLEAN, (s), (l), (v), (h) }
#define OPT_INTEGER(s, l, v, h)     { OPTION_INTEGER, (s), (l), (v), (h) }
#define OPT_STRING(s, l, v, h)      { OPTION_STRING, (s), (l), (v), (h) }

extern int argparse(int argc, const char **argv, const struct argparse_option
        *options, const char *const *usagestr);
extern void argparse_usage(const char *const *usagestr, const struct 
        argparse_option *options);

#endif
