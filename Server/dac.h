#ifndef __DAC_H_
#define __DAC_H_

#include <stdint.h>

#define LASER_MAX_mA   480 // mA
#define TEC_MAX_mA    2000 // mA

#define TEC_REG       0x00 //0x00 << 3
#define LASER_REG     0x08 //0x01 << 3
#define VREF_REG      0x40 //0x08 << 3
#define PD_REG        0x48 //0x09 << 3
#define GAIN_REG      0x50 //0x0A << 3

#define DAC_WRITE     0x00
#define DAC_READ      0x06

#define VREF_VREFPIN  0x0F
#define POWER_UP      0x0F
#define GAINx1        0x00
#define GAINx2        0x03

typedef enum dac_signal
{
  TEC   = 0,
  LASER = 1,
} dac_signal_E;

void dac_init(void);
void dac_read(uint8_t cmd, uint8_t * data);
void dac_write(uint8_t cmd, uint8_t * data);
void dac_setpoint(uint8_t device, uint8_t *value);
void dac_set_current(dac_signal_E device, uint16_t mA);
uint16_t dac_get_currentmA(dac_signal_E device);

#endif // __DAC_H_