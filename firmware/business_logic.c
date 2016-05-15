/*
 * business_logic.c
 *
 *  Created on: May 14, 2016
 *      Author: gosha
 */

/*
 * === VARIABLES ===
 */

/*
 * Current mode in which device is now
 */
extern Device_mode_t device_mode = {0};

/*
 * Data that was fetched from all sources
 */
struct {
	ds1629_Temp_t temp;
	ds1629_Time_t clock;
	uint8_t volt;
}
fetched_data = {0};


/*
 * === FUNCTIONS ===
 */

void
business_init(void)
{
	input_callbacks.on_button_click = button_click;
	input_callbacks.on_button_hold = button_hold;
	input_callbacks.on_button_release = button_release;

	device_mode.display_mode = INIT;
	fetch_all_data();
}

void
button_click(void)
{
	if (device_mode.forbid_button)
		return;

	if (device_mode.settings.settings_mode == NONE) {
		++device_mode.display_mode;

		if (device_mode.display_mode > TEMP) {
			device_mode.display_mode = TIME;
		}

		switch (device_mode.display_mode) {
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
	else if (device_mode.settings.settings_mode == TIME) {
		++device_mode.settings.time_settings.current_digit;

		if (device_mode.settings.time_settings.current_digit > 3) {
			device_mode.settings.settings_mode == NONE;
			ds1629_write_clock(DS1629_ADDR, device_mode.settings.time_settings.new_time);
			fetched_data.clock = device_mode.settings.time_settings.new_time;
			//release semaphore
		}
	}
}

void
button_hold(void)
{
	if (device_mode.forbid_button)
		return;

	if (device_mode.settings.settings_mode == NONE) {
		//get semaphore
		device_mode.settings.settings_mode == TIME;
		device_mode.settings.time_settings.new_time = fetched_data.clock;
		device_mode.settings.time_settings.current_digit = 0;

		device_mode.forbid_button = 1; //User must release button after entering settings
	}
	else if (device_mode.settings.settings_mode == TIME) {
		switch (device_mode.settings.time_settings.current_digit) {
			case 0:
				_time_increment_dechour(device_mode.settings.time_settings.new_time);
				break;
			case 1:
				_time_increment_onehour(device_mode.settings.time_settings.new_time);
				break;
			case 2:
				_time_increment_decmin(device_mode.settings.time_settings.new_time);
				break;
			case 3:
				_time_increment_onemin(device_mode.settings.time_settings.new_time);
				break;
		}
		show_time(device_mode.settings.time_settings.new_time);
	}
}

void
button_release(void)
{
	if (device_mode.settings.settings_mode == TIME) {
		device_mode.forbid_button = 0;
	}

	if (device_mode.forbid_button)
		return;
}

void
fetch_all_data(void)
{
	fetched_data.temp = ds1629_read_temp(DS1629_ADDR);
	fetched_data.clock = ds1629_read_clock(DS1629_ADDR);
	fetched_data.volt = get_volts();
}
