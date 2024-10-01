#ifndef __DAC_H_
#define __DAC_H_

#include <stdint.h>

#define TEC_REG     0x00 //0x00 << 3
#define LASER_REG   0x08 //0x01 << 3
#define VREF_REG    0x40 //0x08 << 3
#define PD_REG      0x48 //0x09 << 3
#define GAIN_REG    0x50 //0x0A << 3

#define DAC_WRITE   0x00
#define DAC_READ    0x06

#define GAIN_2

void dac_init(void);
void dac_read(uint8_t cmd, uint8_t * data);
void dac_write(uint8_t cmd, uint8_t * data);

#endif // __DAC_H_