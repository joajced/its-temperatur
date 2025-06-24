#ifndef GPIO_H
#define GPIO_H

#include "time_modul.h"

typedef uint8_t BYTE;

BYTE readBit();

BYTE readByte();

void readBytes(BYTE *data, int count);

void pushPull();

void openDrain();

void pinHigh();

void pinLow();

void setInput();

void setOutput();

#endif
