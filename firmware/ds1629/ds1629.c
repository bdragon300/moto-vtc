/*
 * ds1629.c
 *
 *  Created on: Apr 30, 2016
 *      Author: gosha
 */

#include "../ds1629/ds1629.h"

void
ds1629_init(uint8_t ctl)
{
	uint8_t d = DSCSR_DEFAULT;
	ds1629_op_w(ctl, DSOP_CSR, &d, 1);
}

ds1629_Temp_t
ds1629_read_temp(uint8_t ctl)
{
	int8_t raw[2];

	uint8_t res = ds1629_op_r(ctl, DSOP_T, raw, 2);
	if (res != 2) return 0xffff;

    raw[1] >>= 7;
    raw[1] *= 5;
    return raw[0] * 10 + raw[1];
}

uint8_t
ds1629_start_convert_temp(uint8_t ctl)
{
	return ds1629_op_noarg(ctl, DSOP_START_CONVERT);
}

uint8_t
ds1629_stop_convert_temp(uint8_t ctl)
{
	return ds1629_op_noarg(ctl, DSOP_STOP_CONVERT);
}

ds1629_Time_t
ds1629_read_clock(uint8_t ctl)
{
	uint8_t raw[7];
	ds1629_op_raddr(ctl, DSOP_CLOCK, 0, raw, 7);

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

	return r;
}

uint8_t
ds1629_write_clock(uint8_t ctl, ds1629_Time_t* data)
{
	uint8_t raw[7];
	_number_to_bcd(raw[0], data->seconds); //CH=0
	_number_to_bcd(raw[1], data->minutes);
	_number_to_bcd(raw[2], data->hours); //24-hours mode
	_number_to_bcd(raw[4], data->day);
	_number_to_bcd(raw[5], data->month);
	_number_to_bcd(raw[6], data->year);

	return ds1629_op_waddr(ctl, DSOP_CLOCK, 0, raw, 7);
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
	DSFUNC_START();

	ctl &= ~(0x01); // SLA_W
	uint8_t res = _ds1629_op_w(ctl, opcode, addr, wdata, wdatac);
	res = (res != 0); //0|1

	if (rdatac) {
		ctl |= 0x01; // SLA_R
		DSFUNC_START(); //REPEATED_START
		res = _ds1629_op_r(ctl, rdata, rdatac);
	}

	DSFUNC_STOP();
	return res;
}

static inline uint8_t
_ds1629_op_w(uint8_t ctl, uint8_t opcode, uint8_t addr, uint8_t* data, size_t datac)
{
	uint8_t sent = DSFUNC_SEND_BYTE(ctl);
	sent += DSFUNC_SEND_BYTE(opcode);
	if (addr != 0xff) {
		sent += DSFUNC_SEND_BYTE(addr);
	}
	if (data) {
		sent += DSFUNC_SEND_ARRAY(data, datac);
	}
	return sent;
}

static inline uint8_t
_ds1629_op_r(uint8_t ctl, uint8_t* data, size_t datac)
{
	uint8_t sent = DSFUNC_SEND_BYTE(ctl);
	if ( ! sent ) return 0;

	return DSFUNC_RECV_ARRAY(data, datac);
}





