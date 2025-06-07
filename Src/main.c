#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "lcd.h"

int main()
{
	initITSboard();
	GUI_init(DEFAULT_BRIGHTNESS);
}
