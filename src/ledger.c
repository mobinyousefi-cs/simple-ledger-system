/*
===========================================================
 Project:    Simple Ledger System
 File:       ledger.c
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-26
 Updated:    2025-11-26
 License:    MIT License (see LICENSE file for details)
===========================================================

 Description:
    Implements the ledger data model and operations:
    - Dynamic array management
    - Add, remove, find transaction
    - Load/save binary persistence
    - Aggregations (income, expense, balance)
    - Printing ledger and summary

 Usage:
    Used by menu.c via the public API in ledger.h.

 Notes:
    - Binary format:
      [size_t count][Transaction * count]
===========================================================
*/

#include "ledger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INITIAL_CAPACITY 16

static int find_max_id(const Ledger *ledger);
static int ensure_capacity(Ledger *ledger, size_t min_capacity);

void ledger_init(Ledger *ledger)
{
    if (ledger == NULL)
    {
        return;
    }

    ledger->items = NULL;
    ledger->size = 0U;
    ledger->capacity = 0U;
}

void ledger_free(Ledger *ledger)
{
    if (ledger == NULL)
    {
        return;
    }

    free(ledger->items);
    ledger->items = NULL;
    ledger->size = 0U;
    ledger->capacity = 0U;
}

bool ledger_load(Ledger *ledger, const char *path)
{
    if (ledger == NULL || path == NULL)
    {
        return false;
    }

    FILE *fp = fopen(path, "rb");
    if (fp == NULL)
    {
        /* No file yet, not an error condition for this app. */
        return false;
    }

    size_t count = 0U;
    if (fread(&count, sizeof(size_t), 1U, fp) != 1U)
    {
        fclose(fp);
        fprintf(stderr, "Error: Failed to read record count from '%s'.\n", path);
        return false;
    }

    if (count == 0U)
    {
        fclose(fp);
        ledger_init(ledger);
        return true;
    }

    Transaction *buffer = (Transaction *)malloc(count * sizeof(Transaction));
    if (buffer == NULL)
    {
        fclose(fp);
        fprintf(stderr, "Error: Out of memory while loading ledger.\n");
        return false;
    }

    size_t read_count = fread(buffer, sizeof(Transaction), count, fp);
    fclose(fp);

    if (read_count != count)
    {
        fprintf(stderr, "Error: Corrupt ledger file '%s'.\n", path);
        free(buffer);
        return false;
    }

    ledger->items = buffer;
    ledger->size = count;
    ledger->capacity = count;

    return true;
}

bool ledger_save(const Ledger *ledger, const char *path)
{
    if (ledger == NULL || path == NULL)
    {
        return false;
    }

    FILE *fp = fopen(path, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open '%s' for writing: %s\n",
                path, strerror(errno));
        return false;
    }

    if (fwrite(&ledger->size, sizeof(size_t), 1U, fp) != 1U)
    {
        fprintf(stderr, "Error: Failed to write record count to '%s'.\n", path);
        fclose(fp);
        return false;
    }

    if (ledger->size > 0U)
    {
        if (fwrite(ledger->items, sizeof(Transaction), ledger->size, fp) != ledger->size)
        {
            fprintf(stderr, "Error: Failed to write records to '%s'.\n", path);
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

Transaction *ledger_add(Ledger *ledger, const Transaction *tx_template)
{
    if (ledger == NULL || tx_template == NULL)
    {
        return NULL;
    }

    if (ensure_capacity(ledger, ledger->size + 1U) != 0)
    {
        return NULL;
    }

    Transaction tx = *tx_template;

    int max_id = find_max_id(ledger);
    tx.id = max_id + 1;

    ledger->items[ledger->size] = tx;
    ledger->size++;

    return &ledger->items[ledger->size - 1U];
}

bool ledger_remove(Ledger *ledger, int id)
{
    if (ledger == NULL || ledger->items == NULL || ledger->size == 0U)
    {
        return false;
    }

    size_t index = 0U;
    bool found = false;

    for (size_t i = 0U; i < ledger->size; ++i)
    {
        if (ledger->items[i].id == id)
        {
            index = i;
            found = true;
            break;
        }
    }

    if (!found)
    {
        return false;
    }

    for (size_t i = index; i + 1U < ledger->size; ++i)
    {
        ledger->items[i] = ledger->items[i + 1U];
    }
    ledger->size--;

    return true;
}

Transaction *ledger_find_by_id(Ledger *ledger, int id)
{
    if (ledger == NULL || ledger->items == NULL)
    {
        return NULL;
    }

    for (size_t i = 0U; i < ledger->size; ++i)
    {
        if (ledger->items[i].id == id)
        {
            return &ledger->items[i];
        }
    }

    return NULL;
}

double ledger_total_income(const Ledger *ledger)
{
    if (ledger == NULL || ledger->items == NULL)
    {
        return 0.0;
    }

    double total = 0.0;
    for (size_t i = 0U; i < ledger->size; ++i)
    {
        if (ledger->items[i].type == TRANSACTION_INCOME)
        {
            total += ledger->items[i].amount;
        }
    }
    return total;
}

double ledger_total_expense(const Ledger *ledger)
{
    if (ledger == NULL || ledger->items == NULL)
    {
        return 0.0;
    }

    double total = 0.0;
    for (size_t i = 0U; i < ledger->size; ++i)
    {
        if (ledger->items[i].type == TRANSACTION_EXPENSE)
        {
            total += ledger->items[i].amount;
        }
    }
    return total;
}

double ledger_balance(const Ledger *ledger)
{
    return ledger_total_income(ledger) - ledger_total_expense(ledger);
}

void ledger_print(const Ledger *ledger)
{
    if (ledger == NULL)
    {
        printf("Ledger is not initialized.\n");
        return;
    }

    if (ledger->size == 0U)
    {
        printf("No transactions recorded.\n");
        return;
    }

    printf("%-4s  %-10s  %-7s  %-10s  %-14s  %-40s\n",
           "ID", "Date", "Type", "Amount", "Category", "Description");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (size_t i = 0U; i < ledger->size; ++i)
    {
        const Transaction *tx = &ledger->items[i];
        printf("%-4d  %-10s  %-7s  %-10.2f  %-14s  %-40s\n",
               tx->id,
               tx->date,
               (tx->type == TRANSACTION_INCOME) ? "Income" : "Expense",
               tx->amount,
               tx->category,
               tx->description);
    }
}

void ledger_print_summary(const Ledger *ledger)
{
    double income = ledger_total_income(ledger);
    double expense = ledger_total_expense(ledger);
    double balance_val = income - expense;

    printf("Total income : %.2f\n", income);
    printf("Total expense: %.2f\n", expense);
    printf("----------------------\n");
    printf("Balance      : %.2f\n", balance_val);
}

/* Internal helpers */

static int find_max_id(const Ledger *ledger)
{
    if (ledger == NULL || ledger->size == 0U)
    {
        return 0;
    }

    int max_id = ledger->items[0].id;
    for (size_t i = 1U; i < ledger->size; ++i)
    {
        if (ledger->items[i].id > max_id)
        {
            max_id = ledger->items[i].id;
        }
    }

    return max_id;
}

static int ensure_capacity(Ledger *ledger, size_t min_capacity)
{
    if (ledger->capacity >= min_capacity)
    {
        return 0;
    }

    size_t new_capacity = (ledger->capacity == 0U)
                              ? INITIAL_CAPACITY
                              : ledger->capacity;

    while (new_capacity < min_capacity)
    {
        new_capacity *= 2U;
    }

    Transaction *new_items = (Transaction *)realloc(
        ledger->items,
        new_capacity * sizeof(Transaction));
    if (new_items == NULL)
    {
        fprintf(stderr, "Error: Out of memory while expanding ledger.\n");
        return -1;
    }

    ledger->items = new_items;
    ledger->capacity = new_capacity;

    return 0;
}
