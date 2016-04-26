/*
 * twi.h
 *
 *  Created on: Apr 24, 2016
 *      Author: gosha
 */

#ifndef DS1629_TWI_H_
#define DS1629_TWI_H_

inline void
twi_init(void);

inline uint8_t
twi_start(void); //0 - success
inline void
twi_stop(void); //0 - success

uint8_t
twi_send_byte(uint8_t data); //Returns ACK
uint8_t
twi_recv_byte(uint8_t ack); //Slave receives ACK/NAK depending on ack

static inline void
_twi_send_byte(uint8_t);

#endif /* DS1629_TWI_H_ */
