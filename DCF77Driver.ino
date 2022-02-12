#include "DCF77.h"
#include "TimeLib.h"
#include "config.h"

DCF77 DCF = DCF77(PIN_DCF77SIGNAL, DCF_INT);

unsigned long lastTimeUpdate = 0;

void DCF77Driver_Init() {
  DCF.Start();
  return;
}

void DCF77Driver_Task() {
  unsigned long Timer = 0;
  unsigned long LastDCFSync = 0;

  DataHandler_SetData(DATAID_DCFINSYNC, ((millis() - LastDCFSync) < DCF_MAXDESYNCTIME) && LastDCFSync != 0);

  if ((millis() - Timer) < DCF_REFRESH)
    return;
  
  Timer = millis();

  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available

  if (DCFtime != 0) {
    lastTimeUpdate = millis();
    setTime(DCFtime);
    LastDCFSync = millis();
  }
  return;
}

#if DEBUG
unsigned long DCF77Driver_GetLastUpdateAge() {
  return millis() - lastTimeUpdate;
}

void DCF77Driver_DigitalClockDisplay() {
  // digital clock display of the time
  Serial.print(year());
  Serial.print("-");
  Serial.print(month());
  Serial.print("-");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(hour());
  DCF77Driver_PrintDigits(minute());
  DCF77Driver_PrintDigits(second());
  Serial.println();
  return;
}

void DCF77Driver_PrintDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
  return;
}
#endif /* DEBUG */
