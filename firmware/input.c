#include "input.h"

uint8_t ticks_counter = 0;
uint8_t event_ticks = 0; //ticks_counter value when any button event occured
Input_mode_t input_mode = IDLE;


void
input_init()
{
	DEBUG1('>', "input_init");

    INPUT_DDR &= ~(_BV(CHARGE_PIN) | _BV(BUTTON_PIN));

    uint8_t t = INPUT_DDR;

    DEBUGHEX1('I', &t, 1);
    DEBUG1('<', "input_init")
}

void
input_tick(void)
{
	++ticks_counter;
	uint8_t btn_state = get_button_state();

    switch (input_mode) {
		case IDLE:
			if (btn_state) {
				_set_input_mode(PRESS);
			}
			break;

		case PRESS:
			if (btn_state) {
				if (ticks_counter - event_ticks == HOLD_THRESHOLD) {
					_set_input_mode(HOLD);
				}
			}
			else {
				_set_input_mode(CLICK);
			}
			break;

		case CLICK:
			//Ignore any input while CLICK_GAP ticks reached
			if (ticks_counter - event_ticks == CLICK_GAP) {
				_set_input_mode(IDLE);
			}
			break;

		case HOLD:
			if (btn_state) {
				if (ticks_counter - event_ticks == HOLD_EVENT_INTERVAL) {
					set_input_mode(HOLD);
				}
			}
			else {
				_set_input_mode(RELEASE);
			}
			break;

		case RELEASE:
			if (ticks_counter - event_ticks == CLICK_GAP) {
				_set_input_mode(IDLE);
			}
			break;
	}
}

inline uint8_t
get_volts(void)
{
	DEBUG1('>', "get_volts");

    ADCSRA |= _BV(ADSC); //Start the conversion
    while (ADCSRA & _BV(ADIF)); //Wait until it complete

    uint8_t raw = ADCL;
    raw = ADCH;
    ADCSRA |= _BV(ADIF); //Clear ADIF bit by write one to it

    uint8_t res = uint16_t(raw * 7) / 10 + 1; //180*res/255

    DEBUGHEX1('=', &res, 1);
	DEBUG1('<', "get_volts");

	return res;
}


static void
_set_input_mode(Input_mode_t mode)
{
	DEBUG2('>', "_set_input_mode");
	DEBUGHEX2('(', &mode, sizeof(Input_mode_t));

	input_mode = mode;
	event_ticks = ticks_counter;

	//Do callback
	if ( (&input_callbacks)[mode] )
	{
		(&input_callbacks)[mode]();
	}

	DEBUG2('<', "_set_input_mode");
}
