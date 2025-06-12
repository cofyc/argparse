#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../argparse.h"

static int callback_count = 0;
static char callback_results[256] = "";

static int
simple_callback(struct argparse *self, const struct argparse_option *option)
{
    (void)self;
    callback_count++;
    strcat(callback_results, option->long_name);
    strcat(callback_results, ",");
    return 0;
}

static int
value_callback(struct argparse *self, const struct argparse_option *option)
{
    (void)self;
    callback_count++;
    if (option->value && *(const char **)option->value) {
        strcat(callback_results, *(const char **)option->value);
        strcat(callback_results, ":");
    }
    strcat(callback_results, option->long_name);
    strcat(callback_results, ",");
    return 0;
}

static int
error_callback(struct argparse *self, const struct argparse_option *option)
{
    (void)self;
    (void)option;
    return -1; // Simulate error
}

static const char *const usages[] = {
    "test_callbacks [options]",
    NULL,
};

int main(int argc, const char **argv)
{
    int force = 0;
    const char *path = NULL;
    const char *name = NULL;
    
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_BOOLEAN('f', "force", &force, "force mode", &simple_callback, 0, 0),
        OPT_STRING('p', "path", &path, "path value", &value_callback, 0, 0),
        OPT_STRING('n', "name", &name, "name value", &value_callback, 0, 0),
        OPT_BOOLEAN('e', "error", NULL, "trigger error", &error_callback, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nTest program for callback functionality.", NULL);
    
    // Parse with potential error callback
    argc = argparse_parse(&argparse, argc, argv);
    
    printf("callback_count: %d\n", callback_count);
    printf("callback_results: %s\n", callback_results);
    printf("force: %d\n", force);
    printf("path: %s\n", path ? path : "(null)");
    printf("name: %s\n", name ? name : "(null)");
    printf("remaining_args: %d\n", argc);
    
    return 0;
}