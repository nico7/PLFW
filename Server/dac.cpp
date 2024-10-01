#include "dac.h"

#include <Arduino.h>
#include <SPI.h>

#include "gpios.h"
#include "oled.h"

const SPISettings spiSettings(4000000, MSBFIRST, SPI_MODE0);


static void ll_dac_start(void) 
{
  SPI.begin(PL_SCK, PL_MISO, PL_MOSI);
  digitalWrite(DAC_CS_N, LOW);
  SPI.beginTransaction(spiSettings);
}

static void ll_dac_end(void) 
{
  digitalWrite(DAC_CS_N, HIGH);
  SPI.endTransaction();
  SPI.end();
}

static void dac_config(uint8_t cmd, uint8_t *data)
{
  digitalWrite(DAC_LATCH_N, LOW);
   SPI.transfer(cmd);
   data[0] = SPI.transfer(data[0]);
   data[1] = SPI.transfer(data[1]);
   digitalWrite(DAC_LATCH_N, HIGH);
}

void dac_read(uint8_t cmd, uint8_t * data)
{
    dac_config(cmd | DAC_READ, data);
}

void dac_write(uint8_t cmd, uint8_t * data)
{
    dac_config(cmd | DAC_WRITE, data);
}

void dac_init(void)
{
  uint8_t buffer[2] = {0, 0 };
  char str [20];

  ll_dac_start();

  //dac_config(TEC_REG, buffer);
  //dac_config(LASER_REG, buffer);
  
  buffer[0] = 0x00;
  buffer[1] = 0x03;
  dac_write(VREF_REG, buffer);
  
  buffer[0] = 0x01;
  buffer[1] = 0x00;
  dac_read(GAIN_REG, buffer);
  dac_write(GAIN_REG, )
  ll_dac_end();
  sprintf(str, "0x%X%X\0", buffer[0], buffer[1]);
  ll_oled_clear(CLEAR_OLED);
  oled_print((uint8_t *)"This works maybe\n\r", 18);
  oled_print((uint8_t *)str, sizeof(str));
}