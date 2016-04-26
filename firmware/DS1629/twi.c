/*
 * twi.c
 *
 *  Created on: Apr 26, 2016
 *      Author: gosha
 */

#include <avr/io.h>
#include "twi.h"

inline void
twi_init(void)
{
    //SCL will be 1024Hz
    TWBR = 8;
    TWSR &= ~(0b00000011); //clear TWPS0:1
}

inline uint8_t
twi_start(void)
{
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    while ( ! (TWCR & _BV(TWINT)) );
    return (TWSR & 0x08);
}

inline void
twi_stop(void)
{
    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}



uint8_t
twi_send_byte(uint8_t data)
{
    _twi_send_byte(data);
    return (TWSR & 0x08);
}

uint8_t
twi_send_array(uint8_t* buffer, size_t size)
{
	for (size_t i = 0; i < size; ++i) {
		twi_send_byte(data[i]);
	}
}

uint8_t
twi_recv_byte(uint8_t ack)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA & (ack != 0));
	while ( ! (TWCR & _BV(TWINT)) );
	return TWDR;
}
size_t
twi_recv_array(uint8_t* buffer, size_t size)
{
	size_t i = 0;
	while ( ! (TWSR & 0x58) ) {
		//On last iteration we send NAK
		buffer[i] = twi_recv_byte(i+1 == size);
		i++;
	}
	return i;
}


static inline void
_twi_send_byte(uint8_t)
{
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN);
    while ( ! (TWCR & _BV(TWINT)) );
}
