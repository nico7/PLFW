#include "adc.h"

#include <Arduino.h>
#include <SPI.h>

#include "gpios.h"

#define START_BIT  0x01
#define SINGLE  0x80

const SPISettings spiSettings(3600000, MSBFIRST, SPI_MODE0);

static void ll_adc_start(void) 
{
  digitalWrite(ADC_CS_N, LOW);
  SPI.begin(PL_SCK, PL_MISO, PL_MOSI);
  SPI.beginTransaction(spiSettings);
}

static void ll_adc_end(void)
{
  digitalWrite(ADC_CS_N, HIGH);
  SPI.endTransaction();
  SPI.end();
}

uint8_t adc_read(adc_signal_E ch, uint16_t * data)
{
  uint8_t buffer[3] = {0,0,0};
  uint8_t data_out[2] = {0,0};

  if(ch > 3)  // if the channel specified is greater than 3, then it does NOT exist
  {
    return 0xFF;    // so don't even try.
  }

  buffer [0] = START_BIT;
  buffer [1] = SINGLE | (((uint8_t) ch) <<4) ;
  
  ll_adc_start();
  data_out[0] = SPI.transfer(buffer[0]);
  data_out[0] = SPI.transfer(buffer[1]);
  data_out[1] = SPI.transfer(buffer[2]);

  ll_adc_end();


  
  *data = (uint16_t(data_out[0]) <<8) | data_out[1];

  return 0;
}