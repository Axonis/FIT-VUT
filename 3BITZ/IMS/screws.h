#ifndef SCREWS
#define SCREWS

#include "simlib.h"

extern Store screw_errors;

class OverheatedScrew : public Process {
	void Behavior();
};

#endif //SCREWS