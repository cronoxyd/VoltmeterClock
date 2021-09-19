#include "resources.h"
#include "config.h"
	
int PWMPins[3] = {
	PIN_HOURMETER,
	PIN_MINUTEMETER,
	PIN_SECONDMETER
};

void VoltmeterDriver_Init() {	
	for (unsigned int i = 0; i < 3; i++) {
		pinMode(PWMPins[i], OUTPUT);
	}
	return;
}

#define VOLTMETERDRIVER_START 0
#define VOLTMETERDRIVER_RUN 1

void VoltmeterDriver_Task() {
	static unsigned int State = 0;
	
	switch (State) {
		case VOLTMETERDRIVER_START: {
			if (VoltmeterDriver_Sweep()) State = VOLTMETERDRIVER_RUN;
			break;
		}
		case VOLTMETERDRIVER_RUN: {
			VoltmeterDriver_Display();
			break;
		}
	}
	return;
}

unsigned int VoltmeterDriver_Sweep() {
	static unsigned long Timer = 0;
	static int Value = 0;
	
	if ((millis() - Timer) < 5) return 0;
	Timer = millis();
	
	unsigned short sweepPWM = (abs(Value - 255) * -1 + 255);
	
	for (unsigned int i = 0; i < 3; i++) {
		analogWrite(PWMPins[i], sweepPWM);
	}
	
	Value++;
	
	return (Value > 510);
}

void VoltmeterDriver_Display() {
	static unsigned long Timer = 0;
	
	if (millis() - Timer < 3) return;
	Timer = millis();
	
	int RequestedPWM[3] = {
		DataHandler_GetData(DATAID_HOURPWM),
		DataHandler_GetData(DATAID_MINUTEPWM),
		DataHandler_GetData(DATAID_SECONDPWM)
	};
	
	static int CurrentPWM[3];
	
	for (unsigned int i = 0; i < 3; i++) {
		int Diff = RequestedPWM[i] - CurrentPWM[i];
		
		if (abs(Diff) == 0) {
			CurrentPWM[i] = RequestedPWM[i];
		} else {
			CurrentPWM[i] += sign(Diff);
		}
		
		analogWrite(PWMPins[i], CurrentPWM[i]);
	}
	return;
}

int sign(int x) {
	if (x < 0) return -1;
	if (x > 0) return 1;
	return 0;
}