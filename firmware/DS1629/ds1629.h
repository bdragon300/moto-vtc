/*
 * ds1629.h
 *
 *  Created on: Apr 26, 2016
 *      Author: gosha
 */

#ifndef DS1629_DS1629_H_
#define DS1629_DS1629_H_

#include <avr/io.h>
#include <stdint.h>
#include "twi.h"


/*
 * Commands opcode definition
 */

/** Configuration/Memory Command **/
/* Read/Write mode of two operations below depends on R/W transaction on I2C bus */
#define DSOP_CSR           0xAC     /* Writes to 8-bit configuration register */
                                     /* Reads from configuration/status register */
#define DSOP_CSRAM         0x17     /* Read/Write SRAM */

/** Thermometer Commands **/
#define DSOP_START_CONVERT 0xEE     /* Initiates temperature conversion(s) */
#define DSOP_STOP_CONVERT  0x22     /* Terminates continuous conversions */
#define DSOP_T             0xAA     /* Access Read Temperature Register */
#define DSOP_TCOUNTER      0xA8     /* Reads COUNT_REMAIN */
#define DSOP_TSLOPE        0xA8     /* Reads COUNT_PER_C */
#define DSOP_TH            0xA1     /* Writes to/Reads from Thermostat High register */
#define DSOP_TL            0xA2     /* Writes to/Reads from Thermostat Low register */

/** Clock Commands **/
#define DSOP_CLOCK         0xC0     /* Sets/Reads Clock */
#define DSOP_CLOCK_ALARM   0xC7     /* Sets/Reads Clock Alarm */


/*
 * Configuration/Status Register
 */
#define DSCSR_OS1          8
#define DSCSR_OS0          7
#define DSCSR_A1           6
#define DSCSR_A0           4
#define DSCSR_CNV          2
#define DSCSR_POL          1
#define DSCSR_1SH          0
#define DSCSR_DEFAULT      _BV(DSCSR_OS1) | _BV(DSCSR_OS0) | _BV(DSCSR_CNV) | _BV(DSCSR_1SH)

#define DSCSR_CAF          8
#define DSCSR_TAF          7
#define DSCSR_CAL          6
#define DSCSR_TAL          5

//ignore addr arg in ds1629_op
#define DSADDR_IGNORE      0xff


/*
 * Types
 */
typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours; //24-hour format
    uint8_t day;
    uint8_t month;
    uint8_t year;
} ds1629_Time_t;
typedef int16_t ds1629_Temp_t;


/*
 * Initialization
 */
void
ds1629_init(uint8_t ctl);

/*
 * Temperature
 */

// Reads temperature register with 0.5 C resolution
// Returns actual temperature * 10, i.e. -12.5 will be -125
ds1629_Temp_t
ds1629_read_temp(uint8_t ctl);
uint8_t
ds1629_start_convert_temp(uint8_t ctl);
uint8_t
ds1629_stop_convert_temp(uint8_t ctl);

/*
 * Time/Date
 */
ds1629_Time_t
ds1629_read_clock(uint8_t ctl);

uint8_t
ds1629_op(
	uint8_t ctl,
	uint8_t opcode,
	uint8_t addr, //DSADDR_IGNORE
	uint8_t* wdata,
	size_t wdatac,
	uint8_t* rdata,
	size_t rdatac
);
#define ds1629_op_noarg(ctl, opcode) \
	ds1629_op(ctl, opcode, 0, 0, DSADDR_IGNORE, 0, 0)
#define ds1629_op_w(ctl, opcode, data, datac) \
	ds1629_op(ctl, opcode, data, DSADDR_IGNORE, datac, 0, 0)
#define ds1629_op_r(ctl, opcode, data, datac) \
	ds1629_op(ctl, opcode, 0, 0, DSADDR_IGNORE, data, datac)
#define ds1629_op_wa(ctl, opcode, addr, data, datac) \
	ds1629_op(ctl, opcode, data, addr, datac, 0, 0)
#define ds1629_op_ra(ctl, opcode, addr, data, datac) \
	ds1629_op(ctl, opcode, data, addr, datac, 0, 0)

/* Writes addr byte, opcode byte and data bytes */
static inline uint8_t
_ds1629_op_w(uint8_t ctl, uint8_t opcode, uint8_t addr, uint8_t* data, size_t datac);

/* Writes addr byte and receive data bytes from slave, related to previous op */
static inline uint8_t
_ds1629_op_r(uint8_t ctl, uint8_t* data, size_t datac);


/*
 * I2C func names
 * Change if you use another func names
 */
#define DSFUNC_START       twi_start         /* start(void) */
#define DSFUNC_STOP        twi_stop          /* stop(void) */
#define DSFUNC_SEND_BYTE   twi_send_byte     /* send_byte(uint8_t data) */
#define DSFUNC_SEND_ARRAY  twi_send_array    /* send_array(uint8_t* buffer, size_t size) */
#define DSFUNC_RECV_BYTE   twi_recv_byte     /* recv_byte(uint8_t ack) */
#define DSFUNC_RECV_ARRAY  twi_recv_array    /* recv_array(uint8_t* buffer, size_t size) */

#endif /* DS1629_DS1629_H_ */
