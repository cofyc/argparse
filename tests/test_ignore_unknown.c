#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../argparse.h"

static const char *const usages[] = {
    "test_ignore_unknown [options] [args...]",
    NULL,
};

int main(int argc, const char **argv)
{
    int verbose = 0;
    const char *file = NULL;
    
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_BOOLEAN('v', "verbose", &verbose, "enable verbose output", NULL, 0, 0),
        OPT_STRING('f', "file", &file, "input file", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, ARGPARSE_IGNORE_UNKNOWN_ARGS);
    argparse_describe(&argparse, "\nTest program for ARGPARSE_IGNORE_UNKNOWN_ARGS flag.", NULL);
    
    argc = argparse_parse(&argparse, argc, argv);
    
    printf("verbose: %d\n", verbose);
    printf("file: %s\n", file ? file : "(null)");
    printf("remaining_argc: %d\n", argc);
    
    // Print remaining arguments
    if (argc > 0) {
        printf("remaining_args:");
        for (int i = 0; i < argc; i++) {
            printf(" %s", argv[i]);
        }
        printf("\n");
    }
    
    return 0;
}