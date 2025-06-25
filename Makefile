CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = build/fs

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -Iinclude $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) src/*.o
