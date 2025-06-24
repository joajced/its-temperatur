#ifndef BUS_SYSTEM_H
#define BUS_SYSTEM_H

#include <stdbool.h>
#include "gpio.h"
#include "timer.h"

// ROM-Befehle
#define READ_ROM    0x33
#define MATCH_ROM   0x55
#define SKIP_ROM    0xCC
#define SEARCH_ROM  0xF0 // "Normal" search, all devices participate
// Funktionsbefehle
#define CONVERT_T       0x44
#define READ_SCRATCHPAD 0xBE

#define MAX_SENSORS 4

typedef struct __attribute__((packed)) ROM
{
	BYTE familyCode;
	BYTE serialNumber[6];
	BYTE crc;
} ROM;

typedef struct __attribute__((packed)) SCRATCHPAD
{
	BYTE tempLsb;
	BYTE tempMsb;
	BYTE th;
	BYTE tl;
	BYTE config;
	BYTE reserved[3];
	BYTE crc;
} SCRATCHPAD;

bool reset();

void writeCommand(BYTE command);

void writeHigh();

void writeLow();

void readRom(BYTE *rom);

void writeRom(BYTE *rom);

void readScratchpad(BYTE *scratch);

bool checkCrc(BYTE *data, int size);

#endif
