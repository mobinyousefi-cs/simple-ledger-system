/*
===========================================================
 Project:    Simple Ledger System
 File:       menu.h
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-26
 Updated:    2025-11-26
 License:    MIT License (see LICENSE file for details)
===========================================================

 Description:
    Declaration of the application runner that exposes the
    interactive CLI menu loop.

 Notes:
    - The caller must provide a valid data file path.
===========================================================
*/

#ifndef MENU_H
#define MENU_H

/* Run the interactive CLI application using the given data file path. */
void run_app(const char *data_path);

#endif /* MENU_H */
