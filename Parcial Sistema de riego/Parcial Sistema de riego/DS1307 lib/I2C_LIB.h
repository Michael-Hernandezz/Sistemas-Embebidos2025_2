#ifndef I2C_LIB_H_
#define I2C_LIB_H_

#include <avr/io.h>
#include <util/twi.h>
#include <stdint.h>

// Dirección del registro TWI del ATmega1284P
#define F_CPU 16000000UL   // Cambia según tu frecuencia
#define SCL_CLOCK 100000UL // 100kHz estándar

// Estructura para representar el estado del TWI
typedef union {
	struct {
		uint8_t start_sent       : 1;
		uint8_t repeated_start   : 1;
		uint8_t slave_ack        : 1;
		uint8_t data_ack         : 1;
		uint8_t arbitration_lost : 1;
		uint8_t bus_error        : 1;
		uint8_t reserved         : 2;
	} bits;
	uint8_t all_flags;
} I2CStatus_t;

extern I2CStatus_t I2CStatus;

// Funciones públicas
void I2C_init(void);
uint8_t I2C_start(uint8_t address);
uint8_t I2C_repeated_start(uint8_t address);
void I2C_stop(void);
uint8_t I2C_write(uint8_t data);
uint8_t I2C_read_ack(void);
uint8_t I2C_read_nack(void);

#endif /* I2C_LIB_H_ */
