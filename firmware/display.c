#include "display.h"


Display_data_t display_data = {0}; //4 digits and indication byte
Display_mode_t display_mode = INIT;
const uint8_t segment_modes[4] = {SEG_MODE_1, SEG_MODE_2, SEG_MODE_3, 0};
const uint8_t display_com[5] = {DISP_COM1, DISP_COM2, DISP_COM3, DISP_COM4, DISP_COM5};
uint8_t charging = 0;

void
display_init()
{
	//Disable all digits
	DISP_COM_PORT |= DISP_COM_MASK;
	DISP_PORT |= DISP_SEG_MASK;
}

void 
show_time(ds1629_Time_t data)
{
    display_data.digits[0] = get_segments(data.hours / 10, 0);
    display_data.digits[1] = get_segments(data.hours % 10);
    display_data.digits[2] = get_segments(data.minutes / 10);
    display_data.digits[3] = get_segments(data.minutes % 10);
    display_data.indication = SEG_COLON;

    display_mode = TIME;
    show_indication();
}

void 
show_temp(ds1629_Temp_t data)
{
    uint8_t adata = abs(data);

    display_data.digits[0] = (data < 0) ? SEG_MINUS : 0;
    display_data.digits[1] = get_segments(adata / 100);
    display_data.digits[2] = get_segments(adata / 10 % 10);
    display_data.digits[3] = get_segments(adata % 10);
    display_data.indication = SEG_DOT;

    display_mode = TEMP;
    show_indication();
}

void 
show_volt(uint8_t data)
{
    display_data.digits[0] = 0;
    display_data.digits[1] = get_segments(data / 100, 0);
    display_data.digits[2] = get_segments(data / 10 % 10);
    display_data.digits[3] = get_segments(data % 10);
    display_data.indication = SEG_DOT;

    display_mode = VOLT;
    show_indication();
}

void
show_charge(uint8_t data)
{
	charging = (data != 0);
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

static void
show_indication()
{
    display_data.indication |= segment_modes[display_mode];
    display_data.indication |= SEG_CHARGE * charging;
}

static inline uint8_t
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
