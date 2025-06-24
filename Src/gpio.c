#include "gpio.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "time_modul.h"
#include "bus_system.h"

BYTE readBit()
{
	pinLow();
	sleep_us(6);
	
	pinHigh();
	setInput();
	sleep_us(9);
	
	BYTE res = GPIOD->IDR & 0x01;
	sleep_us(55);
	setOutput();
	
	return res;
}

BYTE readByte()
{
	BYTE res = 0;
	for (int i = 0; i < 8; i++)
	{
		res |= readBit() << i;
	}
	return res;
}

void readBytes(BYTE *data, int count)
{
	for (int i = 0; i < count; i++)
	{
		data[i] = readByte();
	}
}

void pushPull()
{
	GPIOD->OTYPER &= ~0x01;
}

void openDrain()
{
	GPIOD->OTYPER |= 0x01;
}

void pinHigh()
{
	GPIOD->BSRR = 0x01;
}

void pinLow()
{
	GPIOD->BSRR = 0x01 << 16;
}

void setInput()
{
	GPIOD->MODER &= ~0x03;
}

void setOutput()
{
	GPIOD->MODER = (GPIOD->MODER & ~0x03) | 0x01;
}
