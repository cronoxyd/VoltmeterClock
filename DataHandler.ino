#include "resources.h"
#include "config.h"

static int RAMData[] = {
	0,		/* DATAID_HOURPWM */
    0,		/* DATAID_MINUTEPWM */
    0,		/* DATAID_SECONDPWM */
	
	0		/* DATAID_DCFINSYNC */
};

void DataHandler_SetData(unsigned int ID, int Data) {
	if (DataHandler_IDValid(ID)) {
		RAMData[ID] = Data;
	}
	return;
}

int DataHandler_GetData(unsigned int ID) {
	if (DataHandler_IDValid(ID)) {
		return RAMData[ID];
	}
	return 0;
}

int DataHandler_IDValid(unsigned int ID) {
	return ID < RAMDATA_SIZE;
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