CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -shared -o $@
SRCDIR = classes/c
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:.c=.o)

TARGETDIR = classes/c
TARGET = $(TARGETDIR)/engine.out

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
