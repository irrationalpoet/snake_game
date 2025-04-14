# Makefile for Snake game with Homebrew and SDL dependency checks/install

CC = gcc
CFLAGS = -Wall -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/opt/sdl2_ttf/include
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -L/opt/homebrew/opt/sdl2_ttf/lib
LIBS = -lSDL2 -lSDL2_ttf

SRC = snake.c
TARGET = snake

all: check-deps $(TARGET)

check-deps:
	@echo "🔍 Checking for Homebrew..."
	@if ! command -v brew >/dev/null 2>&1; then \
		echo "🚨 Homebrew not found. Attempting to install..."; \
		/bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"; \
		echo "✅ Homebrew installed."; \
	fi
	@echo "🔍 Checking for SDL2..."
	@if ! brew list sdl2 >/dev/null 2>&1; then \
		echo "📦 Installing SDL2..."; \
		brew install sdl2; \
	fi
	@echo "🔍 Checking for SDL2_ttf..."
	@if ! brew list sdl2_ttf >/dev/null 2>&1; then \
		echo "📦 Installing SDL2_ttf..."; \
		brew install sdl2_ttf; \
	fi

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
