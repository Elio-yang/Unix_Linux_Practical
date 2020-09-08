CC = gcc
CFLAGES = -Og -Wall -std=gnu99
ALL= who_v1 who_v2 who_v3 cp_v1

all: $(ALL)

who_v1.o: 
	$(CC) $(CFLAGES) -c who_v1.c

who_v2.o:
	$(CC) $(CFLAGES) -c who_v2.c

who_v3.o:
	$(CC) $(CFLAGES) -c who_v3.c


cp_v1.o:
	$(CC) $(CFLAGES) -c cp_v1.c
	
clean :
	rm -f *.o $(ALL)

	