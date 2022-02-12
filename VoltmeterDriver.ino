#include "resources.h"
#include "config.h"

#define VOLTMETERDRIVER_START 0
#define VOLTMETERDRIVER_RUN 1

int PWMPins[3] = {
  PIN_HOURMETER,
  PIN_MINUTEMETER,
  PIN_SECONDMETER
};

static unsigned int VoltmeterDriver_State = 0;

void VoltmeterDriver_Init() {
  for (unsigned int i = 0; i < 3; i++) {
    pinMode(PWMPins[i], OUTPUT);
  }
  return;
}

void VoltmeterDriver_Task() {
  switch (VoltmeterDriver_State) {
    case VOLTMETERDRIVER_START: {
        if (VoltmeterDriver_Sweep()) VoltmeterDriver_State = VOLTMETERDRIVER_RUN;
        break;
      }
    case VOLTMETERDRIVER_RUN: {
        VoltmeterDriver_Display();
        break;
      }
  }
  return;
}

void VoltmeterDriver_Reset() {
  VoltmeterDriver_State = VOLTMETERDRIVER_START;
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

  if (Value > 510) {
    Value = 0;
    return 1;
  } else {
    return 0;
  }
}

void VoltmeterDriver_Display() {
  static unsigned long Timer = 0;

  if (millis() - Timer < 3)
    return;

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
