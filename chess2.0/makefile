CC = gcc
CFLAGS = -Wall -Wextra -g

SRCDIR = classes/c
SRC = $(SRCDIR)/eval.c $(SRCDIR)/gen.c
OBJ = $(SRC:.c=.o)

TARGET = engine.out

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
