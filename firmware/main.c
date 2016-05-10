#include "config_application.h"
#include "femtoos_code.h"

#include "display.h"
#include "input.h"
#include "ds1629/ds1629.h"

#include <avr/io.h>


void appBoot(void)
{
	display_init();
	show_init_display();

	input_init();
	ds1629_init(DS1629_ADDR);
}

/*-------------
--- Globals ---
---------------*/

Display_mode_t display_mode = INIT;


#if (preTaskDefined(Display))

void appLoop_Display(void)
{
    while(true) {
        
    }
}

#endif
