#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

/* Types */
enum {TIME, VOLT, TEMP, INIT} Display_mode_t;
typedef struct 
{
	uint8_t digits[4];
	uint8_t aux; //Auxiliary segments like dots, colons
} Display_data_t;

// Show information on display and set display mode
void show_time(Time_t data);
void show_temp(int8_t data);
void show_volt(uint8_t data);

// Adds to display data additional indication like mode or charge
void show_aux();

void render_digit(uint8_t offset);

uint8_t get_segments(uint8_t number, uint8_t display_0);

#endif