/*
===========================================================
 Project:    Simple Ledger System
 File:       io_utils.c
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-26
 Updated:    2025-11-26
 License:    MIT License (see LICENSE file for details)
===========================================================

 Description:
    Input utility functions for safe, validated reading from
    stdin (strings, integers, doubles) and basic user prompts.

 Usage:
    - read_line()
    - read_int(), read_int_in_range()
    - read_double()
    - wait_for_enter()

 Notes:
    - Prevents leftover characters in stdin by flushing.
===========================================================
*/

#include "io_utils.h"

#include <stdio.h>
#include <string.h>

static void strip_newline(char *buffer);

void clear_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* discard */
    }
}

void read_line(const char *prompt, char *buffer, size_t size)
{
    if (buffer == NULL || size == 0U)
    {
        return;
    }

    while (1)
    {
        if (prompt != NULL)
        {
            printf("%s", prompt);
            fflush(stdout);
        }

        if (fgets(buffer, (int)size, stdin) == NULL)
        {
            /* EOF or error; clear buffer and return */
            buffer[0] = '\0';
            return;
        }

        if (strchr(buffer, '\n') == NULL)
        {
            /* input longer than buffer; flush remainder */
            clear_stdin();
        }

        strip_newline(buffer);

        if (buffer[0] == '\0')
        {
            printf("Input cannot be empty. Please try again.\n");
        }
        else
        {
            break;
        }
    }
}

int read_int(const char *prompt)
{
    int value;
    char line[64];

    while (1)
    {
        if (prompt != NULL)
        {
            printf("%s", prompt);
            fflush(stdout);
        }

        if (fgets(line, (int)sizeof(line), stdin) == NULL)
        {
            clear_stdin();
            continue;
        }

        if (sscanf(line, "%d", &value) == 1)
        {
            return value;
        }

        printf("Invalid integer. Please try again.\n");
    }
}

int read_int_in_range(const char *prompt, int min, int max)
{
    int value;
    while (1)
    {
        value = read_int(prompt);
        if (value < min || value > max)
        {
            printf("Value must be between %d and %d.\n", min, max);
        }
        else
        {
            return value;
        }
    }
}

double read_double(const char *prompt)
{
    double value;
    char line[64];

    while (1)
    {
        if (prompt != NULL)
        {
            printf("%s", prompt);
            fflush(stdout);
        }

        if (fgets(line, (int)sizeof(line), stdin) == NULL)
        {
            clear_stdin();
            continue;
        }

        if (sscanf(line, "%lf", &value) == 1)
        {
            return value;
        }

        printf("Invalid number. Please try again.\n");
    }
}

void wait_for_enter(void)
{
    printf("Press ENTER to continue...");
    fflush(stdout);

    int c = getchar();
    if (c != '\n' && c != EOF)
    {
        clear_stdin();
    }
}

static void strip_newline(char *buffer)
{
    if (buffer == NULL)
    {
        return;
    }

    size_t len = strlen(buffer);
    if (len == 0U)
    {
        return;
    }

    if (buffer[len - 1U] == '\n')
    {
        buffer[len - 1U] = '\0';
    }
}
