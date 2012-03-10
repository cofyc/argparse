#include "argparse.h"

#define USAGE_OPTS_WIDTH 24

/*  
 * argparse context
 */
struct argparse_ctx {
    int argc;
    const char **argv;
    const char **out;
    int outidx;
    const char *arg;            // current argument
    const char *optvalue;       // current option value
};

static const char *
skip_prefix(const char *str, const char *prefix)
{
    size_t len = strlen(prefix);
    return strncmp(str, prefix, len) ? NULL : str + len;
}

static int
argparse_error(struct argparse_ctx *ctx, const struct argparse_option *opt, const char *reason)
{
    if (!strncmp(ctx->arg, "--", 2)) {
        return fprintf(stderr, "error: option `%s` %s\n", opt->long_name, reason);
    } else {
        return fprintf(stderr, "error: option `%c` %s\n", opt->short_name, reason);
    }
}

static int
argparse_getvalue(struct argparse_ctx *ctx, const struct argparse_option *opt)
{
    const char *s;
    switch (opt->type) {
    case OPTION_BOOLEAN:
        *(int *)opt->value = *(int *)opt->value + 1;
        return 0;
    case OPTION_STRING:
        if (ctx->optvalue) {
            *(const char **)opt->value = ctx->optvalue;
        } else if (ctx->argc > 1) {
            ctx->argc--;
            *(const char **)opt->value = *++ctx->argv;
        } else {
            return argparse_error(ctx, opt, "requires a value");
        }
        return 0;
    case OPTION_INTEGER:
        if (ctx->optvalue) {
            *(int *)opt->value = strtol(ctx->optvalue, (char **)&s, 0);
        } else if (ctx->argc > 1) {
            ctx->argc--;
            *(int *)opt->value = strtol(*++ctx->argv, (char **)&s, 0);
        } else {
            return argparse_error(ctx, opt, "requires a value");
        }
        if (*s)
            return argparse_error(ctx, opt, "expects a numerical value");
        return 0;
    default:
        assert(0);
    }
}

static void
argparse_options_check(const struct argparse_option *options)
{
    for (; options->type != OPTION_END; options++) {
        switch (options->type) {
        case OPTION_END:
        case OPTION_BOOLEAN:
        case OPTION_INTEGER:
        case OPTION_STRING:
            continue;
        default:
            fprintf(stderr, "wrong option type: %d", options->type);
            break;
        }
    }
}

static int
argparse_short_opt(struct argparse_ctx *ctx, const struct argparse_option *options)
{
    for (; options->type != OPTION_END; options++) {
        if (options->short_name == *(ctx->argv[0] + 1)) {
            ctx->arg = ctx->argv[0];
            return argparse_getvalue(ctx, options);
        }
    }
    return -2;
}

static int
argparse_long_opt(struct argparse_ctx *ctx, const struct argparse_option *options)
{
    for (; options->type != OPTION_END; options++) {
        const char *rest;
        if (!options->long_name)
            continue;

        rest = skip_prefix(ctx->argv[0] + 2, options->long_name);
        if (!rest) {
            continue;
        }
        if (*rest) {
            if (*rest != '=')
                continue;
            ctx->optvalue = rest + 1;
        }
        ctx->arg = ctx->argv[0];
        return argparse_getvalue(ctx, options);
    }
    return -2;
}

int
argparse(int argc, const char **argv, const struct argparse_option *options, const char *const *usagestr)
{
    struct argparse_ctx ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.argc = argc - 1;
    ctx.argv = argv + 1;
    ctx.out = argv;

    argparse_options_check(options);

    for (; ctx.argc; ctx.argc--, ctx.argv++) {
        const char *arg = ctx.argv[0];
        if (*arg != '-' || !arg[1]) {
            ctx.out[ctx.outidx++] = ctx.argv[0];
            continue;
        }
        // short option
        if (arg[1] != '-') {
            switch (argparse_short_opt(&ctx, options)) {
            case -1:
                break;
            case -2:
                goto unknown;
            }
            continue;
        }
        // if '--' presents
        if (!arg[2]) {
            break;
        }
        // long option
        switch (argparse_long_opt(&ctx, options)) {
        case -1:
            break;
        case -2:
            goto unknown;
        }
        continue;

unknown:
        fprintf(stderr, "error: unknown option `%s`\n", ctx.argv[0]);
        argparse_usage(usagestr, options);
        continue;
    }

    memmove(ctx.out + ctx.outidx, ctx.argv, ctx.argc * sizeof(*ctx.out));
    ctx.out[ctx.outidx + ctx.argc] = NULL;

    return ctx.outidx + ctx.argc;
}

void
argparse_usage(const char *const *usagestr, const struct argparse_option *options)
{
    fprintf(stdout, "Usage: %s\n", *usagestr++);
    while (*usagestr && **usagestr)
        fprintf(stdout, "    or: %s\n", *usagestr++);
    fputc('\n', stdout);
    for (; options->type != OPTION_END; options++) {
        size_t pos;
        int pad;
        pos = fprintf(stdout, "    ");
        if (options->short_name) {
            pos += fprintf(stdout, "-%c", options->short_name);
        }
        if (options->long_name && options->short_name) {
            pos += fprintf(stdout, ", ");
        }
        if (options->long_name) {
            pos += fprintf(stdout, "--%s", options->long_name);
        }
        if (pos <= USAGE_OPTS_WIDTH) {
            pad = USAGE_OPTS_WIDTH - pos;
        } else {
            fputc('\n', stdout);
            pad = USAGE_OPTS_WIDTH;
        }
        fprintf(stdout, "%*s%s\n", pad + 2, "", options->help);
    }
    exit(129);
}
