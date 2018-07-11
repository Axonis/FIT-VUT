#ifndef COUNTERS
#define COUNTERS

#include <simlib.h>

#define SHIFTS_IN_DAY 3

/*
 * Counters will singal program using Facilities.
 */
extern Facility summer;

extern Facility working_week;

extern Facility weekend;

extern Facility start_break;

extern Facility end_break;

/*
 * Counters
 */
class YearCounter : public Process {
public:
	void Behavior() override;
};

class WeekCounter : public Process {
public:
	void Behavior() override;
};

class BreakCounter : public Process {
public:
	void Behavior() override;
};





#endif //COUNTERS
