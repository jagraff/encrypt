CC := gcc
CFLAGS := -Wall -g
LDFLAGS :=


OBJECTS := receiver.o sender.o

all: receiver sender


%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f receiver sender
	rm -f *.o

.PHONY: clean
