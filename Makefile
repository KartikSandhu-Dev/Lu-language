CC = gcc
CFLAGS = -Wall -Wextra -Ilexer -Iparser

SRC = main.c \
      lexer/lexer.c \
      parser/parser.c

OBJ = $(SRC:.c=.o)

TARGET = compiler

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@./$(TARGET) prac.lu

clean:
	@echo "CLEANED"
	@rm -f $(OBJ) $(TARGET)

.PHONY: all run clean