#ifndef DISPLAY_OUTPUT_H
#define DISPLAY_OUTPUT_H

#include "bus_system.h"

// Fehlermeldungen
#define NO_SENSORS   "FEHLER: Keine Sensoren    "
#define CRC_MISMATCH "FEHLER: Datenkorruption   "

void printHeaderRow();

void printSensorInfo(ROM *rom, int sensorCount);

void printTemp(SCRATCHPAD *scratch, int index);

void printError(char *msg, int index);

void clearInternalArrays();

#endif
