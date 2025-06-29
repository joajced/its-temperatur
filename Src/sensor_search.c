#include <string.h>
#include "sensor_search.h"
#include "bus_system.h"
#include "display_output.h"

#define FALSE 0
#define TRUE  1

// Global search state
BYTE tempRom[8];
int lastDiscrepancy;
int lastDeviceFlag = FALSE;
BYTE crc8;

int owFirst()
{
	// Reset the search state
	lastDiscrepancy = 0;
	lastDeviceFlag = FALSE;
	
	return owSearch();
}

int owNext()
{
	// Leave the search state alone
	return owSearch();
}

int owSearch()
{
	int id_bit_number;
	int last_zero, rom_byte_number, search_result;
	int id_bit, cmp_id_bit;
	BYTE rom_byte_mask, search_direction;
	
	// Initialize for search
	id_bit_number = 1;
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;
	
	// If the last call was not the last one
	if (!lastDeviceFlag)
	{
		// 1-Wire reset
		if (!reset())
		{
			// Reset the search
			lastDiscrepancy = 0;
			lastDeviceFlag = FALSE;
			return FALSE;
		}
		
		// Issue the search command
		writeCommand(SEARCH_ROM);
		
		// Loop to do the search
		do
		{
			// Read a bit and its complement
			id_bit = readBit();
			cmp_id_bit = readBit();
			
			// Check for no devices on 1-wite
			if ((id_bit == 1) && (cmp_id_bit == 1))
			{
				printError(NO_SENSORS, 0);
				break;
			}
			else
			{
				// All devices coupled have 0 or 1
				if (id_bit != cmp_id_bit)
				{
					search_direction = id_bit;
					// Bit write value for search
				}
				else
				{
					// If this discrepancy if before the lastDiscrepancy
					// on a previous next then pick the same as the last time
					if (id_bit_number < lastDiscrepancy)
					{
						search_direction = ((tempRom[rom_byte_number] & rom_byte_mask) > 0);
					}
					else
					{
						// If equal to last pick 1, if not then pick 0
						search_direction = (id_bit_number == lastDiscrepancy);
					}
					
					// If 0 was picked then record its position in last_zero
					if (search_direction == 0)
					{
						last_zero = id_bit_number;
					}
				}
			}
			
			// Set or clear the bit in the ROM byte with mask
			if (search_direction == 1)
			{
				tempRom[rom_byte_number] |= rom_byte_mask;
			}
			else
			{
				tempRom[rom_byte_number] &= ~rom_byte_mask;
			}
			
			// Serial number search direction write bit
			search_direction ? writeHigh() : writeLow();
			
			// Increment the byte counter id_bit_number and shift the mask
			id_bit_number++;
			rom_byte_mask <<= 1;
			
			// If the mask is 0 then go to new SerialNum byte and reset mask
			if (rom_byte_mask == 0)
			{
				// docrc8
				rom_byte_number++;
				rom_byte_mask = 1;
			}
			
		}
		while (rom_byte_number < 8); // Loop through ROM bytes 0-7
		
		// If the search was successful
		if (!(id_bit_number < 65))
		{
			// Search successful
			lastDiscrepancy = last_zero;
			
			// Check for last device
			if (lastDiscrepancy == 0)
			{
				lastDeviceFlag = TRUE;
			}
			
			search_result = TRUE;
		}
	}
	
	// If no device found then reset counters so next
	// search will be like a first
	if (!search_result || !tempRom[0])
	{
		lastDiscrepancy = 0;
		lastDeviceFlag = FALSE;
		search_result = FALSE;
	}
	
	return search_result;
}

void copyResult(BYTE* rom)
{
	memcpy(rom, tempRom, sizeof(ROM));
}
