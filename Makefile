CC = gcc
PKG_CONFIG = pkg-config

TARGET = build/chip8.exe

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

CFLAGS = -Wall -Wextra -std=c23 -I$(INC_DIR)
SDL_CFLAGS = $(shell $(PKG_CONFIG) --cflags sdl3)
SDL_LIBS = $(shell $(PKG_CONFIG) --libs sdl3)

CFLAGS += $(SDL_CFLAGS)
LDFLAGS = $(SDL_LIBS)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) roms/8-1dcell.ch8

clean:
	@if exist "$(BUILD_DIR)" rmdir /S /Q "$(BUILD_DIR)"
