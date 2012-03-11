#include "argparse.h"

#define USAGE_OPTS_WIDTH 24

static const char *
skip_prefix(const char *str, const char *prefix)
{
    size_t len = strlen(prefix);
    return strncmp(str, prefix, len) ? NULL : str + len;
}

static int
argparse_error(struct argparse *this, const struct argparse_option *opt, const char *reason)
{
    if (!strncmp(this->arg, "--", 2)) {
        return fprintf(stderr, "error: option `%s` %s\n", opt->long_name, reason);
    } else {
        return fprintf(stderr, "error: option `%c` %s\n", opt->short_name, reason);
    }
}

static int
argparse_getvalue(struct argparse *this, const struct argparse_option *opt)
{
    const char *s;
    if (!opt->value) goto no_value;
    switch (opt->type) {
    case OPTION_BOOLEAN:
        *(int *)opt->value = *(int *)opt->value + 1;
        break;
    case OPTION_STRING:
        if (this->optvalue) {
            *(const char **)opt->value = this->optvalue;
        } else if (this->argc > 1) {
            this->argc--;
            *(const char **)opt->value = *++this->argv;
        } else {
            return argparse_error(this, opt, "requires a value");
        }
        break;
    case OPTION_INTEGER:
        if (strlen(this->arg) > 2) {
            *(int *)opt->value = strtol(this->arg + 2, (char **)&s, 0);
        } else if (this->argc > 1) {
            this->argc--;
            *(int *)opt->value = strtol(*++this->argv, (char **)&s, 0);
        } else {
            return argparse_error(this, opt, "requires a value");
        }
        if (*s)
            return argparse_error(this, opt, "expects a numerical value");
        break;
    default:
        assert(0);
    }

no_value:
    if (opt->callback) {
        return opt->callback(this, (struct argparse_option *)opt);
    }
    return 0;
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
argparse_short_opt(struct argparse *this, const struct argparse_option *options)
{
    for (; options->type != OPTION_END; options++) {
        if (options->short_name == *(this->argv[0] + 1)) {
            this->arg = this->argv[0];
            return argparse_getvalue(this, options);
        }
    }
    return -2;
}

static int
argparse_long_opt(struct argparse *this, const struct argparse_option *options)
{
    for (; options->type != OPTION_END; options++) {
        const char *rest;
        if (!options->long_name)
            continue;

        rest = skip_prefix(this->argv[0] + 2, options->long_name);
        if (!rest) {
            continue;
        }
        if (*rest) {
            if (*rest != '=')
                continue;
            this->optvalue = rest + 1;
        }
        this->arg = this->argv[0];
        return argparse_getvalue(this, options);
    }
    return -2;
}

int
argparse_init(struct argparse *this, struct argparse_option *options, const char *const *usage)
{
    memset(this, 0, sizeof(this));
    this->options = options;
    this->usage = usage;
    return 0;
}

int
argparse_parse(struct argparse *this, int argc, const char **argv)
{
    this->argc = argc - 1;
    this->argv = argv + 1;
    this->out = argv;

    argparse_options_check(this->options);

    for (; this->argc; this->argc--, this->argv++) {
        const char *arg = this->argv[0];
        if (arg[0] != '-' || !arg[1]) {
            // if it's not option or is a single char '-', copy verbatimly
            this->out[this->cpidx++] = this->argv[0];
            continue;
        }
        // short option
        if (arg[1] != '-') {
            switch (argparse_short_opt(this, this->options)) {
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
        switch (argparse_long_opt(this, this->options)) {
        case -1:
            break;
        case -2:
            goto unknown;
        }
        continue;

unknown:
        fprintf(stderr, "error: unknown option `%s`\n", this->argv[0]);
        argparse_usage(this);
        continue;
    }

    memmove(this->out + this->cpidx, this->argv, this->argc * sizeof(*this->out));
    this->out[this->cpidx + this->argc] = NULL;

    return this->cpidx + this->argc;
}

void
argparse_usage(struct argparse *this)
{
    fprintf(stdout, "Usage: %s\n", *this->usage++);
    while (*this->usage && **this->usage)
        fprintf(stdout, "    or: %s\n", *this->usage++);
    fputc('\n', stdout);
    for (; this->options->type != OPTION_END; this->options++) {
        size_t pos;
        int pad;
        pos = fprintf(stdout, "    ");
        if (this->options->short_name) {
            pos += fprintf(stdout, "-%c", this->options->short_name);
        }
        if (this->options->long_name && this->options->short_name) {
            pos += fprintf(stdout, ", ");
        }
        if (this->options->long_name) {
            pos += fprintf(stdout, "--%s", this->options->long_name);
        }
        if (pos <= USAGE_OPTS_WIDTH) {
            pad = USAGE_OPTS_WIDTH - pos;
        } else {
            fputc('\n', stdout);
            pad = USAGE_OPTS_WIDTH;
        }
        fprintf(stdout, "%*s%s\n", pad + 2, "", this->options->help);
    }
    exit(129);
}

int
argparse_help_cb(struct argparse *this, struct argparse_option *option)
{
    argparse_usage(this);
    return 0;
}
