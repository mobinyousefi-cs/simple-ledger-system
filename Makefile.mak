# ===========================================================
#  Project:    Simple Ledger System
#  File:       Makefile
#  Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
#  Created:    2025-11-26
#  Updated:    2025-11-26
#  License:    MIT License (see LICENSE file for details)
# ===========================================================
#
#  Description:
#     Build script for the Simple Ledger System C project.
#
#  Usage:
#     make        # build the project
#     make run    # build and run
#     make clean  # remove build artifacts
#
# ===========================================================

CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -Werror
INCLUDES := -Iinclude
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

TARGET  := $(BIN_DIR)/ledger

SRCS := $(SRC_DIR)/main.c \
        $(SRC_DIR)/menu.c \
        $(SRC_DIR)/ledger.c \
        $(SRC_DIR)/io_utils.c

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	$(TARGET)

clean:
	$(RM) -r $(OBJ_DIR) $(BIN_DIR)
