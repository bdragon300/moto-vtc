#include "femtoos_code.h"
#include "display.h"
#include "math.h"


uint8_t display_data[5] = {0}; //4 digits and indication byte
Display_mode_t display_mode = INIT;
const uint8_t segment_modes = {SEG_MODE1, SEG_MODE2, SEG_MODE3, 0};
const uint8_t display_com = {DISP_COM1, DISP_COM2, DISP_COM3, DISP_COM4, DISP_COM5};
uint8_t charging = 0;

void 
show_time(Time_t data)
{
	display_data = {
		get_segments(data.hours / 10, 0),
		get_segments(data.hours % 10),
		get_segments(data.minutes / 10),
		get_segments(data.minutes % 10),
		SEG_COLON
	};
	display_mode = TIME;
	show_aux();
}

void 
show_temp(int8_t data)
{
	uint8_t adata = abs(data);

	display_data = {
		(data < 0) ? SEG_MINUS : 0,
		get_segments(adata / 100),
		get_segments(adata / 10 % 10),
		get_segments(adata % 10),
		SEG_DOT
	};
	display_mode = TEMP;
	show_aux();
}

void 
show_volt(uint8_t data)
{
	display_data = {
		0,
		get_segments(data / 100, 0),
		get_segments(data / 10 % 10),
		get_segments(data % 10),
		SEG_DOT
	};
	display_mode = VOLT;
	show_aux();
}

void 
show_aux()
{
	display_data.aux |= segment_modes[display_mode];
	display_data.aux |= SEG_CHARGE & charging;
}

void 
render_digit(uint8_t offset)
{
	//Disable all
	DISP_COM_PORT |= DISP_COM_MASK; //Disable all digits
	DISP_PORT |= DISP_SEG_MASK; //Disable all segments

	//Set up
	DISP_PORT &= ~(display_data[offset]);
	DISP_COM_PORT &= ~(display_com[offset]);
}

inline uint8_t 
get_segments(uint8_t number, uint8_t display_0 = 1)
{
	switch(number) {
		case 0:
			if (display_0) return SEG_PATTERN_0;
			else return 0;
			break;
		case 1: return SEG_PATTERN_1; break;
		case 2: return SEG_PATTERN_2; break;
		case 3: return SEG_PATTERN_3; break;
		case 4: return SEG_PATTERN_4; break;
		case 5: return SEG_PATTERN_5; break;
		case 6: return SEG_PATTERN_6; break;
		case 7: return SEG_PATTERN_7; break;
		case 8: return SEG_PATTERN_8; break;
		case 9: return SEG_PATTERN_9; break;
	}
}