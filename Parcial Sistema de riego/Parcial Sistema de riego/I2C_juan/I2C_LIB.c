#include "I2C_LIB.h"


void I2C_Init(unsigned long scl_freq) {

	unsigned long twbr_val = ((F_CPU / scl_freq) - 16) / 2;

	TWBRbits.TWBRn = (unsigned char)twbr_val;
	TWSRbits.TWPS = 0; 
	TWCRbits.TWENn = 1; 
}

void I2C_Start(void) {
	TWCRbits.TWSTAn = 1; 
	TWCRbits.TWINTn = 1; 
	while (!TWCRbits.TWINTn); 
}

void I2C_Stop(void) {
	TWCRbits.TWSTOn = 1; 
	TWCRbits.TWINTn = 1; 
}

void I2C_Write(unsigned char data) {
	TWDRbits.TWDn = data;
	TWCRbits.TWINTn = 1; 
	while (!TWCRbits.TWINTn);
}

unsigned char I2C_Read_ACK(void) {
	TWCRbits.TWEAn = 1; 
	TWCRbits.TWINTn = 1; 
	while (!TWCRbits.TWINTn);
	return TWDRbits.TWDn;
}

unsigned char I2C_Read_NACK(void) {
	TWCRbits.TWEAn = 0;  
	TWCRbits.TWINTn = 1; 
	while (!TWCRbits.TWINTn);
	return TWDRbits.TWDn;
}
