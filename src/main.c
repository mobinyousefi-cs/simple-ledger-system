/*
===========================================================
 Project:    Simple Ledger System
 File:       main.c
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-26
 Updated:    2025-11-26
 License:    MIT License (see LICENSE file for details)
===========================================================

 Description:
    Program entry point. Parses command-line arguments to
    determine the data file path and launches the CLI app.

 Usage:
    ./bin/ledger
    ./bin/ledger custom_ledger.dat

 Notes:
    - If no data file exists, an empty ledger is initialized.
===========================================================
*/

#include <stdio.h>
#include <string.h>
#include "menu.h"

#define DEFAULT_DATA_FILE "ledger.dat"

static const char *resolve_data_path(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    const char *data_path = resolve_data_path(argc, argv);

    printf("Simple Ledger System\n");
    printf("Data file: %s\n\n", data_path);

    run_app(data_path);

    return 0;
}

static const char *resolve_data_path(int argc, char *argv[])
{
    if (argc >= 2 && argv[1] != NULL && strlen(argv[1]) > 0)
    {
        return argv[1];
    }
    return DEFAULT_DATA_FILE;
}
