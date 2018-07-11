all:ims

CXXFLAGS= -pedantic -Wall -std=c++11 -lm -lsimlib

counters.o: counters.cpp counters.h constants.h debug.h line.h stats.h
line.o: line.cpp line.h constants.h screws.h counters.h debug.h stats.h params.h
main.o: main.cpp counters.h constants.h line.h stats.h
screws.o: screws.cpp screws.h constants.h
stats.o: stats.cpp stats.h

ims: counters.o line.o main.o screws.o stats.o
	g++ -o $@ $(CXXFLAGS) $^

clean:
	rm -rvf ims *.o

pack:
	zip xurban66_xtomas32.zip *.cpp *.h Makefile doc.pdf
