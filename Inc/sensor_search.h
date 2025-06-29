#ifndef SENSOR_SEARCH_H
#define SENSOR_SEARCH_H

#include <stdbool.h>
#include "gpio.h"
#include "timer.h"

int owFirst();

int owNext();

int owSearch();

void copyResult(BYTE* rom);

#endif
