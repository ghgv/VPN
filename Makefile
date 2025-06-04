# Directorios
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj
INCLUDE_DIR := include/

# Lista de subdirectorios de src (ej. src/mod1, src/mod2)
SUBDIRS := $(shell find $(SRC_DIR) -mindepth 1 -maxdepth 1 -type d)

# Nombre de ejecutable = nombre del subdirectorio
TARGETS := $(patsubst $(SRC_DIR)/%, $(BIN_DIR)/%, $(SUBDIRS))

# Compilador y flags
CC := gcc
CFLAGS := -Wall -O2 -I$(INCLUDE_DIR)

# Regla por defecto
all: $(TARGETS)

# Compilar cada subdirectorio src/xxx/*.c → bin/xxx
$(BIN_DIR)/%: $(SRC_DIR)/%
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^/*.c -o $@

# Limpiar ejecutables
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
