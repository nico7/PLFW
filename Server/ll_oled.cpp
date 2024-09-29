#include "ll_oled.h"

#include <Arduino.h>
#include <SPI.h>

#include "gpios.h"

const SPISettings spiSettings(2000000, MSBFIRST, SPI_MODE0);

static uint8_t myScreen [64] [128] = {0};

void ll_oled_init(void)
{
  uint16_t r, c;
  uint8_t my_byte[1];

  digitalWrite(OLED_RST_N, LOW);
  delay(10);
  digitalWrite(OLED_RST_N, HIGH);
  delay(1);
  
  // All "Recommendations" were made by Chatgpt lol
  ll_oled_start();
  ll_oled_send_cmd(SET_CLK_DIV_RATIO_FMODE);
  ll_oled_send_cmd(RECOMMENDED_DIV_RATIO);
  ll_oled_send_cmd(SET_MULTIPLEX_ROT_MODE);
  ll_oled_send_cmd(RECOMMENDED_MULTIPLEX); 
  ll_oled_send_cmd(SET_DISP_OFFSET_MODE);
  ll_oled_send_cmd(RECOMMENDED_OFFSET); 
  ll_oled_send_cmd(SET_DC_DC_MODE);
  ll_oled_send_cmd(SET_DC_DC_ON);


SPI.transfer(0xD5);          // Set Display Clock Divide Ratio/Oscillator Frequency
SPI.transfer(0x80);          // Recommended oscillator setting

SPI.transfer(0xA8);          // Set Multiplex Ratio
SPI.transfer(0x3F);          // Multiplex ratio for 128x64 (64MUX)

SPI.transfer(0xD3);          // Set Display Offset
SPI.transfer(0x00);          // No offset

SPI.transfer(0x40);          // Set Display Start Line (0x40 | 0x00)

SPI.transfer(0xAD);          // Set DC-DC control mode
SPI.transfer(0x8A);          // DC-DC on

SPI.transfer(0xA1);          // Set Segment Re-map (column address 127 is mapped to SEG0)

SPI.transfer(0xC8);          // Set COM Output Scan Direction (normal)

SPI.transfer(0xDA);          // Set COM Pins Hardware Configuration
SPI.transfer(0x12);          // Alternative COM pin configuration

SPI.transfer(0x81);          // Set Contrast Control
SPI.transfer(0xFF);          // Set contrast

SPI.transfer(0xD9);          // Set Pre-charge Period
SPI.transfer(0xF1);          // Pre-charge period settings

SPI.transfer(0xDB);          // Set VCOMH Deselect Level
SPI.transfer(0x40);          // VCOMH level

SPI.transfer(0xA5);          // Entire display ON (resume to RAM content display)

SPI.transfer(0xA7);          // Set Normal Display mode (A7 for Inverse display)

SPI.transfer(0xAF);          // Display ON
  

  // for(r = 0; r < SCREEN_HEIGHT; r++)
  // {
  //   for(c = 0; c < SCREEN_WIDTH; c++)
  //   {
  //      if(((r + c) * 5) %7 == 0)
  //      {
  //       myScreen[r][c] = 1;
  //      }
  //   }
  // }
  my_byte[0] = 0xAA;

  for(r =  0; r < 1024; r++)
  {
    
    ll_oled_send_data(my_byte, 1);
  }

  ll_oled_end();

}

void ll_oled_start(void)
{
  SPI.begin(PL_SCK, PL_MISO, PL_MOSI);
  digitalWrite(OLED_CS_N, LOW);
  SPI.beginTransaction(spiSettings);
}

void ll_oled_end(void)
{
  digitalWrite(OLED_CS_N, HIGH);
  SPI.endTransaction();
  SPI.end();
}

void ll_oled_send_cmd( uint8_t cmd)
{
  digitalWrite(DATA_CMD_N, LOW);
  SPI.transfer(cmd);
}

void ll_oled_send_data(uint8_t * data, uint16_t size)
{
  uint16_t i;

  digitalWrite(DATA_CMD_N, HIGH);

  for(i = 0; i < size; i++)
  {
    SPI.transfer(data[i]);
  }

}