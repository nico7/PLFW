#include "laser.h"

#include <Arduino.h>
#include "gpios.h"
#include "dac.h"

static bool m_laser_enabled = false;

void laser_init(void)
{
  dac_set_current(LASER, 0);
}

void laser_enable(bool on_off_n)
{
  if(true == on_off_n)
  {
    digitalWrite(LASER_ENABLE, HIGH);
    m_laser_enabled = true;
  }
  else
  {
    digitalWrite(LASER_ENABLE, LOW);
    m_laser_enabled = false;
  }
}

bool laser_is_enabled(void)
{
  return m_laser_enabled;
}