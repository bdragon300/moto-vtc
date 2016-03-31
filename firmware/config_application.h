#include <avr/io.h>


/*----------------
---- Hardware ----
------------------*/

#define INPUT_PORT	PORTC;
#define INPUT_PIN	PINC;
#define INPUT_DDR	DDRC;
#define	VOLTAGE_PIN	PC7;
#define CHARGE_PIN	PC0;
#define BUTTON_PIN	PC6;

//7-seg indicator
#define DISP_PORT	PORTD;
#define DISP_PIN	PIND;
#define DISP_DDR	DDRD;
#define DISP_A		PD0;
#define DISP_B		PD1;
#define DISP_C		PD2;
#define DISP_D		PD3;
#define DISP_E		PD4;
#define DISP_F		PD5;
#define DISP_G		PD6;
// digits
#define DISP_D_PORT	PORTB;
#define DISP_D_PIN	PINB;
#define DISP_D_DDR	DDRB;
#define DISP_D0		PB1;
#define DISP_D1		PB2;
#define DISP_D2		PB3;
#define DISP_D3		PB4;
#define DISP_D4		PB5;


/*----------------
---- Software ----
------------------*/
/* Digits patters depending on DISP_X macroses */
#define	DIG_PATTERN_0	_BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)
#define	DIG_PATTERN_1	_BV(DISP_B)|_BV(DISP_C)
#define	DIG_PATTERN_2	_BV(DISP_A)|_BV(DISP_B)|_BV(DISP_G)|_BV(DISP_E)|_BV(DISP_D)
#define	DIG_PATTERN_3	_BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_G)
#define DIG_PATTERN_4	_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_F)|_BV(DISP_G)
#define	DIG_PATTERN_5	_BV(DISP_A)|_BV(DISP_F)|_BV(DISP_G)|_BV(DISP_C)|_BV(DISP_D)
#define	DIG_PATTERN_6	_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)|_BV(DISP_G)
#define	DIG_PATTERN_7	_BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)
#define	DIG_PATTERN_8	_BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)|_BV(DISP_G)
#define	DIG_PATTERN_9	_BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_F)|_BV(DISP_G)


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