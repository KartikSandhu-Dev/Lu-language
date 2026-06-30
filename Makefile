CC = gcc
CFLAGS = -Wall -Wextra -Ilexer -Iparser -I./include

SRC = src/main.c \
      src/lexer.c \
      src/parser.c \
      src/compiler.c \
      src/vm/vm.c \
      src/vm/bytecode.c \
      src/vm/instruction.c \
      src/vm/stack.c \

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