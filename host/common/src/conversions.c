/*
 * This file is part of the bladeRF project:
 *   http://www.github.com/nuand/bladeRF
 *
 * Copyright (c) 2013 Nuand LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "conversions.h"

enum str2args_parse_state {
    IN_SPACE,
    START_ARG,
    IN_ARG,
    IN_QUOTE,
    ERROR,
};


int str2int(const char *str, int min, int max, bool *ok)
{
    long value;
    char *endptr;

    errno = 0;
    value = strtol(str, &endptr, 0);

    if (errno != 0 || value < (long)min || value > (long)max ||
        endptr == str || *endptr != '\0') {

        if (ok) {
            *ok = false;
        }

        return 0;
    }

    if (ok) {
        *ok = true;
    }
    return (int)value;
}

unsigned int str2uint(const char *str, unsigned int min, unsigned int max, bool *ok)
{
    unsigned long value;
    char *endptr;

    errno = 0;
    value = strtoul(str, &endptr, 0);

    if (errno != 0 ||
        value < (unsigned long)min || value > (unsigned long)max ||
        endptr == str || *endptr != '\0') {

        if (ok) {
            *ok = false;
        }

        return 0;
    }

    if (ok) {
        *ok = true;
    }
    return (unsigned int)value;
}


uint64_t str2uint64(const char *str, uint64_t min, uint64_t max, bool *ok)
{
    unsigned long long value;
    char *endptr;

    errno = 0;
    value = strtoull(str, &endptr, 0);

    if (errno != 0 || endptr == str || *endptr != '\0' ||
        value < (unsigned long long)min || value > (unsigned long long)max) {

        if (ok) {
            *ok = false;
        }

        return 0;
    }

    if (ok) {
        *ok = true;
    }

    return (uint64_t)value;
}

double str2double(const char *str, double min, double max, bool *ok)
{
    double value;
    char *endptr;

    errno = 0;
    value = strtod(str, &endptr);

    if (errno != 0 || value < min || value > max ||
        endptr == str || *endptr != '\0') {

        if (ok) {
            *ok = false;
        }

        return 0;
    }

    if (ok) {
        *ok = true;
    }

    return value;
}

unsigned int str2uint_suffix(const char *str, unsigned int min,
        unsigned int max, const struct numeric_suffix suffixes[],
        int num_suffixes, bool *ok)
{
    double value;
    char *endptr;
    int i;

    errno = 0;
    value = strtod(str, &endptr);

    /* If a number could not be parsed at the beginning of the string */
    if (errno != 0 || endptr == str) {
        if (ok) {
            *ok = false;
        }

        return 0;
    }

    /* Loop through each available suffix */
    for (i = 0; i < num_suffixes; i++) {
        /* If the suffix appears at the end of the number */
        if (!strcasecmp(endptr, suffixes[i].suffix)) {
            /* Apply the multiplier */
            value *= suffixes[i].multiplier;
            break;
        }
    }

    /* Check that the resulting value is in bounds */
    if (value > max || value < min) {
        if (ok) {
            *ok = false;
        }

        return 0;
    }

    if (ok) {
        *ok = true;
    }

    /* Truncate the floating point value to an integer and return it */
    return (unsigned int)value;
}

int str2version(const char *str, struct bladerf_version *version)
{
    unsigned long tmp;
    const char *start = str;
    char *end;

    /* Major version */
    errno = 0;
    tmp = strtoul(start, &end, 10);
    if (errno != 0 || tmp > UINT16_MAX || end == start || *end != '.') {
        return -1;
    }
    version->major = (uint16_t)tmp;

    /* Minor version */
    if (end[0] == '\0' || end[1] == '\0') {
        return -1;
    }
    errno = 0;
    start = &end[1];
    tmp = strtoul(start, &end, 10);
    if (errno != 0 || tmp > UINT16_MAX || end == start || *end != '.') {
        return -1;
    }
    version->minor = (uint16_t)tmp;

    /* Patch version */
    if (end[0] == '\0' || end[1] == '\0') {
        return -1;
    }
    errno = 0;
    start = &end[1];
    tmp = strtoul(start, &end, 10);
    if (errno != 0 || tmp > UINT16_MAX || end == start ||
            (*end != '-' && *end != '\0')) {
        return -1;
    }
    version->patch = (uint16_t)tmp;

    version->describe = str;

    return 0;
}

void free_args(int argc, char **argv)
{
    int i;

    if (argc > 0 && argv != NULL) {

        for (i = 0; i < argc; i++) {
            free(argv[i]);
        }

        argc = 0;
        free(argv);
    }
}

static void zero_argvs(int start, int end, char **argv)
{
    int i;
    for (i = start; i <= end; i++) {
        argv[i] = NULL;
    }
}

/* Returns 0 on success, -1 on failure */
static int append_char(char **arg, int *arg_size, int *arg_i, char c)
{
    void *tmp;

    if (*arg_i >= *arg_size) {
        tmp = realloc(*arg, *arg_size * 2);

        if (!tmp) {
            return -1;
        } else {
            *arg = tmp;
            *arg_size = *arg_size * 2;
        }
    }

    (*arg)[*arg_i] = c;
    *arg_i += 1;

    return 0;
}

const char * devspeed2str(bladerf_dev_speed speed)
{
    switch (speed) {
        case BLADERF_DEVICE_SPEED_HIGH:
            /* Yeah, the USB IF actually spelled it "Hi" instead of "High".
             * I know. It hurts me too. */
            return "Hi-Speed";

        case BLADERF_DEVICE_SPEED_SUPER:
            /* ...and no hyphen :( */
            return "SuperSpeed";

        default:
            return "Unknown";
    }
}

int str2args(const char *line, char ***argv_ret)
{
    int line_i, arg_i;      /* Index into line and current argument */
    int argv_size = 10;     /* Initial # of allocated args */
    int arg_size;           /* Allocated size of the curr arg */
    char **argv;
    int argc;
    enum str2args_parse_state state = IN_SPACE;
    const size_t line_len = strlen(line);


    argc = arg_i = 0;
    argv = malloc(argv_size * sizeof(char *));
    if (!argv) {
        return -1;
    }

    zero_argvs(0, argv_size - 1, argv);
    arg_size = 0;
    line_i = 0;

    while ((size_t)line_i < line_len && state != ERROR) {
        switch (state) {
            case IN_SPACE:
                /* Found the start of the next argument */
                if (!isspace(line[line_i])) {
                    state = START_ARG;
                } else {
                    /* Gobble up space */
                    line_i++;
                }
                break;

            case START_ARG:
                /* Increase size of argv, if needed */
                if (argc >= argv_size) {
                    argv_size = argv_size + argv_size / 2;
                    void *tmp = realloc(argv, argv_size);

                    if (tmp) {
                        argv = tmp;
                        zero_argvs(argc, argv_size - 1, argv);
                    } else {
                        state = ERROR;
                    }
                }

                /* Record start of word (unless we failed to realloc() */
                if (state != ERROR) {

                    /* Reset per-arg variables */
                    arg_i = 0;
                    arg_size = 32;

                    /* Allocate this argument. This will be
                     * realloc'd as necessary by append_char() */
                    argv[argc] = calloc(arg_size, 1);
                    if (!argv[argc]) {
                        state = ERROR;
                        break;
                    }

                    if (line[line_i] == '"') {
                        /* Gobble up quote */
                        state = IN_QUOTE;
                    } else {
                        /* Append this character to the argument begin
                         * fetching up a word */
                        if (append_char(&argv[argc], &arg_size,
                                    &arg_i, line[line_i])) {
                            state = ERROR;
                        } else {
                            state = IN_ARG;
                        }
                    }

                    argc++;
                    line_i++;
                }

                break;

            case IN_ARG:
                if (isspace(line[line_i])) {
                    state = IN_SPACE;
                } else if (line[line_i] == '"') {
                    state = IN_QUOTE;
                } else {
                    /* Append this character to the argument and remain in
                     * IN_ARG state */
                    if (append_char(&argv[argc - 1], &arg_size,
                                    &arg_i, line[line_i])) {
                        state = ERROR;
                    }
                }

                line_i++;
                break;


            case IN_QUOTE:
                if (line[line_i] == '"') {
                    /* Return to looking for more of the word */
                    state = IN_ARG;
                } else {
                    /* Append this character to the argumen */
                  if (append_char(&argv[argc - 1], &arg_size,
                                  &arg_i, line[line_i])) {
                      state = ERROR;
                  }
                }

                line_i++;
                break;

            case ERROR:
                break;
        }
    }

    /* Print error message if hit the EOL in an invalid state */
    switch (state) {
        case IN_SPACE:
        case IN_ARG:
            *argv_ret = argv;
            break;

        /* Unterminated quote or unexpexted state to end on */
        case IN_QUOTE:
        default:
            state = ERROR;
            break;
    }

    if (state == ERROR) {
        free_args(argc, argv);
        return -1;
    } else {
        return argc;
    }
}
