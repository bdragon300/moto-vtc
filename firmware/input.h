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

#endif