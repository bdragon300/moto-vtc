/*
 * ds1629.c
 *
 *  Created on: Apr 30, 2016
 *      Author: gosha
 */

#include "ds1629.h"

void
ds1629_init(uint8_t addr)
{
	ds1629_op(addr, DSOP_CSR, DSCSR_DEFAULT, 1);
}

ds1629_Temp_t
ds1629_read_temp(void)
{

}

uint8_t
ds1629_op(
		uint8_t addr,
		uint8_t opcode,
		uint8_t* wargv,
		size_t wargc,
		uint8_t* rargv,
		size_t rargc
		)
{
	DSFUNC_START();

	addr &= ~(0x01); // SLA_W
	uint8_t res = _ds1629_op_w(addr, opcode, wargv, wargc);
	res = (res != 0); //0|1

	if (rargc) {
		addr |= 0x01; // SLA_R
		DSFUNC_START(); //REPEATED_START
		res = _ds1629_op_r(addr, rargv, rargc);
	}

	DSFUNC_STOP();
	return res;
}

static inline uint8_t
_ds1629_op_w(uint8_t addr, uint8_t opcode, uint8_t* argv, size_t argc)
{
	uint8_t sent = DSFUNC_SEND_BYTE(addr);
	sent += DSFUNC_SEND_BYTE(opcode);
	if (argv) {
		sent += DSFUNC_SEND_ARRAY(argv, argc);
	}
	return sent;
}

static inline uint8_t
_ds1629_op_r(uint8_t addr, uint8_t* argv, size_t argc)
{
	uint8_t sent = DSFUNC_SEND_BYTE(addr);
	if ( ! sent ) return 0;

	return DSFUNC_RECV_ARRAY(argv, argc);
}





