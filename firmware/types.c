#include "types.h"

Time_t 
raw2time (Raw_time_t* raw)
{
	Time_t res = {0};
	res.minutes = ((raw->minutes & 0b01110000) >> 4) * 10 + (raw->minutes & 0b00001111);
	res.hours = ((raw->minutes & 0b00010000) >> 4) * 10 + (raw->hours & 0b00001111);
	return res;
}

int16_t 
raw2temp(Raw_temp_t* raw)
{
	int8_t h = *raw >> 8;
	return ((int16_t)(*h) * 10) + (*raw & 0x0080) * 5;
}

uint8_t 
raw2volt(Raw_volt_t* raw)
{
	//TODO:
	return 12;
}