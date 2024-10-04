#include "tec.h"

#include <Arduino.h>
#include "gpios.h"
#include "dac.h"

void tec_init(void)
{
  dac_set_current(TEC, 0);
}

void tec_enable(bool on_off_n)
{
  if(true == on_off_n)
  {
    digitalWrite(TEC_ENABLE, HIGH);
  }
  else
  {
    digitalWrite(TEC_ENABLE, LOW);
  }
}