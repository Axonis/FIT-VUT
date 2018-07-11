CC=g++
CFLAGS=-std=c++11 -g -o $@ src/isamon.cpp -pthread

isamon: src/isamon.cpp
	$(CC) $(CFLAGS) 
clean:
	rm -f *.o *.out isamon
