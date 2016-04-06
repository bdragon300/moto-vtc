#ifndef DISPLAY_H
#define DISPLAY_H

/* Raw types to store data directly from data sources */
typedef struct
{
	uint8_t	seconds;
	uint8_t minutes;
	uint8_t hours; //24-hour format
	uint8_t day;
	uint8_t month;
	uint8_t year;
} Raw_time_t; // Data received directly from DS1629
typedef uint16_t Raw_temp_t; // Data received directly from DS1629
typedef uint8_t Raw_volt_t; // Data received directly from ADC

/* Data types */
typedef struct 
{
	uint8_t hours;
	uint8_t minutes;
} Time_t;

/* Functions */

/* Converts raw format to appropriate format */
Time_t raw2time (Raw_time_t* raw);
int16_t raw2temp(Raw_temp_t* raw); // Returns x*10 degrees, i.e. 25.6C will be 126
uint8_t raw2volt(Raw_volt_t* raw); // Returns x*10 volts, i.e. 12.6V will be 126

#endif