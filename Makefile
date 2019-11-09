#
# In order to execute this "Makefile" just type "make"
#

OBJS 	= main.o relation.o vector.o merge_sort.o
T_OBJS 	= relation.o

SOURCE	=  relation.c merge_sort.c vector.c
HEADER  =  ./include/relation.h ./include/vector.h ./include/merge_sort.h

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

vector.o: vector.c
	$(CC) $(FLAGS) vector.c

merge_sort.o: merge_sort.c
	$(CC) $(FLAGS) merge_sort.c
# run:
# 	valgrind --leak-check=full --show-leak-kinds=all ./main.exe
run:
	./main.exe


r:
	valgrind --leak-check=full --show-leak-kinds=all ./main.exe -r ./Datasets/tiny/relA




clean:
	rm -f $(OBJS) $(OUT) test.o *.o *.exe *.csv
