#ifndef INPUT_H
#define INPUT_H

#include "config_application.h"
#include "uartdebug/uartdebug.h"
#include <avr/io.h>
#include <avr/stdint.h>

/*
 * Button input mode
 */
typedef enum {
    IDLE,       //Button released
	PRESS,      //Button is pressing now but less then HOLD_THRESHOLD ticks
    CLICK,      //User clicked the button (pressed and released)
    HOLD,       //User is holding button longer then HOLD_THRESHOLD ticks
    RELEASE     //User just released button after HOLD mode
} Input_mode_t;

/*
 * Callbacks that will be called on appropriate input event
 * Pointers arranged in order accordignly Input_mode_t modes
 */
extern struct {
	void (*on_button_idle)(void);
	void (*on_button_press)(void);
	void (*on_button_click)(void);
	void (*on_button_hold)(void);
	void (*on_button_release)(void);
} input_callbacks = {0};

/*
 * Initialization
 */
void
input_init();

/*
 * Calls every time tick from main code
 * Uses to count time for input mode switch
 */
void
input_tick(void);


/*
 * Returns current input mode
 * CLICK mode retuns just after one tick when
 */
inline Input_mode_t
get_input_mode()
{
	return input_mode;
}


/*
 * Current button state. 0|1
 */
inline uint8_t
get_button_state(void)
{
	return (INPUT_PIN & _BV(BUTTON_PIN)) != 0;
}

/*
 * Starts ADC convert and waits until it will finished
 * Returns actual voltage * 10 with accuracy 0.1, i.e. 134 means 13.4 volts
 */
inline uint8_t
get_volts(void);

/*
 * Returns battery charge status. 0|1
 */
inline uint8_t
get_charge(void)
{
	return (INPUT_PIN & _BV(CHARGE_PIN)) != 0;
}



/*
 * Utilities
 */
static void
_set_input_mode(Input_mode_t mode);

#endif
