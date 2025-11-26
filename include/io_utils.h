/*
===========================================================
 Project:    Simple Ledger System
 File:       io_utils.h
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-26
 Updated:    2025-11-26
 License:    MIT License (see LICENSE file for details)
===========================================================

 Description:
    Declarations for input utility helpers used throughout
    the CLI for robust user interaction and validation.

 Notes:
    - All functions operate on stdin/stdout.
===========================================================
*/

#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stddef.h>

/* Flush remaining characters from stdin until newline or EOF. */
void clear_stdin(void);

/* Read a non-empty line from stdin into buffer (strips newline). */
void read_line(const char *prompt, char *buffer, size_t size);

/* Read an integer (with validation) from stdin. */
int read_int(const char *prompt);

/* Read an integer within [min, max]. */
int read_int_in_range(const char *prompt, int min, int max);

/* Read a double (with validation) from stdin. */
double read_double(const char *prompt);

/* Pause until user presses ENTER. */
void wait_for_enter(void);

#endif /* IO_UTILS_H */
