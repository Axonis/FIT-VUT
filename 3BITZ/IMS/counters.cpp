#include "counters.h"
#include "constants.h"
#include "debug.h"
#include "line.h"
#include "stats.h"

Facility summer("Summer");

Facility working_week("Working week");

Facility weekend("Weekend");

Facility start_break("Start break");

Facility end_break("End break");

void YearCounter::Behavior() {
	Priority = DEFAULT_COUNTER_PRIORITY;
	echo("Staring season counter");

	long long start;
	long long end;
	while (true){

		start = processed_cars;

		// starting in another other season than summer
		Seize(summer);
		Wait(OTHER_SEASONS_TIME);

		echo("It summer");
		Release(summer);
		Wait(SUMMER_TIME);
		echo("It's winter");
		end = processed_cars;
		year(end - start);
	}
}

void WeekCounter::Behavior() {
	Priority = DEFAULT_COUNTER_PRIORITY;
	echo("Starting weekend counter");
	Seize(working_week);

	while (true){
		Seize(weekend);
		Release(working_week);
		echo("It's a week");
		Wait(WEEK_DAYS);

		echo("Weekend starting, party up!!!")
		Release(weekend);
		Seize(working_week);
		Wait(WEEKEND_DAYS);
		echo("End of weekend");
	}
}

void BreakCounter::Behavior() {
	Priority = DEFAULT_COUNTER_PRIORITY;
	echo("Starting shift counter");

	Seize(start_break);
	Seize(end_break);

	long long cars_before_shift;
	long long cars_after_shift;

	long long cars_start_day;
	long long cars_end_day;

	while (true) {
		cars_start_day = processed_cars;
		for (int i = 0; i < SHIFTS_IN_DAY; i++) {
			cars_before_shift = processed_cars;
			echo("Starting new shift");
			// At this point we have both breaks
			Wait(WORKING_TIME);

			// First short break is comming
			Release(start_break);
			echo("Starting short break");
			Wait(SHORT_BREAK);
			Seize(start_break);

			// end of first break
			Release(end_break);
			Priority = 0;
			Seize(end_break);

			Wait(WORKING_TIME);

			// First short break is comming
			Release(start_break);
			echo("Starting long break");
			Wait(LONG_BREAK);
			Seize(start_break);

			// end of first break
			Release(end_break);
			Priority = 0;
			Seize(end_break);

			Wait(WORKING_TIME);

			cars_after_shift = processed_cars;
			if (!working_week.Busy()){
				throughput_8_hours(cars_after_shift - cars_before_shift);
			}
		}
		cars_end_day = processed_cars;

		if (!working_week.Busy()){
			throughput_24_hours(cars_end_day - cars_start_day);
		}
	}

}

