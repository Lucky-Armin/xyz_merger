# Compile xyz_merger.c
# Usage: make

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O3
OBJ = xyz_merger.o

# load library
LDLIBS = -lm

all: xyz_merger

xyz_merger: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o xyz_merger merged_trajectory.xyz
