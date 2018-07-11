#ifndef CONSTANTS
#define CONSTANTS

#define DEFAULT_COUNTER_PRIORITY	2

#define INIT_CARS			7

#define OTHER_SEASONS_TIME	23328000 	// 9*30*24*60*60
#define SUMMER_TIME 		8208000		// 365*24*60*60 - other seasons

#define WEEK_DAYS			432001		// 5*24*60*60
#define WEEKEND_DAYS		172799		// 2*24*60*60

#define WORKING_TIME		9000		// 2.5*60*60
#define SHORT_BREAK			600			// 10*60
#define LONG_BREAK			1200		// 20*60


/*
 * Monitor checking times
 */
#define MONITOR_CHECKING_NO_ERROR		15
#define MONITOR_CHECKING_SCREW_ERROR	45


/*
 * Thresholds for randomization
 */

#define SCREW_IS_BROKEN					(1/2000.0)
#define PART_IS_MISSING					(1/600.0)
#define PART_IS_NOT_IN_STORE			(1/100.0)

#define SCREW_IS_OVERHEATED				(1/900.0)


#define CAR_GEN_LOW						51
#define CAR_GEN_HIGH					60


#define WAIT_IN_STORE					240       // 4*60
#define WAIT_NOT_IN_STORE_LOW			600      // 10*60
#define WAIT_NOT_IN_STORE_HIGH			1200      // 20*60

#define WAIT_STD                        55
#define WAIT_SHORTER					50

#define WAIT_BROKEN_SCREW_LOW					53
#define WAIT_BROKEN_SCREW_HIGH					75

#define WAIT_OVERHEATED_SCREW			900		// 15*60


#endif //CONSTANTS
