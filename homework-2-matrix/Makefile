# Makefile to compile a complex program composed of many libraries

### Variables for this project ###
# The files that must be compiled, with a .o extension
OBJECTS = newMatrix.c
# The header files
#DEPENDS =
# The executable program to be created
MAIN = matrix

### Variables for the compilation rules ###
# These should work for most projects, but can be modified when necessary
# The compiler program to use
CC = gcc
# Options to use when compiling object files
CFLAGS = -Wall -g -std=c99 -pedantic
# Options to use for the final linking process
# This one links the math library
LDLIBS = -lm
LDFLAGS = -1 Matrix_A.txt -2 Matrix_D.txt -r result.txt

# Default rule
all: $(MAIN)

# Rule to make the executable
$(MAIN): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the object files
%.o: %.c $(DEPENDS)
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o $(MAIN)

zip: clean
	zip -r $(MAIN).zip *

# Tel make file that these are not file names
.PHONY: clean all zip