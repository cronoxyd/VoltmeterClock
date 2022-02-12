#include "resources.h"
#include "config.h"

void ClockHandler_Init() {
  return;
}

static unsigned long SecondStart = 0;

void ClockHandler_Task() {
  static int SecondMemory = 0;
  if (second() != SecondMemory) {
    SecondStart = millis();
    SecondMemory = second();
  }

  double seconds = (double) second() + ((double) (millis() - SecondStart) / 1000.0);
  double minutes = (double) minute() + (seconds / 60.0);
  double hours = (double) hour() + (minutes / 60.0);

  if (DataHandler_GetData(DATAID_CLOCKMODE)) {
    seconds = second();
    minutes = minute();
    hours = hour();
  }

  DataHandler_SetData(DATAID_HOURPWM, (hours / 24.0) * 255);
  DataHandler_SetData(DATAID_MINUTEPWM, (minutes / 60.0) * 255);
  DataHandler_SetData(DATAID_SECONDPWM, (seconds / 60.0) * 255);
  return;
}
