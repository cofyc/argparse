#include "argparse.h"

static const char * const usage[] = {
    "test-argparse [options]",
    NULL,
};

int
main(int argc, const char **argv)
{
    int force = 0;
    int num = 0;
    const char *path = NULL;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_BOOLEAN('f', "force", &force, "force to do", NULL),
        OPT_STRING('p', "path", &path, "path to read", NULL),
        OPT_INTEGER('n', "num", &num, "selected num", NULL),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, usage);
    argc = argparse_parse(&argparse, argc, argv);
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
