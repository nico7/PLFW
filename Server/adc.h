#ifndef __ADC_H_
#define __ADC_H_

#include <stdint.h>

#define LASER_SENS_RES 210 //mOhms

typedef enum adc_signal {
ADC_TEC = 0,
ADC_LSR = 1,
ADC_THR = 2,
ADC_HTR = 3,
}adc_signal_E;

void adc_init(void);
uint8_t adc_read(adc_signal_E ch, uint16_t * data);
uint16_t adc_get(adc_signal_E ch);
uint16_t adc_value(adc_signal_E ch);

#endif //__ADC_H_