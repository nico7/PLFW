#include "menu.h"

#include <WiFi.h>
#include <string.h>

#include "adc.h"
#include "oled.h"


typedef enum menu_nomenu {
  MENU_CONNECTED,
  MENU_VALUES,
  MENU_MENU_1,
  MENU_MENU_2,
  MENU_NSTATES,
} main_menu_E;


static main_menu_E screen_state = MENU_CONNECTED;
static main_menu_E prev_screen_state = MENU_NSTATES;

static uint8_t main_menu_pos = 0;
static uint8_t sub_menu_pos = 0;
static uint8_t sub_sub_menu_pos = 0;

static uint16_t m_update_time = 500;

static unsigned long time_prev = 0;

char str_helper[20];
uint16_t value;

void menu_sm(uint8_t button_pressed) 
{
  if (millis() - time_prev > m_update_time) 
  {

    if(button_pressed > NONE_PRESSED)
    {
      Serial.print("button pressed = ");
      Serial.println(button_pressed);
    }
    

    time_prev = millis();
    switch (screen_state) 
    {
      case MENU_CONNECTED:
        if(prev_screen_state != MENU_CONNECTED)
        {
          ll_oled_clear(CLEAR_RESET);
          char ip_address[16];
          Serial.println(WiFi.localIP());
          snprintf(ip_address, sizeof(ip_address), "%d.%d.%d.%d",
                  WiFi.localIP()[0], WiFi.localIP()[1],
                  WiFi.localIP()[2], WiFi.localIP()[3]);
          oled_print((uint8_t *)"IP:", 3);
          oled_print((uint8_t *)ip_address, sizeof(ip_address));
          strcpy(str_helper, "\n\n\r     MENU    \x0F\r\n");
          oled_print((uint8_t *)str_helper, strlen(str_helper));
          strcpy(str_helper, "     DATA    \x1B");
          oled_print((uint8_t *)str_helper, strlen(str_helper));
        
          prev_screen_state = screen_state;
        }

        if (LEFT_PRESSED == button_pressed) 
        {
          screen_state = MENU_VALUES;
        } else if (RIGHT_PRESSED == button_pressed) 
        {
          screen_state = MENU_MENU_1;
        }
        break;
      case MENU_VALUES:
        ll_oled_clear(CLEAR_RESET);
        strcpy(str_helper, "VALUES | MAIN MENU \x0F\n\r");
        oled_title((uint8_t *) str_helper, strlen(str_helper), 9);
        value = adc_value(ADC_LSR);
        sprintf(str_helper, "LSR = %d\n\r", value);
        oled_print((uint8_t *)str_helper, strlen(str_helper));
        value = adc_value(ADC_TEC);
        sprintf(str_helper, "TEC = %d\n\r", value);
        oled_print((uint8_t *)str_helper, strlen(str_helper));
        value = adc_value(ADC_THR);
        sprintf(str_helper, "THR = %d\n\r", value);
        oled_print((uint8_t *)str_helper, strlen(str_helper));
        value = adc_value(ADC_HTR);
        sprintf(str_helper, "HTR = %d", value);
        oled_print((uint8_t *)str_helper, strlen(str_helper));
        
        
        if (RIGHT_PRESSED == button_pressed) 
        {
          prev_screen_state = MENU_VALUES;
          screen_state = MENU_CONNECTED;
        }
        break;
      case MENU_MENU_1:
        break;
      case MENU_MENU_2:
        break;
      default:
        Serial.println("something odd happened:\r\n");
        Serial.println(screen_state);
        screen_state = MENU_CONNECTED;
        break;
    }
  }
}