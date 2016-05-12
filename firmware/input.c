#include "input.h"

uint8_t ticks_counter = 0;
Input_mode_t input_mode = IDLE;

void
input_init()
{
    INPUT_DDR &= ~(_BV(CHARGE_PIN) | _BV(BUTTON_PIN));
}

void
input_tick(void)
{
    uint8_t prev_counter = ticks_counter;
    if (get_button_state()) {
    	if (ticks_counter < 255) {
    		ticks_counter++;
    	}
    } else {
    	ticks_counter = 0;
    }

    if (prev_counter
        && prev_counter < PRESS_TIMEOUT 
        && ticks_counter == 0) 
    {
    	// Button just released after short press
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
    return input_mode;
}

inline uint8_t
get_button_state(void)
{
    return (INPUT_PIN & _BV(BUTTON_PIN)) != 0;
}

inline uint8_t
get_volts(void)
{
    ADCSRA |= _BV(ADSC); //Start the conversion
    while (ADCSRA & _BV(ADIF)); //Wait until it complete

    uint8_t raw = ADCL;
    raw = ADCH;
    ADCSRA |= _BV(ADIF); //Clear ADIF bit by write one to it

    return uint16_t(raw * 7) / 10 + 1; //180*res/255
}

inline uint8_t
get_charge(void)
{
    return (INPUT_PIN & _BV(CHARGE_PIN)) != 0;
}
