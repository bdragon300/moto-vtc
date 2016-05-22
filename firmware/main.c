#include "config_application.h"
#include "femtoos_code.h"

#include "display.h"
#include "input.h"
#include "ds1629/ds1629.h"
#include "uartdebug/uartdebug.h"

#include <avr/io.h>


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
	DEBUG1('>', "appBoot");

	uartDebugInit();

	display_init();
	show_init_display();

	input_init();
	ds1629_init(DS1629_ADDR);
	business_init();

	fetch_all_sources();
	business_reset_state();

	DEBUG1('<', "appBoot");
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
	uint8_t ticks_counter = 0;
    while(1) {

    	taskMutexRequestOnName(display);

    	// Resume blinking task when voltage just become bad
    	if ( bad_voltage
    		&& blinking_display == 0)
    	{
    		DEBUG2('@', "Display");
    		DEBUG2('@', "#voltage goes bad");

    		genResume(preTaskNumberOf(DisplayBlink));
    	}
    	blinking_display = bad_voltage;
    	render_digit(ticks_counter % 4);

        taskMutexReleaseOnName(display);

        // If current digit is shadowed then render it shortly
        if (get_digits_shadow_mask() & _BV(ticks_counter % 4)) {
        	taskDelayFromWake(SHADOW_DIGIT_RENDER_DELAY);
        	disable_display();
        	taskDelayFromNow(DIGIT_RENDER_DELAY - SHADOW_DIGIT_RENDER_DELAY);
        }
        else {
        	taskDelayFromWake(DIGIT_RENDER_DELAY);
        }
        ++ticks_counter;
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
    		DEBUG2('@', "DisplayBlink");
    		DEBUG2('@', "#enable display");

    		display_disabled = 0;

    		taskMutexReleaseOnName(display);

    	} else {
        	if ( ! blinking_display) {
        		DEBUG2('@', "DisplayBlink");
        		DEBUG2('@', "#suspend task");

        		taskSuspend(defSuspendNow);
        	}

    		taskMutexRequestOnName(display);

    		DEBUG2('@', "DisplayBlink");
    		DEBUG2('@', "#disable display");

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
		taskMutexRequestOnName(sources);

		fetch_all_sources();

		taskMutexReleaseOnName(sources);

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
	DEBUG2('>', "fetch_all_sources");
	DEBUGHEX2('s', &sources_data, sizeof(sources_data));

	sources_data.temp = ds1629_read_temp(DS1629_ADDR);
	sources_data.clock = ds1629_read_clock(DS1629_ADDR);
	sources_data.volt = get_volts();
	bad_voltage = detect_bad_voltage(); //Sets mode to volt display

	DEBUGHEX2('S', &sources_data, sizeof(sources_data));
	DEBUGHEX2('B', &bad_voltage, 1);
	DEBUG2('<', "fetch_all_sources");
}
