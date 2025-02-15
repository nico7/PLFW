#include "back_end.h"
#include "adc.h"
#include "dac.h"
#include "gpios.h"
#include "laser.h"
#include "oled.h"
#include "tec.h"

#include "driver/adc.h"

#include <string.h>
#include <WiFiManager.h>
#include <WiFi.h>

unsigned long myTime = millis();
const int output18 = 18;
uint16_t adc_val = 0;

int len;
char adc_data[50];
char screen_data[50] = "\n\rSSID: FN530\n\rPW: holograms\n\rIP: 196.168.4.1";
bool wifi_connection = false;

TaskHandle_t Task1;

void setup()
{
  // Initialize the output variables as outputs
  gpios_init();
  ll_oled_init();
  
  WiFi.mode(WIFI_STA);
  
  Serial.begin(115200);

  oled_print((uint8_t *) screen_data, strlen(screen_data));
  
  //WiFiManager, Local initialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  //wm.resetSettings();

  //Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ("AutoConnectAP"),
  // if empty, it will auto generate SSID, if password is blank, it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("FN530", "holograms"); // password protected ap
  

  ll_oled_clear(CLEAR_RESET);
  if(!res)
  {
    oled_print((uint8_t *) "Failed to connect\n\r", 19);
  }
  else
  {
    oled_print((uint8_t *) "Connected\n\rIP: ", 15);
  }

  if(res)
  {
    do
    {
      wifi_connection = (WiFi.status() == WL_CONNECTED);
      Serial.print("awaiting connection...\r");
      delay(1000);

    }while(wifi_connection == false);
  }

  dac_init();
  tec_init();
  laser_init();
  delay(500);
  
}



void loop()
{
  if(wifi_connection)
  {
    back_end_sm();
  }

  int up_state = digitalRead(UP_BTN);
  int down_state = digitalRead(DOWN_BTN);
  if(LOW == up_state)
  {
    Serial.println("UP_pressed!\n");
    delay(500);
    dac_set_current(TEC, 500);
    dac_set_current(LASER, 25);
    tec_enable(true);
    laser_enable(true);
  }
  else if(LOW == down_state)
  {
    Serial.println("DOWN pressed\n");
    delay(500);
    
    dac_set_current(TEC, 0);
    dac_set_current(LASER, 0);
    tec_enable(false);
    laser_enable(false);
    
  }

  adc_read(ADC_TEC, &adc_val);
  sprintf(adc_data, "TEC = 0x%X\r\n", adc_val);
   len = strlen(adc_data);

  adc_read(ADC_LSR, &adc_val);
  sprintf(adc_data + len, "LSR = 0x%X\r\n", adc_val);
  len = strlen(adc_data);
  
  adc_read(ADC_THR, &adc_val);
  sprintf(adc_data + len, "THRM = 0x%X\r\n", adc_val);
  len = strlen(adc_data);

  char uart_prnt[30] = "LASER = ";
  sprintf(uart_prnt + strlen(uart_prnt), "%d ", adc_val);
  //Serial.print(uart_prnt);
  //Serial.print('\r');
  
  adc_read(ADC_HTR, &adc_val);
  sprintf(adc_data + len, "HEATER = 0x%X\r\n", adc_val);
  len = strlen(adc_data);
  
  
  ll_oled_clear(true);
  //oled_print((uint8_t *) adc_data, len);

  delay(100);
}


