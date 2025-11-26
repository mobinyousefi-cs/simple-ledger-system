# Simple Ledger System (C)

A minimal yet robust **command-line ledger system** written in C.\
It records financial transactions (income and expenses), persists them
to disk, and provides basic analytics such as total income, total
expenses, and current balance.

------------------------------------------------------------------------

## Features

-   Add new transactions:
    -   Date (`YYYY-MM-DD`)
    -   Description
    -   Category
    -   Amount
    -   Type (Income / Expense)
-   List all recorded transactions in a tabular view.
-   Compute:
    -   Total income
    -   Total expenses
    -   Current balance
-   Search a transaction by ID and view its details.
-   Delete a transaction by ID.
-   Persistent storage via a binary ledger file (default: `ledger.dat`).

------------------------------------------------------------------------

## Directory Structure

``` text
simple-ledger-system/
├── src/
│   ├── main.c        # Program entry point
│   ├── menu.c        # CLI menu and interaction logic
│   ├── ledger.c      # Ledger data model and business logic
│   └── io_utils.c    # Input utilities and validation
├── include/
│   ├── ledger.h      # Ledger public API
│   ├── menu.h        # Application runner declaration
│   └── io_utils.h    # Input helpers and declarations
├── Makefile          # Build script
└── README.md         # Project documentation
```

------------------------------------------------------------------------

## Build Instructions

### Prerequisites

-   C compiler supporting C11 (e.g. `gcc`, `clang`)
-   `make` (for using the provided `Makefile`)

### Build

``` bash
make
```

This will generate the executable:

    bin/ledger

------------------------------------------------------------------------

## Run Instructions

### Default data file

``` bash
./bin/ledger
```

The application uses `ledger.dat` in the working directory by default.

### Custom data file

``` bash
./bin/ledger my_ledger_file.dat
```

------------------------------------------------------------------------

## Usage Example

``` text
========================================
  Simple Ledger System - Main Menu
========================================
  1) Add transaction
  2) List all transactions
  3) Show summary (income, expenses, balance)
  4) Find transaction by ID
  5) Delete transaction by ID
  0) Save & Exit
========================================
```

------------------------------------------------------------------------

## Data Model

### Transaction structure

``` c
typedef struct {
    int id;
    char date[11];
    char description[64];
    char category[32];
    double amount;
    TransactionType type;
} Transaction;
```

### Ledger structure

``` c
typedef struct {
    Transaction *items;
    size_t size;
    size_t capacity;
} Ledger;
```

------------------------------------------------------------------------

## Error Handling

-   Validates all user input (integers, doubles, nonempty strings).
-   Reports save/load failures.
-   Ensures safe memory handling and dynamic resizing.

------------------------------------------------------------------------

## Extensibility

Ideas for future improvements:

-   CSV export
-   Category filters
-   Date range queries
-   Monthly/annual reports
-   Unit tests for ledger operations

------------------------------------------------------------------------

## License

MIT License
