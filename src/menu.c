/*
===========================================================
 Project:    Simple Ledger System
 File:       menu.c
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-26
 Updated:    2025-11-26
 License:    MIT License (see LICENSE file for details)
===========================================================

 Description:
    Implements the interactive command-line menu and ties
    user actions to ledger operations (add, list, summary,
    search, delete).

 Usage:
    Called via run_app() from main.c.

 Notes:
    - All write operations trigger a save to disk.
===========================================================
*/

#include "menu.h"
#include "ledger.h"
#include "io_utils.h"

#include <stdio.h>
#include <stdbool.h>

static void print_menu(void);
static void handle_add_transaction(Ledger *ledger);
static void handle_list_transactions(const Ledger *ledger);
static void handle_show_summary(const Ledger *ledger);
static void handle_find_transaction(Ledger *ledger);
static void handle_delete_transaction(Ledger *ledger);

void run_app(const char *data_path)
{
    Ledger ledger;
    ledger_init(&ledger);

    if (!ledger_load(&ledger, data_path))
    {
        printf("No existing ledger found or failed to load. "
               "Starting with an empty ledger.\n\n");
    }

    bool running = true;

    while (running)
    {
        print_menu();
        int choice = read_int_in_range("Select an option: ", 0, 5);
        printf("\n");

        switch (choice)
        {
        case 1:
            handle_add_transaction(&ledger);
            if (!ledger_save(&ledger, data_path))
            {
                fprintf(stderr, "Warning: Failed to save ledger to '%s'.\n", data_path);
            }
            break;
        case 2:
            handle_list_transactions(&ledger);
            break;
        case 3:
            handle_show_summary(&ledger);
            break;
        case 4:
            handle_find_transaction(&ledger);
            break;
        case 5:
            handle_delete_transaction(&ledger);
            if (!ledger_save(&ledger, data_path))
            {
                fprintf(stderr, "Warning: Failed to save ledger to '%s'.\n", data_path);
            }
            break;
        case 0:
        default:
            running = false;
            break;
        }

        if (running)
        {
            printf("\n");
            wait_for_enter();
        }
    }

    if (!ledger_save(&ledger, data_path))
    {
        fprintf(stderr, "Error: Failed to save ledger before exit.\n");
    }

    ledger_free(&ledger);
    printf("Goodbye.\n");
}

static void print_menu(void)
{
    printf("========================================\n");
    printf("  Simple Ledger System - Main Menu\n");
    printf("========================================\n");
    printf("  1) Add transaction\n");
    printf("  2) List all transactions\n");
    printf("  3) Show summary (income, expenses, balance)\n");
    printf("  4) Find transaction by ID\n");
    printf("  5) Delete transaction by ID\n");
    printf("  0) Save & Exit\n");
    printf("========================================\n");
}

static void handle_add_transaction(Ledger *ledger)
{
    Transaction tx;

    printf("Add New Transaction\n");
    printf("-------------------\n");

    read_line("Date (YYYY-MM-DD): ", tx.date, sizeof(tx.date));
    read_line("Description: ", tx.description, sizeof(tx.description));
    read_line("Category   : ", tx.category, sizeof(tx.category));

    tx.amount = read_double("Amount: ");

    int type_choice = read_int_in_range(
        "Type (1 = Income, 2 = Expense): ",
        1, 2);

    tx.type = (type_choice == 1)
                  ? TRANSACTION_INCOME
                  : TRANSACTION_EXPENSE;

    Transaction *created = ledger_add(ledger, &tx);
    if (created != NULL)
    {
        printf("\nTransaction added successfully with ID %d.\n", created->id);
    }
    else
    {
        fprintf(stderr, "Error: Failed to add transaction.\n");
    }
}

static void handle_list_transactions(const Ledger *ledger)
{
    printf("All Transactions\n");
    printf("----------------\n");
    ledger_print(ledger);
}

static void handle_show_summary(const Ledger *ledger)
{
    printf("Ledger Summary\n");
    printf("--------------\n");
    ledger_print_summary(ledger);
}

static void handle_find_transaction(Ledger *ledger)
{
    int id = read_int("Enter transaction ID to find: ");
    Transaction *tx = ledger_find_by_id(ledger, id);
    if (tx == NULL)
    {
        printf("Transaction with ID %d not found.\n", id);
        return;
    }

    printf("\nTransaction Details\n");
    printf("-------------------\n");
    printf("ID         : %d\n", tx->id);
    printf("Date       : %s\n", tx->date);
    printf("Type       : %s\n",
           (tx->type == TRANSACTION_INCOME) ? "Income" : "Expense");
    printf("Amount     : %.2f\n", tx->amount);
    printf("Category   : %s\n", tx->category);
    printf("Description: %s\n", tx->description);
}

static void handle_delete_transaction(Ledger *ledger)
{
    int id = read_int("Enter transaction ID to delete: ");
    if (ledger_remove(ledger, id))
    {
        printf("Transaction with ID %d deleted.\n", id);
    }
    else
    {
        printf("Transaction with ID %d not found.\n", id);
    }
}
