#include "config_application.h"
#include "femtoos_code.h"

#include "display.h"
#include "input.h"
#include "ds1629/ds1629.h"

#include <avr/io.h>


void
appBoot(void)
{
	display_init();
	show_init_display();

	input_init();
	ds1629_init(DS1629_ADDR);

	update_data();
}

/*-------------
--- Globals ---
---------------*/

Display_mode_t display_mode = INIT;

struct {
	ds1629_Temp_t temp;
	ds1629_Time_t clock;
	uint8_t volt;
}
fetched_data;


#if (preTaskDefined(Display))

void
appLoop_Display(void)
{
    uint8_t current_digit = 0;
    while(1) {
        render_digit(current_digit++);
        if (current_digit > 4)
            current_digit = 0;

        //Delay 5ms
        taskDelayFromNow(78);
    }
}

#endif

#if (preTaskDefined(Input))

void
appLoop_Input(void)
{
    taskDelayFromNow(7812); //show init display 0.5sec
    display_mode = TIME;
	while(1) {
		input_tick();

		Input_mode_t mode = get_input_mode();
		if (mode == CLICK) {
			display_mode++;
			if (display_mode > TEMP) {
				display_mode = TIME;
			}

			switch (display_mode) {
				case TIME:
					show_time(fetched_data.clock);
					break;
				case VOLT:
					show_volt(fetched_data.volt);
					break;
				case TEMP:
					show_temp(fetched_data.temp);
					break;
			}
		}
	}
}

#endif


/*-------------
-- Utilities --
---------------*/
void
update_data(void)
{
	fetched_data.temp = ds1629_read_temp(DS1629_ADDR);
	fetched_data.clock = ds1629_read_clock(DS1629_ADDR);
	fetched_data.volt = get_volts();
}
