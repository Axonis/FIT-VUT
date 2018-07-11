#ifndef LINE
#define LINE

#include <simlib.h>

extern int post1_mux;
extern int post2_mux;
extern int post3_mux;
extern int post4_mux;

extern long long processed_cars;

class CarGenerator : public Event {
public:
	void Behavior();
};

class Car : public Process {
public:
	void Behavior();
};

class MainLineProc : public Process {
public:
	void Behavior();
};

#endif //LINE