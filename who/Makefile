CC = gcc
CFLAGES = -Og -Wall -std=gnu99
ALL= who_v1 who_v2 who_v3  who_v4 whoami cp_v1
COMPILE = $(CC) $(CFLAGES) -c
all: $(ALL)

who_v1.o: 
	$(COMPILE) who_v1.c

who_v2.o:
	$(COMPILE) who_v2.c

who_v3.o:
	$(COMPILE) who_v3.c

who_v4.o:
	$(COMPILE) who_v4.c

whoami.o:
	$(COMPILE) whoami.c
	
cp_v1.o:
	$(COMPILE) cp_v1.c
	
clean :
	rm -f *.o $(ALL)
