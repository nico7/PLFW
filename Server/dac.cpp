#include "dac.h"

#include <Arduino.h>
#include <SPI.h>
#include <string.h>

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
    uint8_t write_data[2];

    write_data[0] = data[0];
    write_data[1] = data[1];
    dac_config(cmd | DAC_WRITE, write_data);
}

void dac_init(void)
{
  uint8_t buffer[2] = {0, 0 };
  char str [20];

  ll_dac_start();
  
  //dac_config(TEC_REG, buffer);
  //dac_config(LASER_REG, buffer);
  
  buffer[0] = 0x00;
  buffer[1] = VREF_VREFPIN;
  dac_write(VREF_REG, buffer);
  
  dac_read(GAIN_REG, buffer);
  

  buffer[0] = GAINx2;
  buffer[1] = 0x00;
  dac_write(GAIN_REG, buffer);
  ll_dac_end();
  Serial.begin(115200);
  Serial.println(str);
  ll_oled_clear(CLEAR_OLED);
  strcpy(str, "The status reg = \n\r\0\0");
  oled_print((uint8_t *)str, sizeof(str));
  memset(str, 0, sizeof(str));
  sprintf(str, "0x%X%X\0", buffer[0], buffer[1]);
  oled_print((uint8_t *)str, sizeof(str));
  oled_print((uint8_t *)" ", 1);
}

void dac_setpoint(uint8_t device, uint8_t value)
{
  ll_dac_start();
  if(device == LASER)
  {
      dac_write(LASER_REG, value);
  }
  else
  {
      dac_write(TEC_REG, value);
  }
  ll_dac_end();

}

void dac_set_current(uint8_t device, uint16_t mA)
{
  uint8_t value;

  if(device == LASER)
  {
    value = (uint8_t) (mA/LASER_MAX_A);
  }
  else
  {
    value = (uint8_t) (ma/TEC_MAX_A);
  }

  dac_setpoint(device, value);
}