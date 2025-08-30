#include "ds1307.h"
#include "I2C_LIB.h"

// Inicializa el DS1307
void ds1307_init(void) {
	// Inicializar el bus I2C si no se ha hecho antes
	I2C_init();
	
	// Verificar que el oscilador esté activo (CH=0)
	ds1307_enable_oscillator(1);
}

// Habilitar/deshabilitar el oscilador (bit CH)
void ds1307_enable_oscillator(uint8_t enable) {
	// Leer el registro de segundos
	I2C_start(DS1307_ADDRESS << 1); // Dirección para escritura
	I2C_write(DS1307_REG_SECONDS);  // Registro de segundos
	I2C_stop();
	
	I2C_start((DS1307_ADDRESS << 1) | 0x01); // Dirección para lectura
	uint8_t seconds = I2C_read_nack();
	I2C_stop();
	
	// Modificar el bit CH (bit 7)
	if (enable)
	seconds &= ~(1 << 7); // Poner CH a 0 para habilitar
	else
	seconds |= (1 << 7);  // Poner CH a 1 para deshabilitar
	
	// Escribir de vuelta
	I2C_start(DS1307_ADDRESS << 1); // Dirección para escritura
	I2C_write(DS1307_REG_SECONDS);  // Registro
	I2C_write(seconds);             // Dato
	I2C_stop();
}

// Leer la hora actual del DS1307
void ds1307_get_time(ds1307_rtc *rtc) {
	I2C_start(DS1307_ADDRESS << 1); // Dirección para escritura
	I2C_write(DS1307_REG_SECONDS);  // Comenzar desde el registro de segundos
	I2C_stop();
	
	I2C_start((DS1307_ADDRESS << 1) | 0x01); // Dirección para lectura
	rtc->seg.valor_completo = I2C_read_ack() & 0x7F; // Ignorar bit CH
	rtc->min.valor_completo = I2C_read_ack();
	rtc->hr.valor_completo = I2C_read_ack();
	rtc->dia.valor_completo = I2C_read_ack();
	rtc->fec.valor_completo = I2C_read_ack();
	rtc->mes.valor_completo = I2C_read_ack();
	rtc->anio.valor_completo = I2C_read_nack();
	I2C_stop();
}

// Establecer la hora en el DS1307
void ds1307_set_time(ds1307_rtc *rtc) {
	uint8_t ch_bit = rtc->seg.CH; // Preservar estado del oscilador
	
	I2C_start(DS1307_ADDRESS << 1); // Dirección para escritura
	I2C_write(DS1307_REG_SECONDS);  // Comenzar desde el registro de segundos
	
	// Mantener el estado del bit CH
	if (ch_bit)
	I2C_write(rtc->seg.valor_completo | 0x80);
	else
	I2C_write(rtc->seg.valor_completo & 0x7F);
	
	I2C_write(rtc->min.valor_completo);
	I2C_write(rtc->hr.valor_completo);
	I2C_write(rtc->dia.valor_completo);
	I2C_write(rtc->fec.valor_completo);
	I2C_write(rtc->mes.valor_completo);
	I2C_write(rtc->anio.valor_completo);
	I2C_stop();
}

// Configura la salida de onda cuadrada (SQW)
void ds1307_set_sqw_output(uint8_t frequency) {
	I2C_start(DS1307_ADDRESS << 1); // Dirección para escritura
	I2C_write(DS1307_REG_CONTROL);  // Registro de control
	
	uint8_t control_byte = 0;
	
	// Configurar SQWE (bit 4) y los bits RS (0-1)
	control_byte = (1 << 4) | (frequency & 0x03);
	
	I2C_write(control_byte);
	I2C_stop();
}