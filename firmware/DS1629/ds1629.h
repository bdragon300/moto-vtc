/*
 * ds1629.h
 *
 *  Created on: Apr 26, 2016
 *      Author: gosha
 */

#ifndef DS1629_DS1629_H_
#define DS1629_DS1629_H_

#include "twi.h"

void
ds1629_init(void);

void
ds1629_op(uint8_t opcode, uint8_t* argv, size_t argc);

#endif /* DS1629_DS1629_H_ */
