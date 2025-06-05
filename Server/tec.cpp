#include "tec.h"

#include <Arduino.h>
#include "gpios.h"
#include "dac.h"

static bool m_tec_enabled = false;

void tec_init(void)
{
  dac_set_current(TEC, 0);
}

void tec_enable(bool on_off_n)
{
  if(true == on_off_n)
  {
    digitalWrite(TEC_ENABLE, HIGH);
    m_tec_enabled = true;
  }
  else
  {
    digitalWrite(TEC_ENABLE, LOW);
    m_tec_enabled = false;
  }
}

bool tec_is_enabled(void)
{
  return m_tec_enabled;
}