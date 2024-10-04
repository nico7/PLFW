#include "back_end.h"
#include "dac.h"
#include "gpios.h"
#include "oled.h"
#include "tec.h"

#include "driver/adc.h"

#include <string.h>

unsigned long myTime = millis();
const int output18 = 18;


bool oled_initialized = false;

TaskHandle_t Task1;

void setup()
{
  Serial.begin(115200);
  // Initialize the output variables as outputs
  gpios_init(GPIOS_UART);
  ll_oled_init();
  dac_init();
  tec_init();
  xTaskCreatePinnedToCore(
    ADC_STUFF,
    "ADC_STUFF",
    10000,
    NULL,
    1,
    &Task1,
    0
  );
  delay(500);

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_6);
  oled_initialized = true;
}

void ADC_STUFF(void * pvParameters)
{
  uint8_t str[20];
  while(1)
  {

    
    if(oled_initialized)
    {
      
      myTime = micros();
      int adcValue = adc1_get_raw(ADC1_CHANNEL_2);
      
      myTime = micros() - myTime; 
      ll_oled_clear(1);
      sprintf((char *) str, "ADC  0x%X", adcValue);
      oled_print(str, sizeof(str));
      //Serial.println(adcValue >> 4);
      
    }
      delay(100);
  }
}

void loop()
{
  back_end_sm();

  if(digitalRead(UP_BTN) == 0)
  {
    Serial.println("UP_pressed!\n");
    delay(300);
    dac_set_current(TEC, 600);
    tec_enable(true);
  }
  else
  {
   // dac_set_current(TEC, 0);
    //tec_enable(false);
  }
}


