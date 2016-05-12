#ifndef DISPLAY_H
#define DISPLAY_H

#include "config_application.h"
#include "ds1629/ds1629.h"
#include "femtoos_code.h"
#include "math.h"

/* Types */
enum {TIME, VOLT, TEMP, INIT} Display_mode_t;
typedef struct 
{
    uint8_t digits[4];
    uint8_t indication; //Auxiliary segments like dots, colons
} Display_data_t;


void
display_init();

// Show information on display and set display mode
void
show_init_display();

void
show_time(ds1629_Time_t data);

void
show_temp(ds1629_Temp_t data);

void
show_volt(uint8_t data);

void
show_charge(uint8_t data);

void
render_digit(uint8_t offset);

// Shows additional indication such as charge led or mode led
static void
_show_indication();

static inline uint8_t
_get_segments(uint8_t number, uint8_t display_0);

#endif
