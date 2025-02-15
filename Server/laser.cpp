#include "laser.h"

#include <Arduino.h>
#include "gpios.h"
#include "dac.h"

void laser_init(void)
{
  dac_set_current(LASER, 0);
}

void laser_enable(bool on_off_n)
{
  if(true == on_off_n)
  {
    digitalWrite(LASER_ENABLE, HIGH);
  }
  else
  {
    digitalWrite(LASER_ENABLE, LOW);
  }
}