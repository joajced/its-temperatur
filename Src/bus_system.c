#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "time_modul.h"
#include "gpio.h"
#include "bus_system.h"

bool reset()
{
	// Reset pulse from master
	pinLow();
	sleep_us(480);
	
	// Presence pulse from slaves (Busabfrage)
	pinHigh();
	setInput();
	sleep_us(70); // 60-240 us warten
	bool present = !(GPIOD->IDR & 0x01);
	
	sleep_us(410);
	setOutput();
	
	return present;
}

void writeCommand(BYTE command)
{
	for (int i = 0; i < 8; i++)
	{
		command & 0x01 ? writeHigh() : writeLow();
		command >>= 1;
	}
}

void writeHigh()
{	
	pinLow();
	sleep_us(6);
	pinHigh();
	sleep_us(64);
}

void writeLow()
{
	pinLow();
	sleep_us(60);
	pinHigh();
	sleep_us(10);
}

void readRom(BYTE *rom)
{
	readBytes(rom, sizeof(ROM));
}

void writeRom(BYTE *rom)
{
	for (int i = 0; i < 8; i++)
	{
		writeCommand(rom[i]);
	}
}

void readScratchpad(BYTE *scratch)
{
	readBytes(scratch, sizeof(SCRATCHPAD));
}

bool checkCrc(BYTE *data, int size)
{
	BYTE crc = 0;
	
	for (int i = 0; i < size - 1; i++)
	{
		BYTE byte = data[i];
		
		for (int j = 0; j < 8; j++)
		{
			BYTE mix = (crc ^ byte) & 0x01;
			crc >>= 1;
			
			if (mix)
			{
				crc ^= 0x8c;
			}
			byte >>= 1;
		}
	}
	
	return (crc == data[size - 1]);
}
