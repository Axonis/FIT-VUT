#include "stats.h"

Stat throughput_8_hours;
Stat throughput_24_hours;
Stat year;
Histogram queue_of_cars("Number of cars in queu",0,1,15);