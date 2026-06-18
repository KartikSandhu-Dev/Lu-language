CC = gcc
CFLAGS = -Iheader -Wall

SRC = $(wildcard *.c files/*.c)
OBJ = $(SRC:.c=.o)

main: $(OBJ)
	$(CC) $(OBJ) -o main

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ) main
	@echo "CLEANED"