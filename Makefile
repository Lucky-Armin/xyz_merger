# Compile xyz_merger.c
# Usage: make

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -lm -O3
OBJ = xyz_merger.o

all: xyz_merger

xyz_merger: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o xyz_merger merged_trajectory.xyz