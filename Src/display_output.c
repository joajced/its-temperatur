#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "display_output.h"

#define DS18B20_FAMCODE 0x28
#define DS18S20_FAMCODE 0x10

char sensorArray[MAX_SENSORS][27] = {0};

void printHeaderRow()
{
	lcdGotoXY(0, 0);
	lcdPrintS("Sensor  PDROM              Temp. [C]");
}

void printSensorInfo(ROM *rom, int index)
{
	char line[27];
	char* familyCode = "UNKNOWN";
	
	if (rom->familyCode == DS18B20_FAMCODE)
	{
		familyCode = "DS18B20";
	}
	else if (rom->familyCode == DS18S20_FAMCODE)
	{
		familyCode = "DS18S20";
	}
	else
	{
		strcpy(sensorArray[index], " ");
		lcdGotoXY(0, 1 + index);
		lcdPrintS(" ");
		return;
	}
	
	uint64_t serialNumber;
  memcpy(&serialNumber, rom, sizeof(ROM));
	
	sprintf(line, "%s 0x%016llx", familyCode, (unsigned long long) serialNumber);
	
	// Wenn die Strings unterschiedlich sind
	if (strcmp(line, sensorArray[index]))
	{
		strcpy(sensorArray[index], line);
		lcdGotoXY(0, 1 + index);
		lcdPrintS(line);
	}
}

void printTemp(SCRATCHPAD *scratch, int index)
{
	char tempString[10];
	
	int16_t rawTemp = scratch->tempLsb + (scratch->tempMsb << 8);
	double tempCelsius = rawTemp * 0.0625;
	sprintf(tempString, "%.4f", tempCelsius);
	
	lcdGotoXY(27, 1 + index);
	lcdPrintS(tempString);
}

void printError(char *msg, int index)
{
	// Wenn die Strings unterschiedlich sind
	if (strcmp(msg, sensorArray[index]))
	{
		strcpy(sensorArray[index], msg);
		lcdGotoXY(0, 1 + index);
		lcdPrintS(msg);
		
		// Alle anderen Zeilen loeschen
		if (!strcmp(msg, NO_SENSORS))
		{
			for (int i = 0; i < MAX_SENSORS - 2; i++)
			{
				strcpy(sensorArray[1 + i], " ");
				lcdGotoXY(0, 2 + i);
				lcdPrintS(" ");
			}
		}
	}
}
