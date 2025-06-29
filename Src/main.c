#include "stm32f4xx_hal.h"
#include <string.h>
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include "timer.h"
#include "gpio.h"
#include "bus_system.h"
#include "display_output.h"
#include "sensor_search.h"

// Hard coded ROMs
#define SENSOR1 (ROM) { 0x28, {0xf2, 0x54, 0x88, 0x0d, 0x00, 0x00}, 0x5d } // 0x5d00000d8854f228
#define SENSOR2 (ROM) { 0x28, {0x5b, 0xf5, 0x86, 0x0d, 0x00, 0x00}, 0x7c } // 0x7c00000d86f55b28
#define SENSOR3 (ROM) { 0x28, {0x4b, 0xe3, 0x88, 0x0d, 0x00, 0x00}, 0xd2 } // 0xd200000d88e34b28
#define SENSOR4 (ROM) { 0x28, {0x3f, 0x84, 0x88, 0x0d, 0x00, 0x00}, 0xd6 } // 0xd600000d88843f28

int main()
{
	initITSboard();
	GUI_init(DEFAULT_BRIGHTNESS);
	initTimer();
	
	GPIOD->BSRR = (0x01 << 1); // PD1 auf 3,3 V
	openDrain();							 // PD0 auf Open Drain setzen
	pinHigh();								 // PD0 auf 3,3 V
	printHeaderRow();          // Tabellenkopf wird ausgedruckt
	
	while (1)
	{
		// 1. Ermittlung der Sensoren
		
		if (!reset())
		{
			printError(NO_SENSORS, 0);
			continue;
		}
		
		ROM romArray[MAX_SENSORS];
		int sensorCount = 0;
		
		clearInternalArrays();
		int res = owFirst();
		if (res)
		{
			copyResult((BYTE*) &romArray[sensorCount]);
			sensorCount++;
		}
		while (res)
		{
			if (res = owNext()) copyResult((BYTE*) &romArray[sensorCount]);
			sensorCount++;
		}
		
		printSensorInfo(&romArray, sensorCount);
		
		// 2. Messung durchfuehren
		
		SCRATCHPAD scratchArray[MAX_SENSORS];
		
		reset();
		writeCommand(SKIP_ROM); // Alle Sensoren gleichzeitig ansprechen
		writeCommand(CONVERT_T);
		
		pushPull();
		sleep_us(750000);
		openDrain();
		
		// 3. Sensoren auslesen und Temperatur ausdrucken
		
		for (int i = 0; i < sensorCount; i++)
		{
			ROM currSensor = romArray[i];
			
			reset();
			writeCommand(MATCH_ROM);
			writeRom((BYTE*) &currSensor);
			writeCommand(READ_SCRATCHPAD);
			readScratchpad((BYTE*) &scratchArray[i]);
			
			if (checkCrc((BYTE*) &scratchArray[i], 9))
			{
				printTemp(&scratchArray[i], i);
			}
			else
			{
				printError(CRC_MISMATCH, i);
				continue;
			}
		}
	}
	
	return 0;
}
