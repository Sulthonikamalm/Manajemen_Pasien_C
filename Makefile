# Makefile untuk Sistem Manajemen Rumah Sakit Jiwa SultHim (Raylib Version)
# Compiler dan flags

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -D_DEFAULT_SOURCE -Wno-missing-braces
LDFLAGS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

# Direktori
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
OBJ_DIR = $(BUILD_DIR)/obj

# Target executable
TARGET = $(BIN_DIR)/rumah_sakit_jiwa_gui.exe
TARGET_CONSOLE = $(BIN_DIR)/rumah_sakit_jiwa_console.exe

# Source files (GUI)
SOURCES_GUI = $(SRC_DIR)/main_gui.c \
          $(SRC_DIR)/gui/ui_components.c \
          $(SRC_DIR)/gui/gui_pages.c \
          $(SRC_DIR)/models/pasien.c \
          $(SRC_DIR)/models/rekam_medis.c \
          $(SRC_DIR)/models/resep.c \
          $(SRC_DIR)/utils/csv_handler.c \
          $(SRC_DIR)/utils/validator.c \
          $(SRC_DIR)/utils/date_utils.c \
          $(SRC_DIR)/utils/string_utils.c

# Source files (Console)
SOURCES_CONSOLE = $(SRC_DIR)/main_console.c \
          $(SRC_DIR)/models/pasien.c \
          $(SRC_DIR)/models/rekam_medis.c \
          $(SRC_DIR)/models/resep.c \
          $(SRC_DIR)/utils/csv_handler.c \
          $(SRC_DIR)/utils/validator.c \
          $(SRC_DIR)/utils/date_utils.c \
          $(SRC_DIR)/utils/string_utils.c

# Object files
OBJECTS_GUI = $(SOURCES_GUI:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJECTS_CONSOLE = $(SOURCES_CONSOLE:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target - build raylib gui by default
all: directories $(TARGET)

# Build GUI version
gui: directories $(TARGET)

# Build console version
console: directories $(TARGET_CONSOLE)

# Buat direktori yang diperlukan
directories:
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(OBJ_DIR)\models" mkdir "$(OBJ_DIR)\models"
	@if not exist "$(OBJ_DIR)\utils" mkdir "$(OBJ_DIR)\utils"
	@if not exist "$(OBJ_DIR)\gui" mkdir "$(OBJ_DIR)\gui"
	@if not exist "data" mkdir "data"

# Link object files menjadi executable (GUI version)
$(TARGET): $(OBJECTS_GUI)
	@echo "Linking $(TARGET)..."
	@$(CC) $(OBJECTS_GUI) -o $(TARGET) $(LDFLAGS)
	@echo "Build GUI berhasil! Executable: $(TARGET)"

# Link object files menjadi executable (Console version)
$(TARGET_CONSOLE): $(OBJECTS_CONSOLE)
	@echo "Linking $(TARGET_CONSOLE)..."
	@$(CC) $(OBJECTS_CONSOLE) -o $(TARGET_CONSOLE)
	@echo "Build Console berhasil! Executable: $(TARGET_CONSOLE)"

# Compile source files menjadi object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	@echo "Cleaning build files..."
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
	@echo "Clean selesai!"

# Run aplikasi GUI
run: gui
	@echo "Menjalankan aplikasi GUI..."
	@$(TARGET)

# Run aplikasi console
run-console: console
	@echo "Menjalankan aplikasi console..."
	@$(TARGET_CONSOLE)

.PHONY: all clean run run-console help directories
