#include "argparse.h"

static const char * const usage[] = {
    "test-argparse [options]",
    NULL,
};

int
argparse_help(struct argparser *this, struct argparse_option *option)
{
    argparse_usage(this);
    return 0;
}

int
main(int argc, const char **argv)
{
    int force = 0;
    int num = 0;
    const char *path = NULL;
    struct argparse_option options[] = {
        OPT_BOOLEAN('f', "force", &force, "force to do", NULL),
        OPT_BOOLEAN('h', "help", NULL, "show this help message and exit", argparse_help),
        OPT_STRING('p', "path", &path, "path to read", NULL),
        OPT_INTEGER('n', "num", &num, "selected num", NULL),
        OPT_END(),
    };
    struct argparser argparser;
    argparse_init(&argparser, options, usage);
    argc = argparse_parser(&argparser, argc, argv);
    if (force != 0)
        printf("force: %d\n", force);
    if (path != NULL)
        printf("path: %s\n", path);
    if (num != 0)
        printf("num: %d\n", num);
    if (argc != 0) {
        printf("argc: %d\n", argc);
        int i;
        for (i = 0; i < argc; i++) {
            printf("argv[%d]: %s\n", i, *(argv + i));
        }
    }
    return 0;
}
