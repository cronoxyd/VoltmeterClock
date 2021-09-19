#include "resources.h"
#include "config.h"

void LEDDriver_Init() {
	pinMode(PIN_LEDFLASH, OUTPUT);
	pinMode(PIN_LEDDCFSTATUS, OUTPUT);
	return;
}

void LEDDriver_Task() {
	LEDDriver_HandleFlash();
	LEDDriver_HandleDCFStatus();
}

#define FLASHSTATE_STANDBY 0
#define FLASHSTATE_FLASH 1

void LEDDriver_HandleFlash() {
	static unsigned int State = FLASHSTATE_STANDBY;	
	static int hourMemory = 0;
	
	static unsigned long FlashTimer = 0;
	
	switch (State) {
		case FLASHSTATE_STANDBY: {
			digitalWrite(PIN_LEDFLASH, LOW);
			
			FlashTimer = millis();
			
			if (hour() != hourMemory) State = FLASHSTATE_FLASH;
			break;
		}
		
		case FLASHSTATE_FLASH: {
			unsigned long FlashTime = (millis() - FlashTimer);
			digitalWrite(PIN_LEDFLASH, ((FlashTime / LED_FLASHINTERVAL) % 2) != 0);
			if (FlashTime / (LED_FLASHINTERVAL * 2) >= LED_FLASHCOUNT) State = FLASHSTATE_STANDBY;
			break;
		}
	}
	return;	
}

void LEDDriver_HandleDCFStatus() {
	digitalWrite(PIN_LEDDCFSTATUS, !DataHandler_GetData(PIN_LEDDCFSTATUS));
}