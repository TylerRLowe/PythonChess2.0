CC = gcc
CFLAGS = -Wall -Werror 

# List your source files here
SOURCES = eval.c

# Derive object file names from source files
OBJECTS = $(SOURCES:.c=.o)

# Name of the final executable
EXECUTABLE = engine.exe

# Build rule for the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# Rule to compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
