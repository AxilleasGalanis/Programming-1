CC = gcc
CFLAGS = -Wall -g
OBJ = main.o objdb.o

project3: $(OBJ)
    $(CC) $(OBJ) -o project3

main.o: main.c objdb.h
    $(CC) $(CFLAGS) -c $<

objdb.o: objdb.c objdb.h
    $(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
    rm *.o    
