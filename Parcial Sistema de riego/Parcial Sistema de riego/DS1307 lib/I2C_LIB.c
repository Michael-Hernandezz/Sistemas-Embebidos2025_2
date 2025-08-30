#include "I2C_LIB.h"

// Estado global del bus I2C
I2CStatus_t I2CStatus = { .all_flags = 0 };

void I2C_init(void) {
	TWSR = 0x00; // Preescaler = 1
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2; // Fórmula para SCL freq
	TWCR = (1 << TWEN); // Habilitar TWI
}

uint8_t I2C_start(uint8_t address) {
	// Enviar condición de START
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))); // Esperar fin de transmisión

	if ((TWSR & 0xF8) != TW_START) {
		I2CStatus.bits.start_sent = 0;
		return 0;
	}
	I2CStatus.bits.start_sent = 1;

	// Enviar dirección
	TWDR = address;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	uint8_t status = TWSR & 0xF8;
	if ((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK)) {
		I2CStatus.bits.slave_ack = 0;
		return 0;
	}
	I2CStatus.bits.slave_ack = 1;
	return 1;
}

uint8_t I2C_repeated_start(uint8_t address) {
	// Repetir START
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != TW_REP_START) {
		I2CStatus.bits.repeated_start = 0;
		return 0;
	}
	I2CStatus.bits.repeated_start = 1;

	// Enviar dirección
	TWDR = address;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	uint8_t status = TWSR & 0xF8;
	if ((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK)) {
		I2CStatus.bits.slave_ack = 0;
		return 0;
	}
	I2CStatus.bits.slave_ack = 1;
	return 1;
}

void I2C_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while (TWCR & (1 << TWSTO)); // Esperar que se envíe STOP
}

uint8_t I2C_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
		I2CStatus.bits.data_ack = 0;
		return 0;
	}
	I2CStatus.bits.data_ack = 1;
	return 1;
}

uint8_t I2C_read_ack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t I2C_read_nack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}
