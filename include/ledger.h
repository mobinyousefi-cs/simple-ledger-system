/*
===========================================================
 Project:    Simple Ledger System
 File:       ledger.h
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-26
 Updated:    2025-11-26
 License:    MIT License (see LICENSE file for details)
===========================================================

 Description:
    Public API for the ledger module, including the
    Transaction and Ledger data structures and high-level
    operations on them.

 Notes:
    - All functions assume a properly initialized Ledger.
===========================================================
*/

#ifndef LEDGER_H
#define LEDGER_H

#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    TRANSACTION_EXPENSE = -1,
    TRANSACTION_INCOME  =  1
} TransactionType;

typedef struct
{
    int id;
    char date[11];        /* "YYYY-MM-DD" + '\0' */
    char description[64];
    char category[32];
    double amount;
    TransactionType type;
} Transaction;

typedef struct
{
    Transaction *items;
    size_t size;
    size_t capacity;
} Ledger;

/* Lifecycle */
void ledger_init(Ledger *ledger);
void ledger_free(Ledger *ledger);

/* Persistence */
bool ledger_load(Ledger *ledger, const char *path);
bool ledger_save(const Ledger *ledger, const char *path);

/* Operations */
Transaction *ledger_add(Ledger *ledger, const Transaction *tx_template);
bool         ledger_remove(Ledger *ledger, int id);
Transaction *ledger_find_by_id(Ledger *ledger, int id);

/* Aggregations / calculations */
double ledger_total_income(const Ledger *ledger);
double ledger_total_expense(const Ledger *ledger);
double ledger_balance(const Ledger *ledger);

/* Presentation */
void ledger_print(const Ledger *ledger);
void ledger_print_summary(const Ledger *ledger);

#endif /* LEDGER_H */
