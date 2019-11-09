#
# In order to execute this "Makefile" just type "make"
#

OBJS 	= main.o tuple.o relation.o
T_OBJS 	= tuple.o relation.o

SOURCE	= tuple.c relation.c
HEADER  = ./include/tuple.h ./include/relation.h

OUT  	= main.exe

#

CC	= gcc
FLAGS   = -g -c  -march=native
# FLAGS   = -g -c  -march=skylake-avx512

# -g option enables debugging mode
# -c flag generates object code for separate files

# create/compile the individual files >>separately<<
all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

test: test.o
	$(CC) -g $(T_OBJS) test.o -o test.exe -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

test_run:
	./test.exe

test.o: test.c
	$(CC) $(FLAGS) test.c

main.o: main.c
	$(CC) $(FLAGS) main.c

tuple.o: tuple.c
	$(CC) $(FLAGS) tuple.c


# run:
# 	valgrind --leak-check=full --show-leak-kinds=all ./main.exe
run:
	./main.exe


clean:
	rm -f $(OBJS) $(OUT) test.o *.o *.exe *.csv
