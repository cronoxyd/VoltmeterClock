#include "resources.h"
#include "config.h"
#include <EEPROM.h>

static int DataHandlerBuffer[] = {
  0,		/* DATAID_HOURPWM */
  0,		/* DATAID_MINUTEPWM */
  0,		/* DATAID_SECONDPWM */
  0,		/* DATAID_DCFINSYNC */
  0     /* DATAID_CLOCKMODE */
};

void DataHandler_Init() {
  // Read data from EEPROM
  for (int i = 0; i < EEPROMDATA_SIZE; i++) {
    int eepromVal = 0;
    EEPROM.get(i * sizeof(int), eepromVal);
    DataHandler_SetData(RAMDATA_SIZE + i, eepromVal);
  }
}

void DataHandler_Save() {
  for (int i = 0; i < EEPROMDATA_SIZE; i++) {
    int eepromVal = DataHandler_GetData(RAMDATA_SIZE + i);
    EEPROM.put(i * sizeof(int), eepromVal);
  }  
}

void DataHandler_SetData(unsigned int ID, int Data) {
  if (DataHandler_IDValid(ID)) {
    DataHandlerBuffer[(int)ID] = Data;
  }
  return;
}

int DataHandler_GetData(unsigned int ID) {
  if (DataHandler_IDValid(ID)) {
    return DataHandlerBuffer[ID];
  }
  return 0;
}

int DataHandler_IDValid(unsigned int ID) {
  return ID < (RAMDATA_SIZE + EEPROMDATA_SIZE);
}

void DataHandler_ModifyData(unsigned int ID, int Modifier) {
  DataHandler_SetData(ID, DataHandler_GetData(ID) + Modifier);
  return;
}

void DataHandler_WrapData(unsigned int ID, int Min, int Max) {
  if (Min < Max) {
    if (DataHandler_GetData(ID) < Min) {
      DataHandler_SetData(ID, Max);
    }
    if (DataHandler_GetData(ID) > Max) {
      DataHandler_SetData(ID, Min);
    }
  }
  return;
}

void DataHandler_ConstrainData(unsigned int ID, int Min, int Max) {
  if (Min < Max) {
    if (DataHandler_GetData(ID) < Min) {
      DataHandler_SetData(ID, Min);
    }
    if (DataHandler_GetData(ID) > Max) {
      DataHandler_SetData(ID, Max);
    }
  }
  return;
}
