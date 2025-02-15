#ifndef __GPIOS_H_
#define __GPIOS_H_

#include <stdint.h>

#define UP_BTN        0
#define DOWN_BTN      1
#define P_N_FAULT     2
#define L_N_FAULT     3

#define SELECT_BTN    4
#define BACK_BTN      5
#define DAC_LATCH_N   6
#define TEC_ENABLE    7

#define ADC_CS_N      8
#define OLED_RST_N    9
#define LASER_ENABLE  10
#define SHUTTER       11

#define DAC_CS_N      15

#define ESP_TX        16
#define EXP_RX        17
#define DATA_CMD_N    18
#define OLED_CS_N     19

#define PL_MOSI       20
#define PL_SCK        21
#define PL_MISO       22
#define FRAM_CS_N     23


void gpios_init(void);


#endif //__GPIOS_H_