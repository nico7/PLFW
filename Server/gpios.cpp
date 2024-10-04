#include "gpios.h"

#include <Arduino.h>


void gpios_init(uint8_t using_uart)
{
  pinMode(UP_BTN, INPUT);
  pinMode(DOWN_BTN, INPUT);
  pinMode(DAC_LATCH_N, OUTPUT);
  pinMode(TEC_ENABLE, OUTPUT);
  pinMode(FAULT_N, INPUT);
  pinMode(OLED_RST_N, OUTPUT);
  pinMode(LASER_ENABLE, OUTPUT);
  pinMode(SHUTTER, OUTPUT);
  pinMode(DAC_CS_N, OUTPUT);
  pinMode(DATA_CMD_N, OUTPUT);
  pinMode(OLED_CS_N, OUTPUT);
  pinMode(FRAM_CS_N, OUTPUT);
  
  if(GPIOS_NO_UART == using_uart)
  {
    pinMode(SELECT_BTN, INPUT);
    pinMode(BACK_BTN, INPUT);
  }
  
  digitalWrite(DAC_LATCH_N, HIGH);
  digitalWrite(TEC_ENABLE, LOW);
  digitalWrite(OLED_RST_N, LOW);
  digitalWrite(LASER_ENABLE, LOW);
  digitalWrite(SHUTTER, LOW);
  digitalWrite(DAC_CS_N, HIGH);
  
  digitalWrite(DATA_CMD_N, LOW);
  digitalWrite(OLED_CS_N, HIGH);
  digitalWrite(FRAM_CS_N, HIGH);
  
}