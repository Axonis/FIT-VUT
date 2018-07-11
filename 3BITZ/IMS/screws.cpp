#include "screws.h"
#include "constants.h"

Store screw_errors("Screws", 0);

void OverheatedScrew::Behavior() {
	Wait(Exponential(WAIT_OVERHEATED_SCREW));
	screw_errors.SetCapacity(screw_errors.Capacity() - 1);
}
