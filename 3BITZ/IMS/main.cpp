#include <iostream>
#include <simlib.h>
#include <getopt.h>
#include "counters.h"
#include "constants.h"
#include "line.h"
#include "stats.h"

int backup_screw = 0;
int store_prediction = 0;
int no_errors = 0 ;
int shorter_time = 0;

int screw_errors_count = 0;
int screw_overheated = 0;
int missing_part = 0;

void arg_parser(int argc, char **argv);

int main(int argc, char **argv) {

	arg_parser(argc, argv);

	SetCalendar("cq");
	Init(0, 60*60*24*365*2);

	(new YearCounter)->Activate();
	(new WeekCounter)->Activate();
	(new BreakCounter)->Activate();

	(new CarGenerator)->Activate();

	(new MainLineProc)->Activate();


	// Defining cars for init()
	for (int i = 0 ; i < INIT_CARS; i++){
		(new Car)->Activate();
	}

	Run();


	throughput_8_hours.Output();
	throughput_24_hours.Output();
	year.Output();
	queue_of_cars.Output();

	std::cout << "While this program running:\n";
	std::cout << "\tMissing parts : " << missing_part << "\n";
	std::cout << "\tOverheated screws : " << screw_overheated << "\n";
	std::cout << "\tBroken screws : "  << screw_errors_count << "\n";

	return 0;
}

void arg_parser(int argc, char **argv){
	int c;

	while ((c = getopt(argc, argv, "bpes")) != -1){
		switch (c) {
			case 'b':
				backup_screw++;
				break;

			case 'p':
				store_prediction++;
				break;

			case 'e':
				no_errors++;
				break;

			case 's':
				shorter_time++;
				break;

			default:
				std::cout << "This is simulation of car line.\n"
						"\t-s shorter time priod for car\n"
						"\t -b enable backup screw\n"
						"\t -e no errors coused by summer\n"
						"\t -p enable prediction in store\n";
				abort();
		}
	}
}
