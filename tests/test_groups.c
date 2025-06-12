#include <stdio.h>
#include <stdlib.h>
#include "../argparse.h"

static const char *const usages[] = {
    "test_groups [options]",
    NULL,
};

int main(int argc, const char **argv)
{
    int verbose = 0;
    int debug = 0;
    int force = 0;
    int quiet = 0;
    const char *input = NULL;
    const char *output = NULL;
    const char *format = NULL;
    int threads = 1;
    
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("General options"),
        OPT_BOOLEAN('v', "verbose", &verbose, "enable verbose output", NULL, 0, 0),
        OPT_BOOLEAN('d', "debug", &debug, "enable debug mode", NULL, 0, 0),
        OPT_BOOLEAN('q', "quiet", &quiet, "suppress normal output", NULL, 0, 0),
        OPT_GROUP("Input/Output options"),
        OPT_STRING('i', "input", &input, "input file path", NULL, 0, 0),
        OPT_STRING('o', "output", &output, "output file path", NULL, 0, 0),
        OPT_STRING('f', "format", &format, "output format (json, xml, csv)", NULL, 0, 0),
        OPT_GROUP("Processing options"),
        OPT_INTEGER('t', "threads", &threads, "number of threads", NULL, 0, 0),
        OPT_BOOLEAN('F', "force", &force, "force overwrite", NULL, 0, 0),
        OPT_GROUP(""), // Empty group name
        OPT_BOOLEAN(0, "hidden", NULL, "hidden option", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nTest program for option groups display.",
                      "\nThis program demonstrates how option groups are displayed in help.");
    
    // Just trigger help to see group display
    if (argc == 1) {
        argparse_usage(&argparse);
        return 0;
    }
    
    argc = argparse_parse(&argparse, argc, argv);
    
    printf("Options parsed successfully\n");
    
    return 0;
}