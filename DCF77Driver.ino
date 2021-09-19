#include "DCF77.h"
#include "TimeLib.h"
#include "config.h"

DCF77 DCF = DCF77(PIN_DCF77SIGNAL, DCF_INT);

void DCF77Driver_Init() {
	#if DEBUG
	Serial.begin(SERIAL_BAUD);
	#endif /* DEBUG */
	
	DCF.Start();
	
	#if DEBUG
	Serial.println("Waiting for DCF77 time ... ");
	Serial.println("It will take at least 2 minutes until a first update can be processed.");
	#endif /* DEBUG */
	return;
}

void DCF77Driver_Task() {
	unsigned long Timer = 0;
	unsigned long LastDCFSync = 0;
	
	DataHandler_SetData(DATAID_DCFINSYNC, ((millis() - LastDCFSync) < DCF_MAXDESYNCTIME) && LastDCFSync != 0);
	
	if ((millis() - Timer) < DCF_REFRESH) return;
	Timer = millis();
	
	time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
	
	if (DCFtime != 0) {
		#if DEBUG
		Serial.println("Time is updated");
		#endif /* DEBUG */
		setTime(DCFtime);
		LastDCFSync = millis();
	}
	
	#if DEBUG
	if (millis() % 1000 == 0) {
		digitalClockDisplay();			
	}
	#endif /* DEBUG */
	return;
}

#if DEBUG
void digitalClockDisplay(){
	// digital clock display of the time
	Serial.print(hour());
	printDigits(minute());
	printDigits(second());
	Serial.print(" ");
	Serial.print(day());
	Serial.print(" ");
	Serial.print(month());
	Serial.print(" ");
	Serial.print(year()); 
	Serial.println(); 
	return;
}

void printDigits(int digits){
	// utility function for digital clock display: prints preceding colon and leading 0
	Serial.print(":");
	if(digits < 10)
		Serial.print('0');
	Serial.print(digits);
	return;
}
#endif /* DEBUG */