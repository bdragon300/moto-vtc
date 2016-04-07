#include "input.h"
#include <avr/io.h>

uint8_t ticks_counter = 0;
Input_mode_t input_mode = IDLE;

void
button_timer_tick(void)
{
	uint8_t prev_counter = ticks_counter;
	ticks_counter = (get_button_state()) ? ticks_counter++ : 0;

	if ( // Button just released after short press
		prev_counter
		&& prev_counter < PRESS_TIMEOUT 
		&& ticks_counter == 0) 
	{
		input_mode = CLICK;
	}
	else if (ticks_counter == 0) //IDLE
	{
		input_mode = IDLE;
	}
	else if (ticks_counter >= PRESS_TIMEOUT) // Button still pressing for a long time
	{
		input_mode = LONG_PRESS;
	}
}

inline Input_mode_t
get_input_mode()
{
	return ticks_counter;
}

inline uint8_t
get_button_state(void)
{
	return (INPUT_PIN & _BV(BUTTON_PIN)) != 0;
}