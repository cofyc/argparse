#include <stdio.h>
#include <stdlib.h>
#include "../argparse.h"

static const char *const usages[] = {
    "test_null_help [options]",
    NULL,
};

int main(int argc, const char **argv)
{
    int force = 0;
    int verbose = 0;
    const char *file = NULL;
    
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Options with NULL help"),
        OPT_BOOLEAN('f', "force", &force, NULL, NULL, 0, 0),
        OPT_BOOLEAN('v', "verbose", &verbose, "enable verbose output", NULL, 0, 0),
        OPT_STRING('F', "file", &file, NULL, NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nTest program for options with NULL help text.", NULL);
    argc = argparse_parse(&argparse, argc, argv);
    
    printf("force: %d\n", force);
    printf("verbose: %d\n", verbose);
    printf("file: %s\n", file ? file : "(null)");
    
    return 0;
}