#include <stdio.h>
#include <stdlib.h>
#include "../argparse.h"

static const char *const usages[] = {
    "test_stress [options]",
    NULL,
};

int main(int argc, const char **argv)
{
    // Create arrays for 50 different options
    int flags[50] = {0};
    const char *strings[10] = {NULL};
    int integers[10] = {0};
    float floats[10] = {0.0f};
    
    // Large options array with many different types
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Boolean flags (40 total)"),
        OPT_BOOLEAN('a', "flag-a", &flags[0], "flag a description", NULL, 0, 0),
        OPT_BOOLEAN('b', "flag-b", &flags[1], "flag b description", NULL, 0, 0),
        OPT_BOOLEAN('c', "flag-c", &flags[2], "flag c description", NULL, 0, 0),
        OPT_BOOLEAN('d', "flag-d", &flags[3], "flag d description", NULL, 0, 0),
        OPT_BOOLEAN('e', "flag-e", &flags[4], "flag e description", NULL, 0, 0),
        OPT_BOOLEAN('f', "flag-f", &flags[5], "flag f description", NULL, 0, 0),
        OPT_BOOLEAN('g', "flag-g", &flags[6], "flag g description", NULL, 0, 0),
        OPT_BOOLEAN(0, "flag-h", &flags[7], "flag h description", NULL, 0, 0),
        OPT_BOOLEAN('i', "flag-i", &flags[8], "flag i description", NULL, 0, 0),
        OPT_BOOLEAN('j', "flag-j", &flags[9], "flag j description", NULL, 0, 0),
        OPT_BOOLEAN('k', "flag-k", &flags[10], "flag k description", NULL, 0, 0),
        OPT_BOOLEAN('l', "flag-l", &flags[11], "flag l description", NULL, 0, 0),
        OPT_BOOLEAN('m', "flag-m", &flags[12], "flag m description", NULL, 0, 0),
        OPT_BOOLEAN('n', "flag-n", &flags[13], "flag n description", NULL, 0, 0),
        OPT_BOOLEAN('o', "flag-o", &flags[14], "flag o description", NULL, 0, 0),
        OPT_BOOLEAN('p', "flag-p", &flags[15], "flag p description", NULL, 0, 0),
        OPT_BOOLEAN('q', "flag-q", &flags[16], "flag q description", NULL, 0, 0),
        OPT_BOOLEAN('r', "flag-r", &flags[17], "flag r description", NULL, 0, 0),
        OPT_BOOLEAN('s', "flag-s", &flags[18], "flag s description", NULL, 0, 0),
        OPT_BOOLEAN('t', "flag-t", &flags[19], "flag t description", NULL, 0, 0),
        OPT_BOOLEAN('u', "flag-u", &flags[20], "flag u description", NULL, 0, 0),
        OPT_BOOLEAN('v', "flag-v", &flags[21], "flag v description", NULL, 0, 0),
        OPT_BOOLEAN('w', "flag-w", &flags[22], "flag w description", NULL, 0, 0),
        OPT_BOOLEAN('x', "flag-x", &flags[23], "flag x description", NULL, 0, 0),
        OPT_BOOLEAN('y', "flag-y", &flags[24], "flag y description", NULL, 0, 0),
        OPT_BOOLEAN('z', "flag-z", &flags[25], "flag z description", NULL, 0, 0),
        OPT_BOOLEAN('A', "flag-A", &flags[26], "flag A description", NULL, 0, 0),
        OPT_BOOLEAN('B', "flag-B", &flags[27], "flag B description", NULL, 0, 0),
        OPT_BOOLEAN('C', "flag-C", &flags[28], "flag C description", NULL, 0, 0),
        OPT_BOOLEAN('D', "flag-D", &flags[29], "flag D description", NULL, 0, 0),
        OPT_BOOLEAN('E', "flag-E", &flags[30], "flag E description", NULL, 0, 0),
        OPT_BOOLEAN('F', "flag-F", &flags[31], "flag F description", NULL, 0, 0),
        OPT_BOOLEAN('G', "flag-G", &flags[32], "flag G description", NULL, 0, 0),
        OPT_BOOLEAN('H', "flag-H", &flags[33], "flag H description", NULL, 0, 0),
        OPT_BOOLEAN('I', "flag-I", &flags[34], "flag I description", NULL, 0, 0),
        OPT_BOOLEAN('J', "flag-J", &flags[35], "flag J description", NULL, 0, 0),
        OPT_BOOLEAN('K', "flag-K", &flags[36], "flag K description", NULL, 0, 0),
        OPT_BOOLEAN('L', "flag-L", &flags[37], "flag L description", NULL, 0, 0),
        OPT_BOOLEAN('M', "flag-M", &flags[38], "flag M description", NULL, 0, 0),
        OPT_BOOLEAN('N', "flag-N", &flags[39], "flag N description", NULL, 0, 0),
        OPT_GROUP("String options (10 total)"),
        OPT_STRING(0, "str0", &strings[0], "string option 0", NULL, 0, 0),
        OPT_STRING(0, "str1", &strings[1], "string option 1", NULL, 0, 0),
        OPT_STRING(0, "str2", &strings[2], "string option 2", NULL, 0, 0),
        OPT_STRING(0, "str3", &strings[3], "string option 3", NULL, 0, 0),
        OPT_STRING(0, "str4", &strings[4], "string option 4", NULL, 0, 0),
        OPT_STRING(0, "str5", &strings[5], "string option 5", NULL, 0, 0),
        OPT_STRING(0, "str6", &strings[6], "string option 6", NULL, 0, 0),
        OPT_STRING(0, "str7", &strings[7], "string option 7", NULL, 0, 0),
        OPT_STRING(0, "str8", &strings[8], "string option 8", NULL, 0, 0),
        OPT_STRING(0, "str9", &strings[9], "string option 9", NULL, 0, 0),
        OPT_GROUP("Integer options (10 total)"),
        OPT_INTEGER(0, "int0", &integers[0], "integer option 0", NULL, 0, 0),
        OPT_INTEGER(0, "int1", &integers[1], "integer option 1", NULL, 0, 0),
        OPT_INTEGER(0, "int2", &integers[2], "integer option 2", NULL, 0, 0),
        OPT_INTEGER(0, "int3", &integers[3], "integer option 3", NULL, 0, 0),
        OPT_INTEGER(0, "int4", &integers[4], "integer option 4", NULL, 0, 0),
        OPT_INTEGER(0, "int5", &integers[5], "integer option 5", NULL, 0, 0),
        OPT_INTEGER(0, "int6", &integers[6], "integer option 6", NULL, 0, 0),
        OPT_INTEGER(0, "int7", &integers[7], "integer option 7", NULL, 0, 0),
        OPT_INTEGER(0, "int8", &integers[8], "integer option 8", NULL, 0, 0),
        OPT_INTEGER(0, "int9", &integers[9], "integer option 9", NULL, 0, 0),
        OPT_GROUP("Float options (10 total)"),
        OPT_FLOAT(0, "float0", &floats[0], "float option 0", NULL, 0, 0),
        OPT_FLOAT(0, "float1", &floats[1], "float option 1", NULL, 0, 0),
        OPT_FLOAT(0, "float2", &floats[2], "float option 2", NULL, 0, 0),
        OPT_FLOAT(0, "float3", &floats[3], "float option 3", NULL, 0, 0),
        OPT_FLOAT(0, "float4", &floats[4], "float option 4", NULL, 0, 0),
        OPT_FLOAT(0, "float5", &floats[5], "float option 5", NULL, 0, 0),
        OPT_FLOAT(0, "float6", &floats[6], "float option 6", NULL, 0, 0),
        OPT_FLOAT(0, "float7", &floats[7], "float option 7", NULL, 0, 0),
        OPT_FLOAT(0, "float8", &floats[8], "float option 8", NULL, 0, 0),
        OPT_FLOAT(0, "float9", &floats[9], "float option 9", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nStress test with 71 total options.", NULL);
    
    argc = argparse_parse(&argparse, argc, argv);
    
    // Count active flags
    int flag_count = 0;
    for (int i = 0; i < 40; i++) {
        if (flags[i]) flag_count++;
    }
    
    printf("stress_test_completed: yes\n");
    printf("active_flags: %d\n", flag_count);
    printf("remaining_args: %d\n", argc);
    
    return 0;
}