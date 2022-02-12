#include "config.h"
#include "TimeLib.h"
time_t time;

void setup() {
#if DEBUG
  SerialTerminal_Init();
#endif /* DEBUG */
  DataHandler_Init();
  DCF77Driver_Init();
  ClockHandler_Init();
  VoltmeterDriver_Init();
  LEDDriver_Init();
  return;
}

void loop() {
#if DEBUG
  SerialTerminal_Task();
#endif /* DEBUG */
  DCF77Driver_Task();
  ClockHandler_Task();
  VoltmeterDriver_Task();
  LEDDriver_Task();
  return;
}
