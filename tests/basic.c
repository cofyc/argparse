#include "argparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* const usages[] = {
    "basic [options] [[--] args]",
    "basic [options]",
    NULL,
};

#define PERM_READ (1 << 0)
#define PERM_WRITE (1 << 1)
#define PERM_EXEC (1 << 2)

int main(int argc, const char** argv)
{
    int force = 0;
    int test = 0;
    int int_num = 0;
    float flt_num = 0.f;
    const char* path = NULL;
    int perms = 0;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_BOOLEAN('f', "force", &force, "force to do"),
        OPT_BOOLEAN('t', "test", &test, "test only"),
        OPT_STRING('p', "path", &path, "path to read"),
        OPT_INTEGER('i', "int", &int_num, "selected integer"),
        OPT_FLOAT('s', "float", &flt_num, "selected float"),
        OPT_GROUP("Bits options"),
        OPT_BIT(0, "read", &perms, "read perm", NULL, PERM_READ, OPT_NONEG),
        OPT_BIT(0, "write", &perms, "write perm", NULL, PERM_WRITE, 0),
        OPT_BIT(0, "exec", &perms, "exec perm", NULL, PERM_EXEC, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, NULL, usages);
    argparse_describe(
        &argparse,
        "\nA brief description of what the program does and how it works.",
        "\nAdditional description of the program after the description of the arguments.");
    // printing in json form for testing
    if (!argparse_parse(&argparse, argc, argv)) {
        printf("{\n");
        printf("\t\"force\" : %d,\n", force);
        printf("\t\"test\" : %d,\n", test);
        printf("\t\"int\" : %d,\n", int_num);
        printf("\t\"float\" : %g,\n", flt_num);
    }
    if (path != NULL)
        printf("\t\"path\" : \"%s\",\n", path);
    if (argc != 0) {
        printf("\t\"argc\" : %d,\n", argc);
        int i;
        for (i = 0; i < argc; i++) {
            printf("\t\"argv[%d]\" : \"%s\"", i, *(argv + i));
            if (argc != i + 1) {
                printf(",\n");
            } else {
                printf("\n");
            }
        }
    }
    if (perms) {
        printf(",\t\"perms\" : %d\n", perms);
    }
    printf("}\n");
    return 0;
}
