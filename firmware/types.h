typedef struct {
	uint8_t	seconds;
	uint8_t minutes;
	uint8_t hours; //24-hour format
	uint8_t day;
	uint8_t month;
	uint8_t year;
} RTC_time_t;
typedef RTC_time_t Human_timedate_t;

enum {TIME, VOLT, TEMP, INIT} Display_mode_t;