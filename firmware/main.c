#include "config_application.h"
#include "femtoos_code.h"
#include <avr/io.h>


void appBoot(void)
{
	DISP_PORT = _BV(DISP_A) | _BV(DISP_B) | _BV(DISP_C) | _BV(DISP_D) | _BV(DISP_E)
		| _BV(DISP_F) | _BV(DISP_G);
	DISP_DDR = _BV(DISP_A) | _BV(DISP_B) | _BV(DISP_C) | _BV(DISP_D) | _BV(DISP_E)
		| _BV(DISP_F) | _BV(DISP_G);

	DISP_D_PORT = _BV(DISP_D0) | _BV(DISP_D1) | _BV(DISP_D2) | _BV(DISP_D3)
		| _BV(DISP_D4) | _BV(DISP_D5);
	DISP_D_DDR = _BV(DISP_D0) | _BV(DISP_D1) | _BV(DISP_D2) | _BV(DISP_D3)
		| _BV(DISP_D4) | _BV(DISP_D5);

	INPUT_DDR &= ~(_BV(CHARGE_PIN) | _BV(BUTTON_PIN));
}

#if (preTaskDefined(Display))

void appLoop_Display(void)
{
	const char dig_pattern = {DIG_PATTERN_0, DIG_PATTERN_1, DIG_PATTERN_2, DIG_PATTERN_3, DIG_PATTERN_4,
		DIG_PATTERN_5, DIG_PATTERN_6, DIG_PATTERN_7, DIG_PATTERN_8, DIG_PATTERN_9};
	while(true) {
		
	}
}

#endif