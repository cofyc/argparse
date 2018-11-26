# FAQs

## Why removing parsed command-line switches/options?

It destroys the original `argv` array, not compatible with other arguments parsing
library.

This is because this library is used for short-lived programs, e.g. cli tools
at beginning. It's very convenient to process remain arguments if we remove
parsed command-line arguments,  e.g. `<comamnd> [-[s]|--switch]... arguments`.

If you want keep original `argc/argv`, you can make a copy, then pass them to
`argparse_parse`, e.g.

```c
int copy_argc          = argc;
const char **copy_argv = argv;
copy_argv  = malloc(copy_argc * sizeof(char *));
for (int i = 0; i < argc; i++) {
	copy_argv[i] = (char *)argv[i];
}   
argparse_parse(&argparse, copy_argc, copy_argv);
```

Issues:

- https://github.com/cofyc/argparse/issues/3
- https://github.com/cofyc/argparse/issues/9

## Why using `intptr_t` to hold associated data? Why not `void *`?

I choose `intptr_t` because it's a integer type which also can be used to hold
a pointer value.  Most of the time, we only need a integer to hold
user-provided value, see `OPT_BIT` as example. If you want to provide a pointer
which points to a large amount of data, you can cast it to `intptr_t` and cast
it back to original pointer in callback function.
