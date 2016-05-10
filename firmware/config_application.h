#include <avr/io.h>


/*----------------
---- Hardware ----
------------------*/

#define INPUT_PORT  PORTC;
#define INPUT_PIN   PINC;
#define INPUT_DDR   DDRC;
#define VOLTAGE_PIN PC7;
#define CHARGE_PIN  PC0;
#define BUTTON_PIN  PC6;

//7-seg indicator
//Low level is active
#define DISP_PORT   PORTD;
#define DISP_PIN    PIND;
#define DISP_DDR    DDRD;
#define DISP_A      PD0;
#define DISP_B      PD1;
#define DISP_C      PD2;
#define DISP_D      PD3;
#define DISP_E      PD4;
#define DISP_F      PD5;
#define DISP_G      PD6;
#define DISP_SEG_MASK   _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)|_BV(DISP_G)
// digits
//Low level is active
#define DISP_COM_PORT   PORTB;
#define DISP_COM_PIN    PINB;
#define DISP_COM_DDR    DDRB;
#define DISP_COM1       PB1;
#define DISP_COM2       PB2;
#define DISP_COM3       PB3;
#define DISP_COM4       PB4;
#define DISP_COM5       PB5;
#define DISP_COM_MASK   _BV(DISP_COM1)|_BV(DISP_COM2)|_BV(DISP_COM3)|_BV(DISP_COM4)|_BV(DISP_COM5)

// DS1629 address
// Last bit will be ignored
#define DS1629_ADDR     0x02

/*----------------
---- Software ----
------------------*/
/* Digits patters depending on DISP_X macroses */
#define SEG_PATTERN_0   _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)
#define SEG_PATTERN_1   _BV(DISP_B)|_BV(DISP_C)
#define SEG_PATTERN_2   _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_G)|_BV(DISP_E)|_BV(DISP_D)
#define SEG_PATTERN_3   _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_G)
#define SEG_PATTERN_4   _BV(DISP_B)|_BV(DISP_C)|_BV(DISP_F)|_BV(DISP_G)
#define SEG_PATTERN_5   _BV(DISP_A)|_BV(DISP_F)|_BV(DISP_G)|_BV(DISP_C)|_BV(DISP_D)
#define SEG_PATTERN_6   _BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)|_BV(DISP_G)
#define SEG_PATTERN_7   _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)
#define SEG_PATTERN_8   _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)|_BV(DISP_G)
#define SEG_PATTERN_9   _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_F)|_BV(DISP_G)

#define SEG_MODE_1      _BV(DISP_A)
#define SEG_MODE_2      _BV(DISP_B)
#define SEG_MODE_3      _BV(DISP_C)
#define SEG_COLON       _BV(DISP_D)|_BV(DISP_E)
#define SEG_DOT         _BV(DISP_F)
#define SEG_CHARGE      _BV(DISP_G)
#define SEG_MINUS       _BV(DISP_G) /* Minus in digit */


/*----------------
----- Tasks ------
------------------*/

#define CN_00   Display
#define CN_01   Input
#define CN_02   ADC
#define CN_04   TimeTemp

/*----------------
- API inclusions -
------------------*/
#define includeTaskYield            cfgFalse
#define includeTaskDelayFromNow     cfgTrue
#define includeTaskDelayFromWake    cfgFalse
#define includeGenSuspend           cfgFalse
#define includeTaskDelayFromNow     cfgTrue

/*----------------
- external calls -
------------------*/
#define callAppBoot                 cfgTrue

/*----------------
------ Stack -----
------------------*/
#define StackSafety                 4
#define StackSizeOS                 24
#define StackSizeISR                0
#define StackSizeShared             0
#define StackSizeOverride           46
