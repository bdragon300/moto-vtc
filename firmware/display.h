#ifndef DISPLAY_H
#define DISPLAY_H

#include "config_application.h"
#include "ds1629/ds1629.h"
#include "femtoos_code.h"
#include "math.h"


/*
 * --- TYPES ---
 */
typedef struct 
{
    uint8_t digits[4];
    uint8_t indication; //Auxiliary segments like dots, colons
} Display_data_t;

/*
 * Initialization
 */
void
display_init();


/*
 * Show "init display" with dashes
 */
void
show_init_display();


/*
 * Functions show given information
 */
void
show_time(ds1629_Time_t data);

void
show_temp(ds1629_Temp_t data);

void
show_volt(uint8_t data);

void
show_charge(uint8_t data);


/*
 * Calls from main code to render digit with offset
 */
void
render_digit(uint8_t offset);



/*
 * --- Utilities ---
 */

/*
 * Shows additional indication such as mode led
 */
static void
_show_indication();

/*
 * Gets segment mask byte for given number
 * display_0 - whether show or not 0 number
 */
static inline uint8_t
_get_segments(uint8_t number, uint8_t display_0);

#endif
