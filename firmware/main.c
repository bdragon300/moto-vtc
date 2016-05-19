#include "config_application.h"
#include "femtoos_code.h"

#include "display.h"
#include "input.h"
#include "ds1629/ds1629.h"

#include <avr/io.h>


//TODO: debug
/*
 * Data that was fetched from all sources
 */
extern struct {
	ds1629_Temp_t temp;
	ds1629_Time_t clock;
	uint8_t volt;
}
sources_data = {0};

uint8_t bad_voltage = 0;
uint8_t blinking_display = 0;

void
fetch_all_sources(void);


void
appBoot(void)
{
	display_init();
	show_init_display();

	input_init();
	ds1629_init(DS1629_ADDR);
	business_init();

	fetch_all_sources();
	business_reset_state();
}

/*
 * --- TASKS ---
 */

/*
 * Display rendering task
 * Refresh frequency is 100Hz
 */
#if (preTaskDefined(Display))

void
appLoop_Display(void)
{
	//TODO: half-brightness digits
	uint8_t ticks_counter = 0;
    while(1) {

    	taskMutexRequestOnName(display);

    	// Resume blinking task when voltage just become bad
    	if ( bad_voltage
    		&& blinking_display == 0)
    	{
    		genResume(preTaskNumberOf(DisplayBlink));
    	}
    	blinking_display = bad_voltage;
    	render_digit(ticks_counter % 4);
        ++ticks_counter;

        taskMutexReleaseOnName(display);

        taskDelayFromWake(DIGIT_RENDER_DELAY);
    }
}

#endif


/*
 * Display blinking task
 * Toggles display every DISPLAY_BLINK_INTERVAL system ticks
 */
#if (preTaskDefined(DisplayBlink))

void
appLoop_DisplayBlink(void)
{
	int8_t display_disabled = 0;

    while(1) {
    	if (display_disabled) {
    		display_disabled = 0;
    		taskMutexReleaseOnName(display);

    	} else {
        	if ( ! blinking_display) {
        		taskSuspend(defSuspendNow);
        	}

    		taskMutexRequestOnName(display);
    		display_disabled = 1;
    		disable_display();
    	}
    	taskDelayFromNow(DISPLAY_BLINK_INTERVAL);
    }
}

#endif


/*
 * Button task
 */
#if (preTaskDefined(Button))

void
appLoop_Button(void)
{
    taskDelayFromNow(SYSTEM_TICKS_1SEC/2); //show init display 0.5sec
    uint8_t prev_button_pressed = 0;

	while(1) {
		input_tick();
		taskDelayFromNow(INPUT_TICK_DURATION);
	}
}

#endif


/*
 * Input sources read task, except button (temp, clock, volts)
 */
#if (preTaskDefined(Sources))

void
appLoop_Sources(void)
{
	while(1) {
		fetch_all_sources();

		taskDelayFromNow(SYSTEM_TICKS_1SEC);
	}
}

#endif

/*
 * --- UTILITIES ---
 */
/*
 * Fetches data from all input sources, except button (temp, clock, volts)
 */
void
fetch_all_sources(void)
{
	sources_data.temp = ds1629_read_temp(DS1629_ADDR);
	sources_data.clock = ds1629_read_clock(DS1629_ADDR);
	sources_data.volt = get_volts();
	bad_voltage = detect_bad_voltage(); //Sets mode to volt display
}
