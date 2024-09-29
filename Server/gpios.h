#ifndef __GPIOS_H_
#define __GPIOS_H_

#include <stdint.h>

#define GPIOS_UART        0
#define GPIOS_NO_UART     1

#define UP_BUTTON     0
#define DOWN_BUTTON   1
#define THERMISTOR    2
#define LASER_MON     3

#define TEC_MON       4
#define DAC_LATCH_N   5
#define TEC_ENABLE    6
#define FAULT_N       7

// PIN 8 is part of a boot_sequence pin
#define OLED_RST_N    9
#define LASER_ENABLE  10
#define SHUTTER       11

#define DAC_CS_N      15

#define SELECT        16
#define BACK          17
#define DATA_CMD_N    18
#define OLED_CS_N     19

#define PL_MOSI          20
#define PL_SCK           21
#define PL_MISO          22
#define FRAM_CS_N     23


void gpios_init(uint8_t using_uart);


#endif //__GPIOS_H_