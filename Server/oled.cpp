#include "oled.h"

void oled_print(uint8_t * str, uint16_t size)
{
  ll_oled_start();
  ll_oled_print(str, size);
  ll_oled_display_screen();
  ll_oled_end();
}