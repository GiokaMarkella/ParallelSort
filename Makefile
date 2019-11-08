#
# In order to execute this "Makefile" just type "make"
#

OBJS 	= main.o tuple.o relation.o radix_sort.o merge.o bucket.o quickSort.o resultsList.o sort_merge_join.o
T_OBJS 	= tuple.o relation.o radix_sort.o merge.o bucket.o quickSort.o resultsList.o

SOURCE	= tuple.c relation.c radix_sort.c merge.c bucket.c quickSort.c resultsList.c sort_merge_join.c
HEADER  = ./include/tuple.h ./include/relation.h ./include/radix_sort.h ./include/merge.h ./include/bucket.h ./include/quickSort.h ./include/resultsList.h ./include/sort_merge_join.h

OUT  	= main.exe

#

CC	= gcc
FLAGS   = -g -c
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

relation.o: relation.c
	$(CC) $(FLAGS) relation.c

radix_sort.o: radix_sort.c
	$(CC) $(FLAGS) radix_sort.c

sort_merge_join.o: sort_merge_join.c
	$(CC) $(FLAGS) sort_merge_join.c

bucket.o: bucket.c
	$(CC) $(FLAGS) bucket.c

merge.o: merge.c
	$(CC) $(FLAGS) merge.c

quickSort.o: quickSort.c
	$(CC) $(FLAGS) quickSort.c

resultsList.o: resultsList.c
	$(CC) $(FLAGS) resultsList.c


# run:
# 	valgrind --leak-check=full --show-leak-kinds=all ./main.exe


on:
	valgrind --leak-check=full --show-leak-kinds=all ./main.exe -norm on -data old -r1 ./input/r0 -r2 ./input/r1

off:
	valgrind --leak-check=full --show-leak-kinds=all ./main.exe -norm off -data old -r1 ./input/r0 -r2 ./input/r1

tiny:
	valgrind --leak-check=full --show-leak-kinds=all ./main.exe -norm off -data new -r1 ./Datasets/tiny/relA -r2 ./Datasets/tiny/relB

small:
	valgrind --leak-check=full --show-leak-kinds=all ./main.exe -norm off -data new -r1 ./Datasets/small/relA -r2 ./Datasets/small/relB

new_small:
	valgrind --leak-check=full --show-leak-kinds=all ./main.exe -norm off -data new -r1 test_data -r2 ./Datasets/small/relB

mytest:
	valgrind --leak-check=full  --show-leak-kinds=all ./main.exe -norm off -data new -r1 test_data1 -r2 test_data2


medium:
	valgrind --leak-check=full --show-leak-kinds=all ./main.exe -norm off -data new -r1 ./Datasets/medium/relA -r2 ./Datasets/medium/relB




clean:
	rm -f $(OBJS) $(OUT) test.o *.o *.exe *.csv
