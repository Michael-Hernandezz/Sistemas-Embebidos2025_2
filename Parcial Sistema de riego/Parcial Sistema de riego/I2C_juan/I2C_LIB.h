#ifndef I2C_LIB_H_
#define I2C_LIB_H_

#define F_CPU 16000000UL
#include <avr/io.h>

// ===============================
// TWI Bit Rate Register (TWBR)
// ===============================
typedef union TWBR_t {
	
	struct {
		unsigned char TWBRn0 : 1;
		unsigned char TWBRn1 : 1;
		unsigned char TWBRn2 : 1;
		unsigned char TWBRn3 : 1;
		unsigned char TWBRn4 : 1;
		unsigned char TWBRn5 : 1;
		unsigned char TWBRn6 : 1;
		unsigned char TWBRn7 : 1;
	};
	
	struct {
		unsigned char TWBRn : 8;
	};
} volatile *TWBRbits_t;


// ===============================
// TWI Control Register (TWCR)
// ===============================
typedef union TWCR_t {
	struct {
		unsigned char TWIEn  : 1; 
		unsigned char		 : 1;
		unsigned char TWENn  : 1; 
		unsigned char TWWCn  : 1; 
		unsigned char TWSTOn : 1; 
		unsigned char TWSTAn : 1; 
		unsigned char TWEAn  : 1;
		unsigned char TWINTn : 1; 
	};
} volatile *TWCRbits_t;

// ===============================
// TWI Status Register (TWSR)
// ===============================
typedef union TWSR_t{
	
	struct {
		unsigned char TWPS00  : 1; 
		unsigned char TWPS01  : 1;
		unsigned char		  : 1;
		unsigned char TWS03	  : 1; 
		unsigned char TWS04	  : 1; 
		unsigned char TWS05	  : 1; 
		unsigned char TWS06	  : 1;
		unsigned char TWS07	  : 1;  
	};
	
	struct {
		unsigned char TWPS  : 2; 
		unsigned char       : 1;
		unsigned char TWSn  : 5; 
	};
} volatile *TWSRbits_t;

// ===============================
// TWI Data Register (TWDR)
// ===============================
typedef union TWDR_t{
	
	struct {
		unsigned char TWD00 : 1;
		unsigned char TWD01 : 1;
		unsigned char TWD02 : 1;
		unsigned char TWD03 : 1;
		unsigned char TWD04 : 1;
		unsigned char TWD05 : 1;
		unsigned char TWD06 : 1;
		unsigned char TWD07 : 1;
	};
	
	struct {
		unsigned char TWDn : 8;
	};
} volatile *TWDRbits_t;

// ===============================
// TWI (Slave) Address Register (TWAR)
// ===============================
typedef union TWAR_t{
	struct {
		unsigned char TWGCEn : 1; 
		unsigned char TWA00  : 1; 
		unsigned char TWA02  : 1; 
		unsigned char TWA03  : 1; 
		unsigned char TWA04  : 1; 
		unsigned char TWA05  : 1;
		unsigned char TWA06  : 1;  
	};
	struct {
		unsigned char		 : 1; 
		unsigned char TWAn   : 7; 
	};
} volatile *TWARbits_t;

// ===============================
// TWI Address Mask Register (TWAMR)
// ===============================
typedef union TWAMR_t{
	struct {
		unsigned char       : 1;
		unsigned char TWAM  : 7; 
	};
} volatile *TWAMRbits_t;

// ===============================
// Macros para acceso directo
// ===============================
#define TWBRbits  (*(TWBRbits_t)_SFR_MEM_ADDR(TWBR))
#define TWSRbits  (*(TWSRbits_t)_SFR_MEM_ADDR(TWSR))
#define TWARbits  (*(TWARbits_t)_SFR_MEM_ADDR(TWAR))
#define TWDRbits  (*(TWDRbits_t)_SFR_MEM_ADDR(TWDR))
#define TWCRbits  (*(TWCRbits_t)_SFR_MEM_ADDR(TWCR))
#define TWAMRbits (*(TWAMRbits_t)_SFR_MEM_ADDR(TWAMR))

// ===============================
// METODOS
// ===============================
void I2C_Init(unsigned long scl_freq);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(unsigned char data);
unsigned char I2C_Read_ACK(void);
unsigned char I2C_Read_NACK(void);



#endif /* I2C_LIB_H_ */