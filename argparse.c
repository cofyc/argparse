#include "argparse.h"

static const char *
skip_prefix(const char *str, const char *prefix)
{
    size_t len = strlen(prefix);
    return strncmp(str, prefix, len) ? NULL : str + len;
}

static void
argparse_error(struct argparse *this, const struct argparse_option *opt,
               const char *reason)
{
    if (!strncmp(this->argv[0], "--", 2)) {
        fprintf(stderr, "error: option `%s` %s\n", opt->long_name, reason);
        exit(-1);
    } else {
        fprintf(stderr, "error: option `%c` %s\n", opt->short_name, reason);
        exit(-1);
    }
}

static int
argparse_getvalue(struct argparse *this, const struct argparse_option *opt)
{
    const char *s;
    if (!opt->value)
        goto skipped;
    switch (opt->type) {
    case ARGPARSE_OPT_BOOLEAN:
        *(int *)opt->value = *(int *)opt->value + 1;
        break;
    case ARGPARSE_OPT_BIT:
        *(int *)opt->value |= opt->data;
        break;
    case ARGPARSE_OPT_STRING:
        if (this->optvalue) {
            *(const char **)opt->value = this->optvalue;
            this->optvalue = NULL;
        } else if (this->argc > 1) {
            this->argc--;
            *(const char **)opt->value = *++this->argv;
        } else {
            argparse_error(this, opt, "requires a value");
        }
        break;
    case ARGPARSE_OPT_INTEGER:
        if (this->optvalue) {
            *(int *)opt->value = strtol(this->optvalue, (char **)&s, 0);
            this->optvalue = NULL;
        } else if (this->argc > 1) {
            this->argc--;
            *(int *)opt->value = strtol(*++this->argv, (char **)&s, 0);
        } else {
            argparse_error(this, opt, "requires a value");
        }
        if (*s)
            argparse_error(this, opt, "expects a numerical value");
        break;
    default:
        assert(0);
    }

skipped:
    if (opt->callback) {
        return opt->callback(this, opt);
    }

    return 0;
}

static void
argparse_options_check(const struct argparse_option *options)
{
    for (; options->type != ARGPARSE_OPT_END; options++) {
        switch (options->type) {
        case ARGPARSE_OPT_END:
        case ARGPARSE_OPT_BOOLEAN:
        case ARGPARSE_OPT_BIT:
        case ARGPARSE_OPT_INTEGER:
        case ARGPARSE_OPT_STRING:
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
    for (; options->type != ARGPARSE_OPT_END; options++) {
        if (options->short_name == *this->optvalue) {
            this->optvalue = this->optvalue[1] ? this->optvalue + 1 : NULL;
            return argparse_getvalue(this, options);
        }
    }
    return -2;
}

static int
argparse_long_opt(struct argparse *this, const struct argparse_option *options)
{
    for (; options->type != ARGPARSE_OPT_END; options++) {
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
        return argparse_getvalue(this, options);
    }
    return -2;
}

int
argparse_init(struct argparse *this, struct argparse_option *options,
              const char *const *usage, int flags)
{
    memset(this, 0, sizeof(*this));
    this->options = options;
    this->usage = usage;
    this->flags = flags;
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
            if (this->flags & ARGPARSE_STOP_AT_NON_OPTION) {
                goto end;
            }
            // if it's not option or is a single char '-', copy verbatimly
            this->out[this->cpidx++] = this->argv[0];
            continue;
        }
        // short option
        if (arg[1] != '-') {
            this->optvalue = arg + 1;
            switch (argparse_short_opt(this, this->options)) {
            case -1:
                break;
            case -2:
                goto unknown;
            }
            while (this->optvalue) {
                switch (argparse_short_opt(this, this->options)) {
                case -1:
                    break;
                case -2:
                    goto unknown;
                }
            }
            continue;
        }
        // if '--' presents
        if (!arg[2]) {
            this->argc--;
            this->argv++;
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
        exit(0);
    }

end:
    memmove(this->out + this->cpidx, this->argv,
            this->argc * sizeof(*this->out));
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

    // figure out best width
    int usage_opts_width = 0;
    int i;
    int len;
    for (i = 0; (this->options + i)->type != ARGPARSE_OPT_END; i++) {
        len = 0;
        if ((this->options + i)->short_name) {
            len += 2;
        }
        if ((this->options + i)->short_name && (this->options + i)->long_name) {
            len += 2;           // separator ", "
        }
        if ((this->options + i)->long_name) {
            len += strlen((this->options + i)->long_name) + 2;
        }
        len = ceil((float)len / 4) * 4;
        if (usage_opts_width < len) {
            usage_opts_width = len;
        }
    }
    usage_opts_width += 4;      // 4 spaces prefix

    for (; this->options->type != ARGPARSE_OPT_END; this->options++) {
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
        if (pos <= usage_opts_width) {
            pad = usage_opts_width - pos;
        } else {
            fputc('\n', stdout);
            pad = usage_opts_width;
        }
        fprintf(stdout, "%*s%s\n", pad + 2, "", this->options->help);
    }
}

int
argparse_help_cb(struct argparse *this, const struct argparse_option *option)
{
    argparse_usage(this);
    exit(0);
    return 0;
}
