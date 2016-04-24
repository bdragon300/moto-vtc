/*
 * twi.h
 *
 *  Created on: Apr 24, 2016
 *      Author: gosha
 */

#ifndef DS1629_TWI_H_
#define DS1629_TWI_H_

/* Set START condition on bus */
void
twi_start(void);

/* Set STOP condition on bus */
void
twi_stop(void);

uint8_t twi_write_byte(uint8_t data); //Returns ACK
uint8_t twi_read_byte(void);
uint8_t twi_write_2byte(uint16_t data); //Returns ACK
uint16_t twi_read_byte(void);

#endif /* DS1629_TWI_H_ */
