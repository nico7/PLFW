#include "back_end.h"
#include "gpios.h"
#include "ll_oled.h"

const int output18 = 18;


void setup()
{
  Serial.begin(115200);

  // Initialize the output variables as outputs
  gpios_init(GPIOS_UART);
  ll_oled_init();
}

void loop()
{
  back_end_sm();
  
}


