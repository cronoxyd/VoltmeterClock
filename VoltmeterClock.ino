#include "TimeLib.h"
time_t time;

void setup() {
	DCF77Driver_Init();
	ClockHandler_Init();
	VoltmeterDriver_Init();
	LEDDriver_Init();
	return;
}

void loop() {
	DCF77Driver_Task();
	ClockHandler_Task();
	VoltmeterDriver_Task();
	LEDDriver_Task();
	return;
}