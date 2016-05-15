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


void
appBoot(void)
{
	display_init();
	show_init_display();

	input_init();
	ds1629_init(DS1629_ADDR);
	business_init();
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
    uint8_t current_digit = 0;
    while(1) {
        render_digit(current_digit++);
        if (current_digit > 4)
            current_digit = 0;
        taskDelayFromWake(DIGIT_RENDER_DELAY);
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
		sources_data.temp = ds1629_read_temp(DS1629_ADDR);
		sources_data.clock = ds1629_read_clock(DS1629_ADDR);
		sources_data.volt = get_volts();
		//TODO: wrong voltage warning

		taskDelayFromNow(SYSTEM_TICKS_1SEC);
	}
}

#endif
