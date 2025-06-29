#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "display_output.h"

#define DS18B20_FAMCODE 0x28
#define DS18S20_FAMCODE 0x10

BYTE lcdFamCodeArray[MAX_SENSORS] = {0};
uint64_t lcdRomArray[MAX_SENSORS] = {0};

void printHeaderRow()
{
	lcdGotoXY(0, 0);
	lcdPrintS("Sensor  PDROM              Temp. [C]");
}

void printSensorInfo(ROM *romArray, int sensorCount)
{
	for (int i = 0; i < MAX_SENSORS; i++)
	{
		ROM currSensor = romArray[i];
		
		lcdGotoXY(0, 1 + i);
		if (currSensor.familyCode == DS18B20_FAMCODE && lcdFamCodeArray[i] != 1)
		{
			lcdFamCodeArray[i] = 1;
			lcdPrintS("DS18B20");
		}
		else if (currSensor.familyCode == DS18S20_FAMCODE && lcdFamCodeArray[i] != 2)
		{
			lcdFamCodeArray[i] = 2;
			lcdPrintS("DS18S20");
		}
		else
		{
			lcdFamCodeArray[i] = 0;
			if (i > sensorCount)
			{
				lcdPrintS(" ");
			}
			continue;
		}
		
		uint64_t serialNumber;
		memcpy(&serialNumber, &currSensor, sizeof(ROM));
		
		if (serialNumber != lcdRomArray[i])
		{
			memcpy(&lcdRomArray[i], &currSensor, sizeof(ROM)); // LCD-Ausgabe
			
			char* temp[27];
			sprintf((char*) temp, "0x%016llx", (unsigned long long) serialNumber);
			lcdGotoXY(8, 1 + i);
			lcdPrintS((char*) temp);
		}
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
}

void clearInternalArrays()
{
	for (int i = 0; i < MAX_SENSORS; i++)
	{
		lcdFamCodeArray[i] = 0;
	}
	
	for (int i = 0; i < MAX_SENSORS; i++)
	{
		lcdRomArray[i] = 0;
	}
}
