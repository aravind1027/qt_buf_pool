CC = g++

CFLAGS = -g -Wall

all: target

target: buf_pool.o buf_pool_tests.o
	$(CC) $(CFLAGS) buf_pool.o buf_pool_tests.o -o buf_pool.x -l gtest -l pthread 

buf_pool.o: buf_pool.cpp buf_pool.h
	$(CC) $(CFLAGS) -c buf_pool.cpp

buf_pool_tests.o: buf_pool_tests.cpp buf_pool.h
	$(CC) $(CFLAGS) -c buf_pool_tests.cpp 

clean:
	rm *.o *.x