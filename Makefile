CC = gcc
CFLAGS = -Wall -Wextra -std=c99

EXEC1 = spc
EXEC2 = spc2

.PHONY: all clean

all: $(EXEC1) $(EXEC2)

$(EXEC1): src/spc.c
	$(CC) $(CFLAGS) src/spc.c -o $(EXEC1)

$(EXEC2): src/spc2.c
	$(CC) $(CFLAGS) src/spc2.c -o $(EXEC2)

clean:
	rm -f $(EXEC1) $(EXEC2)