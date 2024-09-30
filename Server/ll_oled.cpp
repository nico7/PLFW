#include "ll_oled.h"

#include <Arduino.h>
#include <SPI.h>
#include <string.h>

#include "gpios.h"
#include "symbols.h"

const SPISettings spiSettings(2000000, MSBFIRST, SPI_MODE0);

static uint8_t myScreen[64][128] = { 0 };
static uint8_t rastScreen[NUM_PAGES][128] = {0};

static uint8_t scrx, scry;    // These keep track of where symbols are written on the screen

void ll_oled_init(void) {
  scrx = 0;
  scry = 0;
  Serial.begin(115200);
  digitalWrite(OLED_RST_N, LOW);
  delay(10);
  digitalWrite(OLED_RST_N, HIGH);
  delay(1);

  // All "Recommendations" were made by Chatgpt lol
  ll_oled_start();
  /*ll_oled_send_cmd(SET_CLK_DIV_RATIO_FMODE);
 */

  ll_oled_send_cmd(SET_MULTIPLEX_ROT_MODE);
  ll_oled_send_cmd(RECOMMENDED_MULTIPLEX);

  ll_oled_send_cmd(SET_DC_DC_MODE);
  ll_oled_send_cmd(SET_DC_DC_OFF);
  ll_oled_send_cmd(SET_DISP_OFFSET_MODE);
  
  ll_oled_send_cmd(RECOMMENDED_OFFSET);
  ll_oled_send_cmd(SET_DISP_START_LN);
  ll_oled_send_cmd(SET_CONTRAST_CTRL_MODE);
  ll_oled_send_cmd(MOST_CONTRAST);
  
  ll_oled_send_cmd(SET_PRE_CHRG_MODE);
  ll_oled_send_cmd(RECOMMENDED_PRE_CHG);  // Longer pre-charge for brighter pixels
  ll_oled_send_cmd(SET_COM_PADS_CFG_MODE);
  ll_oled_send_cmd(SET_COM_PADS_CFG_DATA);
  
  ll_oled_send_cmd(SET_VCOM_DESEL_LVL_MODE);
  ll_oled_send_cmd(SET_VCOM_DESEL_LVL_DATA);
  ll_oled_send_cmd(SET_NRML_DISP);
  ll_oled_send_cmd(DISPLAY_ON);

  ll_oled_clear();
  ll_oled_print((uint8_t *)"AcBCDEabcdeaaBBOoBBaaa", 20);
  ll_oled_display_screen();
  
  ll_oled_end();
}

void ll_oled_start(void) {
  SPI.begin(PL_SCK, PL_MISO, PL_MOSI);
  digitalWrite(OLED_CS_N, LOW);
  SPI.beginTransaction(spiSettings);
}

void ll_oled_end(void) {
  digitalWrite(OLED_CS_N, HIGH);
  SPI.endTransaction();
  SPI.end();
}

void ll_oled_send_cmd(uint8_t cmd) {
  digitalWrite(DATA_CMD_N, LOW);
  SPI.transfer(cmd);
}

void ll_oled_send_data(uint8_t* data, uint16_t size) {
  uint16_t i;

  digitalWrite(DATA_CMD_N, HIGH);

  for (i = 0; i < size; i++) {
    SPI.transfer(data[i]);
  }
}

void rasterize(void)
{
  uint8_t page, col, row;


  for(page = 0; page < NUM_PAGES; page++)
  {
    row = page * PAGE_HEIGHT;
    for(col = 0; col < SCREEN_WIDTH; col++)
    {
      rastScreen[page][col] = ((myScreen[row][col] & 0x01) ) | ((myScreen[row + 1][col] & 0x01) << 1) | ((myScreen[row + 2][col] & 0x01) << 2) | ((myScreen[row + 3][col] & 0x01) << 3)
                              | ((myScreen[row + 4][col] & 0x01) << 4) | ((myScreen[row + 5][col] & 0x01) << 5) | ((myScreen[row + 6][col] & 0x01) << 6) | ((myScreen[row + 7][col] & 0x01) << 7);
    }
  }
}

void ll_oled_display_screen(void)
{
  uint8_t col, page;
  uint32_t ran;

  rasterize();


  for(page = 0; page < NUM_PAGES; page++)
  {
    ll_oled_send_cmd(SET_PAGE_ADDR | page);
    ll_oled_send_cmd(SET_LOW_COL_ADDR + 2 );
    ll_oled_send_cmd(SET_HIGH_COL_ADDR);

    ll_oled_send_data(rastScreen[page], SCREEN_WIDTH);
  }
}

void ll_oled_clear(void)
{
    memset(myScreen, 0, sizeof(myScreen));
}

static void ll_draw_char( const uint8_t * charArray, uint8_t charWidth)
{
  uint8_t height, width;

  if(scrx + A_10_WIDTH > SCREEN_WIDTH)
  {
    scrx = 0;

    if(scry + LINE_HEIGHT > SCREEN_HEIGHT)
    {
      scry = 0;
    }
    else
    {
      scry += LINE_HEIGHT;
    }
  }
  for(height = 0; height < SYMBOL_HEIGHT_10; height++)
  {
    for(width = 0; width < charWidth; width++)
    {
      myScreen[height + scry][ width + scrx] = (charArray[height] >> (7 - width));
    }
  }
  scrx += A_10_WIDTH + 1;
}

void ll_oled_print(uint8_t * string, uint16_t size)
{
  uint16_t i;
  uint8_t height, width;
  

  for(i = 0; i < size; i++)
  {
  
    height = 0;
    width = 0;
    switch(string[i])
    {
      case ' ':
        scrx += 3;
        break;
      case '\n':
        break;
      case 'e':
        ll_draw_char( e_10, e_10_WIDTH);
        break;
      case 't':
        break;
      case 'o':
        ll_draw_char( o_10, o_10_WIDTH);
        break;
      case 'a':
        ll_draw_char( a_10, a_10_WIDTH);
        break;
      case 'i':
        break;
      case 'n':
        break;
      case 's':
        break;
      case 'h':
        break;
      case 'r':
        break;
      case 'l':
        break;
      case 'd':
        ll_draw_char( d_10, d_10_WIDTH);
        break;
      case 'u':
        break;
      case 'm':
        break;
      case 'y':
        break;
      case 'g':
        break;
      case 'c':
        ll_draw_char( c_10, c_10_WIDTH);
        break;
      case 'w':
        break;
      case 'f':
        break;
      case 'b':
        ll_draw_char( b_10, b_10_WIDTH);
        break;
      case 'p':
        break;
      case 'k':
        break;
      case 'v':
        break;
      case 'I':
        break;
      case 'M':
        break;
      case 'T':
        break;
      case 'J':
        break;
      case 'H':
        break;
      case 'A':
         ll_draw_char( A_10, A_10_WIDTH);
        break;
      case 'W':
        break;
      case 'S':
        break;
      case 'Y':
        break;
      case 'E':
        ll_draw_char( E_10, E_10_WIDTH);
        break;
      case 'j':
        break;
      case 'N':
        break;
      case 'P':
        break;
      case 'C':
          ll_draw_char( C_10, C_10_WIDTH);
        break;
      case 'R':
        break;
      case 'x':
        break;
      case 'B':
          ll_draw_char( B_10, B_10_WIDTH);
        break;
      case 'q':
        break;
      case 'D':
          ll_draw_char( D_10, D_10_WIDTH);
        break;
      case 'O':
          ll_draw_char( O_10, O_10_WIDTH);
        break;
      case 'L':
        break;
      case 'G':
        break;
      case 'F':
        break;
      case 'z':
        break;
      case 'U':
        break;
      case 'K':
        break;
      case 'V':
        break;
      case 'Q':
        break;
      case 'X':
        break;
      case 'Z':
        break;
      default:
        break;
    }
  }


}