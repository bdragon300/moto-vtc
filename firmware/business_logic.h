/*
 * business_logic.h
 *
 *  Created on: May 14, 2016
 *      Author: gosha
 */

#ifndef BUSINESS_LOGIC_H_
#define BUSINESS_LOGIC_H_

#include "config_application.h"
#include "ds1629/ds1629.h"
#include "input.h"

/*
 * === TYPES ===
 */

/*
 * Current display mode
 */
typedef enum {TIME, VOLT, TEMP, INIT} Display_mode_t;

/*
 * Settings
 */
typedef enum {NONE, TIME} Settings_mode_t;
typedef struct {
	Settings_mode_t settings_mode;
	struct {
		uint8_t current_digit;
		ds1629_Time_t new_time;
	} time_settings;
} Device_settings_t;

/*
 * Device mode
 */
typedef struct {
	Display_mode_t display_mode;
	Device_settings_t settings;
	uint8_t forbid_button;
} Device_mode_t;

/*
 * === FUNCTIONS ===
 */

/*
 * Initialization
 */
void
business_init(void);

/*
 * Calls when button clicked
 */
void
button_click(void);

/*
 * Calls when button is started hold and repeatedly while it is pressing
 */
void
button_hold(void);

/*
 * Calls when button released
 */
void
button_release(void);

/*
 * Fetch and updates actual data from all devices
 */
void
fetch_all_data(void);

/*
 * Returns mask of half-brigthness digits. 1 is shadowed.
 */
uint8_t
get_digits_shadow_mask(void)
{
	if (device_mode.settings.settings_mode == TIME)
		return ~(0x80 >> device_mode.settings.time_settings.current_digit);
	return 0;
}

/*
 * === UTILITIES ===
 */
static void
_time_increment_decmin(ds1629_Time_t* time)
{
	time->minutes = (time->minutes > 49) ? time->minutes % 10 : time->minutes + 10;
}
static void
_time_increment_onemin(ds1629_Time_t* time)
{
	if (++time->minutes % 10 == 0)
		time->minutes -= 10;
}
static void
_time_increment_dechour(ds1629_Time_t* time)
{
	time->hours = (time->hours > 19) ? time->hours % 10 : time->hours + 10;

	if (time->hours > 23)
		time->hours = 23;
}
static void
_time_increment_onehour(ds1629_Time_t* time)
{
	if (++time->hours % 10 == 0)
		time->hours -= 10;
	else
		++time->hours;

	if (time->hours > 23)
		time->hours = 20;
}

#endif /* BUSINESS_LOGIC_H_ */
