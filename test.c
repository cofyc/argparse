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
        OPT_BOOLEAN('f', "force", &force, "force to do"),
        OPT_STRING('p', "path", &path, "path to read"),
        OPT_INTEGER('n', "num", &num, "selected num"),
        OPT_END(),
    };
    argc = argparse(argc, argv, options, usage);
    printf("force: %d\n", force);
    printf("path: %s\n", path);
    printf("num: %d\n", num);
    printf("argc: %d\n", argc);
    int i;
    for (i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, *(argv + i));
    }
    return 0;
}
