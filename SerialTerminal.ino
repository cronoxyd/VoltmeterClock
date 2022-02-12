#if DEBUG
#include "config.h"

String commandLine = "";
unsigned int commandIndex = 0;

void(* resetFunc) (void) = 0;

void SerialTerminal_Init() {
#if DEBUG
  Serial.begin(SERIAL_BAUD);
  Serial.print("VoltmeterClock CLI [Version ");
  Serial.print(VERSION);
  Serial.println("]");
  Serial.println("");
#endif /* DEBUG */
}

String SerialTerminal_GetNextCommandPart() {
  String retVal = "";

  do {
    retVal += commandLine[commandIndex];
    commandIndex++;
  } while (
    commandLine[commandIndex] != ' ' &&
    commandIndex < commandLine.length()
  );

  retVal.toLowerCase();
  retVal.trim();

  return retVal;
}

void SerialTerminal_Task() {
  if (!Serial.available())
    return;

  commandLine = Serial.readStringUntil('\n');
  commandIndex = 0;

  // Ignore empty command line
  if (!commandLine.length())
    return;

  String command = SerialTerminal_GetNextCommandPart();

  if (command == "get") {
    command = SerialTerminal_GetNextCommandPart();
    if (command == "time") {
      DCF77Driver_DigitalClockDisplay();
    } else if (command == "lastupdate") {
      Serial.print(DCF77Driver_GetLastUpdateAge());
      Serial.println(" ms");
    } else if (command == "clockmode") {
      Serial.print(DataHandler_GetData(DATAID_CLOCKMODE));
    } else {
      SerialTerminal_WriteInputError(command, "is not recognized as a query.");
    }
  } else if (command == "ver") {
    Serial.print("VoltmeterClock [Version ");
    Serial.print(VERSION);
    Serial.println("]");
  } else if (command == "sweep") {
    VoltmeterDriver_Reset();
    Serial.println("OK");
  } else if (command == "set") {
    String parameterName = SerialTerminal_GetNextCommandPart();
    String parameterValue = SerialTerminal_GetNextCommandPart();

    if (parameterName == "clockmode") {
      int clockMode = parameterValue.toInt();
      if (clockMode < 0 || clockMode > 1) {
        SerialTerminal_WriteInputError(parameterValue, "is not a valid CLOCKMODE value.");
      } else {
        DataHandler_SetData(DATAID_CLOCKMODE, clockMode);
        Serial.println("OK");
      }
    } else if (parameterName == "hour" || parameterName == "minute" || parameterName == "second") {
      int memHour = hour();
      int memMinute = minute();
      int memSecond = second();

      int parameterValueInt = parameterValue.toInt();
      bool valid = true;

      if (parameterName == "hour") {
        if (parameterValueInt < 0 || parameterValueInt > 23) {
          SerialTerminal_WriteInputError(parameterValue, "is not a valid MINUTE value.");
          valid = false;
        } else {
          memHour = parameterValueInt;
        }
      }

      if (parameterName == "minute") {
        if (parameterValueInt < 0 || parameterValueInt > 59) {
          SerialTerminal_WriteInputError(parameterValue, "is not a valid MINUTE value.");
          valid = false;
        } else {
          memMinute = parameterValueInt;
        }
      }

      if (parameterName == "second") {
        if (parameterValueInt < 0 || parameterValueInt > 59) {
          SerialTerminal_WriteInputError(parameterValue, "is not a valid SECOND value.");
          valid = false;
        } else {
          memSecond = parameterValueInt;
        }
      }

      if (valid) {
        setTime(memHour, memMinute, memSecond, day(), month(), year());
        Serial.println("OK");
      }
    } else {
      SerialTerminal_WriteInputError(parameterName, "is not recognized as a parameter.");
    }
  } else if (command == "save") {
    DataHandler_Save();
    Serial.println("OK");
  } else if (command == "reset") {
    DataHandler_SetData(DATAID_CLOCKMODE, 0);
    DataHandler_Save();
    Serial.println("OK");
    resetFunc();
  } else if (command == "help") {
    command = SerialTerminal_GetNextCommandPart();
    if (command == "get") {
      Serial.println(F("GET [query]\n"));
      Serial.println(F("Retrieves information about the device.\n"));
      Serial.println(F("Query parameters"));
      Serial.println(F("\tTIME\t\tDisplay the current time"));
      Serial.println(F("\tLASTUPDATE\tDisplay the age of the last DCF77 update"));
      Serial.println(F("\tCLOCKMODE\tDisplay the current mode of display (0 = smoothed, 1 = stepped)"));
    } else if (command == "help") {
      Serial.println(F("HELP [command]\n"));
      Serial.println(F("Provides information about VoltmeterClock commands."));
    } else if (command == "set") {
      Serial.println(F("SET [parameter] [value]\n"));
      Serial.println(F("Sets configuration parameters of the device.\n"));
      Serial.println(F("Parameter names"));
      Serial.println(F("\tCLOCKMODE\tSets the mode of display either to smoothed (0) or stepped (1)"));
      Serial.println(F("\tHOUR\t\tSets the hour of the internal clock (0-24)"));
      Serial.println(F("\tMINUTE\t\tSets the minute of the internal clock (0-59)"));
      Serial.println(F("\tSECOND\t\tSets the second of the internal clock (0-59)"));
    } else if (command == "sweep") {
      Serial.println(F("SWEEP\n"));
      Serial.println(F("Tests the voltmeter displays by sweeping to full scale."));
    } else if (command == "ver") {
      Serial.println(F("VER\n"));
      Serial.println(F("Displays the VoltmeterClock version"));
    } else if (command == "reset") {
      Serial.println(F("RESET\n"));
      Serial.println(F("Resets the device (including the configuration parameters)"));
    } else {
      Serial.println(F("For more information on a specific command, type HELP command-name"));
      Serial.println(F("GET\tRetrieves information about the device"));
      Serial.println(F("HELP\tProvides information about VoltmeterClock commands"));
      Serial.println(F("RESET\tResets device"));
      Serial.println(F("SAVE\tSaves all configuration parameters to non-volatile memory"));
      Serial.println(F("SET\tSets configuration parameters of the device"));
      Serial.println(F("SWEEP\tSweeps the voltmeter displays"));
      Serial.println(F("VER\tDisplays the VoltmeterClock version"));
    }
  } else {
    SerialTerminal_WriteInputError(command, "is not recognized as an command.");
  }

  Serial.println();
}

void SerialTerminal_WriteInputError(String inputValue, String message) {
  Serial.print("'");
  Serial.print(inputValue);
  Serial.print("' ");
  Serial.println(message);
}

#endif /* DEBUG */
