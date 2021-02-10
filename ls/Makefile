CC = gcc
CFLAGES = -Og -Wall -std=gnu99
ALL= ls_v1 ls_v2
COMPILE = $(CC) $(CFLAGES) -c
all: $(ALL)

ls_v1.o: 
	$(COMPILE) ls_v1.c
	
ls_v2.o:
	$(COMPILE) ls_v2.c
clean :
	rm -f *.o $(ALL)

	