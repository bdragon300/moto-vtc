/*
 * ds1629.c
 *
 *  Created on: Apr 30, 2016
 *      Author: gosha
 */

#include "ds1629.h"

void
ds1629_init(uint8_t ctl)
{
	DEBUG1('>', "ds1629_init");
	DEBUGHEX1('(', &ctl, 1);

	uint8_t d = DSCSR_DEFAULT;
	ds1629_op_w(ctl, DSOP_CSR, &d, 1);

	DEBUG1('<', "ds1629_init");
}

ds1629_Temp_t
ds1629_read_temp(uint8_t ctl)
{
	DEBUG1('>', "ds1629_read_temp");

	int8_t raw[2];

	uint8_t res = ds1629_op_r(ctl, DSOP_T, raw, 2);
	if (res != 2) return 0xffff;

    raw[1] >>= 7;
    raw[1] *= 5;

    uint8_t res = raw[0] * 10 + raw[1];

    DEBUGHEX1('=', &res, 1);
    DEBUG1('<', "ds1629_read_temp");

    return res;
}

uint8_t
ds1629_start_convert_temp(uint8_t ctl)
{
	DEBUG2('>', "ds1629_start_convert_temp");
	DEBUGHEX2('(', &ctl, 1);

	uint8_t res = ds1629_op_noarg(ctl, DSOP_START_CONVERT);

	DEBUGHEX2('=', &res, 1);
	DEBUG2('<', "ds1629_start_convert_temp");
}

uint8_t
ds1629_stop_convert_temp(uint8_t ctl)
{
	DEBUG2('>', "ds1629_start_convert_temp");
	DEBUGHEX2('(', &ctl, 1);

	uint8_t res = ds1629_op_noarg(ctl, DSOP_STOP_CONVERT);

	DEBUGHEX2('=', &res, 1);
	DEBUG2('<', "ds1629_start_convert_temp");
}

ds1629_Time_t
ds1629_read_clock(uint8_t ctl)
{
	DEBUG2('>', "ds1629_read_clock");
	DEBUGHEX2('(', &ctl, 1);

	uint8_t raw[7];
	ds1629_op_raddr(ctl, DSOP_CLOCK, 0, raw, 7);

	DEBUGHEX2('r', &raw, 7);

	ds1629_Time_t r;
	_bcd_to_number(r.seconds, raw[0], 0b01110000);
	_bcd_to_number(r.minutes, raw[1], 0b01110000);
	_bcd_to_number(r.hours, raw[2], 0b00010000);
	_bcd_to_number(r.day, raw[4], 0b00110000);
	_bcd_to_number(r.month, raw[5], 0b00010000);
	_bcd_to_number(r.year, raw[6], 0b11110000);
	if (raw[2] & 0b01000000) { //12 to 24-hours mode
		r.hours += 12;
	}

	DEBUGHEX2('=', &r, sizeof(ds1629_Time_t));
	DEBUG2('<', "ds1629_read_clock");

	return r;
}

uint8_t
ds1629_write_clock(uint8_t ctl, ds1629_Time_t* data)
{
	DEBUG2('>', "ds1629_write_clock");
	DEBUGHEX2('(', &ctl, 1);
	DEBUGHEX2('(', &data, sizeof(ds1629_Time_t));

	uint8_t raw[7];
	_number_to_bcd(raw[0], data->seconds); //CH=0
	_number_to_bcd(raw[1], data->minutes);
	_number_to_bcd(raw[2], data->hours); //24-hours mode
	_number_to_bcd(raw[4], data->day);
	_number_to_bcd(raw[5], data->month);
	_number_to_bcd(raw[6], data->year);

	DEBUGHEX2('r', &raw, 7);

	uint8_t res = ds1629_op_waddr(ctl, DSOP_CLOCK, 0, raw, 7);

	DEBUGHEX2('=', &res, 1);
	DEBUG2('<', "ds1629_write_clock");
}

uint8_t
ds1629_op(
		uint8_t ctl,
		uint8_t opcode,
		uint8_t addr,
		uint8_t* wdata,
		size_t wdatac,
		uint8_t* rdata,
		size_t rdatac
		)
{
	DEBUG2('>', "ds1629_op");
	DEBUGHEX2('(', &ctl, 1);
	DEBUGHEX2('(', &opcode, 1);
	DEBUGHEX2('(', &addr, 1);
	DEBUGHEX2('(', &wdata, wdatac);
	DEBUGHEX2('(', &wdatac, 1);
	DEBUGHEX2('(', &rdata, rdatac);
	DEBUGHEX2('(', &rdatac, 1);

	DEBUG2('#', "i2c start");

	DSFUNC_START();

	ctl &= ~(0x01); // SLA_W

	DEBUGHEX2('c', &ctl, 1);
	DEBUG2('#', "write data");

	uint8_t res = _ds1629_op_w(ctl, opcode, addr, wdata, wdatac);
	res = (res != 0); //0|1

	DEBUGHEX2('r', &res, 1);

	if (rdatac) {
		DEBUG2('#', "read data");

		ctl |= 0x01; // SLA_R

		DEBUGHEX2('c', &ctl, 1);
		DEBUG2('#', "repeated start");

		DSFUNC_START(); //REPEATED_START
		res = _ds1629_op_r(ctl, rdata, rdatac);

		DEBUGHEX2('r', &res, 1);
	}

	DEBUG2('#', "i2c start");

	DSFUNC_STOP();

	DEBUGHEX2('=', &res, 1);
	DEBUG2('<', "ds1629_op");

	return res;
}

static inline uint8_t
_ds1629_op_w(uint8_t ctl, uint8_t opcode, uint8_t addr, uint8_t* data, size_t datac)
{
	DEBUG2('>', "_ds1629_op_w");
	DEBUGHEX2('(', &ctl, 1);
	DEBUGHEX2('(', &opcode, 1);
	DEBUGHEX2('(', &addr, 1);
	DEBUGHEX2('(', &data, datac);
	DEBUGHEX2('(', &datac, 1);

	uint8_t sent = DSFUNC_SEND_BYTE(ctl);

	DEBUGHEX2('s', &sent, 1);

	sent += DSFUNC_SEND_BYTE(opcode);

	DEBUGHEX2('s', &sent, 1);

	if (addr != 0xff) {
		sent += DSFUNC_SEND_BYTE(addr);
		DEBUGHEX2('s', &sent, 1);
	}
	if (data) {
		sent += DSFUNC_SEND_ARRAY(data, datac);
		DEBUGHEX2('s', &sent, 1);
	}

	DEBUGHEX2('=', &sent, 1);
	DEBUG2('<', "_ds1629_op_w");

	return sent;
}

static inline uint8_t
_ds1629_op_r(uint8_t ctl, uint8_t* data, size_t datac)
{
	DEBUG2('>', "_ds1629_op_r");
	DEBUGHEX2('(', &ctl, 1);
	DEBUGHEX2('(', &data, datac);
	DEBUGHEX2('(', &datac, 1);

	uint8_t sent = DSFUNC_SEND_BYTE(ctl);

	DEBUGHEX2('s', &sent, 1);

	if ( ! sent ) {
		DEBUG2('<', "_ds1629_op_r");
		return 0;
	}

	uint8_t res = DSFUNC_RECV_ARRAY(data, datac);

	DEBUGHEX2('=', &res, 1);
	DEBUG2('<', "_ds1629_op_r");

	return res;
}





