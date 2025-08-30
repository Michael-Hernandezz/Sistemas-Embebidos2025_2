/*
 * ds1307.h
 *
 * Created: 30/08/2025 11:05:00 a. m.
 *  Author: miche
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#include <avr/io.h>
#include <stdint.h>

typedef uint8_t byte;

// Dirección I2C del DS1307
#define DS1307_ADDRESS 0x68

// Direcciones de los registros
#define DS1307_REG_SECONDS    0x00
#define DS1307_REG_MINUTES    0x01
#define DS1307_REG_HOURS      0x02
#define DS1307_REG_DAY        0x03
#define DS1307_REG_DATE       0x04
#define DS1307_REG_MONTH      0x05
#define DS1307_REG_YEAR       0x06
#define DS1307_REG_CONTROL    0x07

// Estructura para segundos (Registro 00h)
typedef union segundos_t {
    struct {
        byte seg0 :1;
        byte seg1 :1;
        byte seg2 :1;
        byte seg3 :1;
        byte seg10 :1; // Decenas de segundos bit 0
        byte seg11 :1; // Decenas de segundos bit 1
        byte seg12 :1; // Decenas de segundos bit 2
        byte CH :1;    // Bit de detención del oscilador
    };
    struct {
        byte segX :4;  // Unidades de segundos (0-9)
        byte seg1X :3; // Decenas de segundos (0-5)
        byte      :1;  // CH bit
    };
    byte valor_completo;
} segundos;

// Estructura para minutos (Registro 01h)
typedef union minutos_t {
    struct {
        byte min0 :1;
        byte min1 :1;
        byte min2 :1;
        byte min3 :1;
        byte min10 :1; // Decenas de minutos bit 0
        byte min11 :1; // Decenas de minutos bit 1
        byte min12 :1; // Decenas de minutos bit 2
        byte      :1;  // Sin uso, siempre 0
    };
    struct {
        byte minX :4;  // Unidades de minutos (0-9)
        byte min1X :3; // Decenas de minutos (0-5)
        byte      :1;  // Sin uso
    };
    byte valor_completo;
} minutos;

// Estructura para horas (Registro 02h)
typedef union horas_t {
    struct {
        byte hora0 :1;
        byte hora1 :1;
        byte hora2 :1;
        byte hora3 :1;
        byte hora10 :1;   // Decenas de hora bit 0 o AM/PM en modo 12h
        byte hora11 :1;   // Decenas de hora bit 1 o bit 10 horas en modo 12h
        byte modo_12_24 :1; // 0 = 24h, 1 = 12h
        byte      :1;     // Sin uso, siempre 0
    };
    struct {
        byte horaX :4;    // Unidades de hora
        byte hora1X :2;   // Decenas de hora o indicador AM/PM
        byte formato :1;  // Formato 12/24 horas
        byte      :1;     // Sin uso
    };
    struct {              // Estructura específica para modo 12h
        byte hora12_unidad :4;    // Unidades de hora (1-2)
        byte hora12_decena :1;    // Decenas de hora (0-1)
        byte am_pm :1;           // 0 = AM, 1 = PM
        byte es_12h :1;          // Debe ser 1 para 12h
        byte      :1;            // Sin uso
    };
    byte valor_completo;
} horas;

// Estructura para día de la semana (Registro 03h)
typedef union dia_semana_t {
    struct {
        byte dia0 :1;
        byte dia1 :1;
        byte dia2 :1;
        byte      :5;  // Sin uso, siempre 0
    };
    struct {
        byte diaX :3;  // Día de la semana (1-7)
        byte     :5;   // Sin uso
    };
    byte valor_completo;
} dia_semana;

// Estructura para fecha (Registro 04h)
typedef union fecha_t {
    struct {
        byte fecha0 :1;
        byte fecha1 :1;
        byte fecha2 :1;
        byte fecha3 :1;
        byte fecha10 :1;  // Decenas de fecha bit 0
        byte fecha11 :1;  // Decenas de fecha bit 1
        byte       :2;    // Sin uso, siempre 0
    };
    struct {
        byte fechaX :4;   // Unidades de fecha (0-9)
        byte fecha1X :2;  // Decenas de fecha (0-3)
        byte       :2;    // Sin uso
    };
    byte valor_completo;
} fecha;

// Estructura para mes (Registro 05h)
typedef union mes_t {
    struct {
        byte mes0 :1;
        byte mes1 :1;
        byte mes2 :1;
        byte mes3 :1;
        byte mes10 :1;    // Decenas de mes
        byte       :3;    // Sin uso, siempre 0
    };
    struct {
        byte mesX :4;     // Unidades de mes (0-9)
        byte mes1X :1;    // Decenas de mes (0-1)
        byte      :3;     // Sin uso
    };
    byte valor_completo;
} mes;

// Estructura para año (Registro 06h)
typedef union anio_t {
    struct {
        byte anio0 :1;
        byte anio1 :1;
        byte anio2 :1;
        byte anio3 :1;
        byte anio10 :1;   // Decenas de año bit 0
        byte anio11 :1;   // Decenas de año bit 1
        byte anio12 :1;   // Decenas de año bit 2
        byte anio13 :1;   // Decenas de año bit 3
    };
    struct {
        byte anioX :4;    // Unidades de año (0-9)
        byte anio1X :4;   // Decenas de año (0-9)
    };
    byte valor_completo;
} anio;

// Estructura para registro de control (Registro 07h)
typedef union control_t {
    struct {
        byte RS0 :1;      // Bit de selección de frecuencia 0
        byte RS1 :1;      // Bit de selección de frecuencia 1
        byte      :2;     // Sin uso, siempre 0
        byte SQWE :1;     // Square Wave Enable
        byte      :2;     // Sin uso, siempre 0
        byte OUT :1;      // Bit de salida
    };
    struct {
        byte rs_bits :2;  // Bits RS (0-3)
        byte      :2;     // Sin uso
        byte sqwe_bit :1; // Habilitación de onda cuadrada
        byte      :2;     // Sin uso
        byte out_bit :1;  // Salida
    };
    byte valor_completo;
} control;

// Estructura completa del RTC DS1307
typedef struct {
    segundos seg;
    minutos min;
    horas hr;
    dia_semana dia;
    fecha fec;
    mes mes;
    anio anio;
    control ctrl;
} ds1307_rtc;

// Macro para mapeo de registros
#define MACRO_MAPEO(STRUCT, SFR) (*(volatile STRUCT *)_SFR_MEM_ADDR(SFR))

// Definiciones de mapeo para acceso directo
#define PORTSeg MACRO_MAPEO(segundos, PORTSeg)
#define PORTMin MACRO_MAPEO(minutos, PORTMin)
#define PORTHora MACRO_MAPEO(horas, PORTHora)
#define PORTDia MACRO_MAPEO(dia_semana, PORTDia)
#define PORTFecha MACRO_MAPEO(fecha, PORTFecha)
#define PORTMes MACRO_MAPEO(mes, PORTMes)
#define PORTAnio MACRO_MAPEO(anio, PORTAnio)
#define PORTCtrl MACRO_MAPEO(control, PORTCtrl)

// Constantes para el control de la salida de onda cuadrada
#define SQW_1HZ   0x00
#define SQW_4KHZ  0x01
#define SQW_8KHZ  0x02
#define SQW_32KHZ 0x03

// Prototipos de funciones (puedes implementarlas en ds1307.c)
void ds1307_init(void);
void ds1307_set_time(ds1307_rtc *rtc);
void ds1307_get_time(ds1307_rtc *rtc);
void ds1307_enable_oscillator(uint8_t enable);
void ds1307_set_sqw_output(uint8_t frequency);

#endif /* DS1307_H_ *//*
 * ds1307.h
 *
 * Created: 30/08/2025 11:05:00 a. m.
 *  Author: miche
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#include <avr/io.h>
#include <stdint.h>

typedef uint8_t byte;

// Dirección I2C del DS1307
#define DS1307_ADDRESS 0x68

// Direcciones de los registros
#define DS1307_REG_SECONDS    0x00
#define DS1307_REG_MINUTES    0x01
#define DS1307_REG_HOURS      0x02
#define DS1307_REG_DAY        0x03
#define DS1307_REG_DATE       0x04
#define DS1307_REG_MONTH      0x05
#define DS1307_REG_YEAR       0x06
#define DS1307_REG_CONTROL    0x07

// Estructura para segundos (Registro 00h)
typedef union segundos_t {
    struct {
        byte seg0 :1;
        byte seg1 :1;
        byte seg2 :1;
        byte seg3 :1;
        byte seg10 :1; // Decenas de segundos bit 0
        byte seg11 :1; // Decenas de segundos bit 1
        byte seg12 :1; // Decenas de segundos bit 2
        byte CH :1;    // Bit de detención del oscilador
    };
    struct {
        byte segX :4;  // Unidades de segundos (0-9)
        byte seg1X :3; // Decenas de segundos (0-5)
        byte      :1;  // CH bit
    };
    byte valor_completo;
} segundos;

// Estructura para minutos (Registro 01h)
typedef union minutos_t {
    struct {
        byte min0 :1;
        byte min1 :1;
        byte min2 :1;
        byte min3 :1;
        byte min10 :1; // Decenas de minutos bit 0
        byte min11 :1; // Decenas de minutos bit 1
        byte min12 :1; // Decenas de minutos bit 2
        byte      :1;  // Sin uso, siempre 0
    };
    struct {
        byte minX :4;  // Unidades de minutos (0-9)
        byte min1X :3; // Decenas de minutos (0-5)
        byte      :1;  // Sin uso
    };
    byte valor_completo;
} minutos;

// Estructura para horas (Registro 02h)
typedef union horas_t {
    struct {
        byte hora0 :1;
        byte hora1 :1;
        byte hora2 :1;
        byte hora3 :1;
        byte hora10 :1;   // Decenas de hora bit 0 o AM/PM en modo 12h
        byte hora11 :1;   // Decenas de hora bit 1 o bit 10 horas en modo 12h
        byte modo_12_24 :1; // 0 = 24h, 1 = 12h
        byte      :1;     // Sin uso, siempre 0
    };
    struct {
        byte horaX :4;    // Unidades de hora
        byte hora1X :2;   // Decenas de hora o indicador AM/PM
        byte formato :1;  // Formato 12/24 horas
        byte      :1;     // Sin uso
    };
    struct {              // Estructura específica para modo 12h
        byte hora12_unidad :4;    // Unidades de hora (1-2)
        byte hora12_decena :1;    // Decenas de hora (0-1)
        byte am_pm :1;           // 0 = AM, 1 = PM
        byte es_12h :1;          // Debe ser 1 para 12h
        byte      :1;            // Sin uso
    };
    byte valor_completo;
} horas;

// Estructura para día de la semana (Registro 03h)
typedef union dia_semana_t {
    struct {
        byte dia0 :1;
        byte dia1 :1;
        byte dia2 :1;
        byte      :5;  // Sin uso, siempre 0
    };
    struct {
        byte diaX :3;  // Día de la semana (1-7)
        byte     :5;   // Sin uso
    };
    byte valor_completo;
} dia_semana;

// Estructura para fecha (Registro 04h)
typedef union fecha_t {
    struct {
        byte fecha0 :1;
        byte fecha1 :1;
        byte fecha2 :1;
        byte fecha3 :1;
        byte fecha10 :1;  // Decenas de fecha bit 0
        byte fecha11 :1;  // Decenas de fecha bit 1
        byte       :2;    // Sin uso, siempre 0
    };
    struct {
        byte fechaX :4;   // Unidades de fecha (0-9)
        byte fecha1X :2;  // Decenas de fecha (0-3)
        byte       :2;    // Sin uso
    };
    byte valor_completo;
} fecha;

// Estructura para mes (Registro 05h)
typedef union mes_t {
    struct {
        byte mes0 :1;
        byte mes1 :1;
        byte mes2 :1;
        byte mes3 :1;
        byte mes10 :1;    // Decenas de mes
        byte       :3;    // Sin uso, siempre 0
    };
    struct {
        byte mesX :4;     // Unidades de mes (0-9)
        byte mes1X :1;    // Decenas de mes (0-1)
        byte      :3;     // Sin uso
    };
    byte valor_completo;
} mes;

// Estructura para año (Registro 06h)
typedef union anio_t {
    struct {
        byte anio0 :1;
        byte anio1 :1;
        byte anio2 :1;
        byte anio3 :1;
        byte anio10 :1;   // Decenas de año bit 0
        byte anio11 :1;   // Decenas de año bit 1
        byte anio12 :1;   // Decenas de año bit 2
        byte anio13 :1;   // Decenas de año bit 3
    };
    struct {
        byte anioX :4;    // Unidades de año (0-9)
        byte anio1X :4;   // Decenas de año (0-9)
    };
    byte valor_completo;
} anio;

// Estructura para registro de control (Registro 07h)
typedef union control_t {
    struct {
        byte RS0 :1;      // Bit de selección de frecuencia 0
        byte RS1 :1;      // Bit de selección de frecuencia 1
        byte      :2;     // Sin uso, siempre 0
        byte SQWE :1;     // Square Wave Enable
        byte      :2;     // Sin uso, siempre 0
        byte OUT :1;      // Bit de salida
    };
    struct {
        byte rs_bits :2;  // Bits RS (0-3)
        byte      :2;     // Sin uso
        byte sqwe_bit :1; // Habilitación de onda cuadrada
        byte      :2;     // Sin uso
        byte out_bit :1;  // Salida
    };
    byte valor_completo;
} control;

// Estructura completa del RTC DS1307
typedef struct {
    segundos seg;
    minutos min;
    horas hr;
    dia_semana dia;
    fecha fec;
    mes mes;
    anio anio;
    control ctrl;
} ds1307_rtc;

// Macro para mapeo de registros
#define MACRO_MAPEO(STRUCT, SFR) (*(volatile STRUCT *)_SFR_MEM_ADDR(SFR))

// Definiciones de mapeo para acceso directo
#define PORTSeg MACRO_MAPEO(segundos, PORTSeg)
#define PORTMin MACRO_MAPEO(minutos, PORTMin)
#define PORTHora MACRO_MAPEO(horas, PORTHora)
#define PORTDia MACRO_MAPEO(dia_semana, PORTDia)
#define PORTFecha MACRO_MAPEO(fecha, PORTFecha)
#define PORTMes MACRO_MAPEO(mes, PORTMes)
#define PORTAnio MACRO_MAPEO(anio, PORTAnio)
#define PORTCtrl MACRO_MAPEO(control, PORTCtrl)

// Constantes para el control de la salida de onda cuadrada
#define SQW_1HZ   0x00
#define SQW_4KHZ  0x01
#define SQW_8KHZ  0x02
#define SQW_32KHZ 0x03

// Prototipos de funciones (puedes implementarlas en ds1307.c)
void ds1307_init(void);
void ds1307_set_time(ds1307_rtc *rtc);
void ds1307_get_time(ds1307_rtc *rtc);
void ds1307_enable_oscillator(uint8_t enable);
void ds1307_set_sqw_output(uint8_t frequency);

#endif /* DS1307_H_ */