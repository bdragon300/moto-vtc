#ifndef INPUT_H
#define INPUT_H

/*
Minimal count of task ticks for LONG_PRESS button mode switch on
*/
#define PRESS_TIMEOUT 200;

/*
Button input mode.
*/
enum {
	IDLE,		//Button released
	CLICK,		//User clicked the button (pressed and released)
	LONG_PRESS	//User is pressing button longer then press timeout
} Input_mode_t;

// Calls on every timer tick from task
void
button_timer_tick(void);


inline Input_mode_t
get_input_mode();

// Returns 0|1
inline uint8_t
get_button_state(void);

//Starts ADC convert and waits until it will finished
//Returns actual voltage * 10 with accuracy 0.1, i.e. 134 means 13.4 volts
inline uint8_t
get_volts(void);

#endif
