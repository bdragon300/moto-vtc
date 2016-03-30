#include <avr/io.h>


/*----------------
---- Hardware ----
------------------*/

#define	VOLTAGE_PIN	PC7;
#define CHARGE_PIN	PC0;
#define BUTTON_PIN	PB0;

//7-seg indicator
#define DISP_A		PD0;
#define DISP_B		PD1;
#define DISP_C		PD2;
#define DISP_D		PD3;
#define DISP_E		PD4;
#define DISP_F		PD5;
#define DISP_G		PD6;
// digits
#define DISP_D0		PB1;
#define DISP_D1		PB2;
#define DISP_D2		PB3;
#define DISP_D3		PB4;
#define DISP_D4		PB5;





/*----------------
----- Tasks ------
------------------*/

#define	CN_00	Display
#define	CN_01	Input
#define CN_02	ADC
#define CN_04	TimeTemp

/*----------------
- API inclusions -
------------------*/
#define	includeTaskYield			cfgFalse
#define	includeTaskDelayFromNow		cfgTrue
#define	includeTaskDelayFromWake	cfgFalse
#define	includeGenSuspend			cfgFalse

/*----------------
- external calls -
------------------*/
#define	callAppBoot					cfgTrue