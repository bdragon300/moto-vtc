#ifndef INPUT_H
#define INPUT_H

#include "config_application.h"
#include "types.h"
#include <avr/io.h>
#include <avr/stdint.h>

/*
 * Button input mode
 */
enum {
    IDLE,       //Button released
	PRESS,      //Button is pressing now but less then HOLD_THRESHOLD ticks
    CLICK,      //User clicked the button (pressed and released)
    HOLD        //User is holding button longer then HOLD_THRESHOLD ticks
} Input_mode_t;

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
get_input_mode();


/*
 * Current button state. 0|1
 */
inline uint8_t
get_button_state(void);

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
get_charge(void);



/*
 * Utilities
 */
static void
_set_input_mode(Input_mode_t mode);

#endif
