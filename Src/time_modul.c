#include "time_modul.h"

void sleep_us(uint32_t delay)
{
	uint32_t time1 = TIM2->CNT;
	delay *= 90;
	
	while ((TIM2->CNT - time1) <= delay);
}
