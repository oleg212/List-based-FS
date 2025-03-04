CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src/fs -I./src/blocks -I./src/superblock

# SRC = src/*.c \ #
SRC = src/main.c  \
      src/types/block/block.c \
	  src/types/block/superblock.c \
	  src/types/filesystem/filesystem.c

OBJ = $(SRC:.c=.o)

TARGET = fs_app
OBJS = main.o filesystem.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
