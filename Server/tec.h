#ifndef __TEC_H_
#define __TEC_H_

#include <stdint.h>

void tec_init(void);
void tec_enable(bool on_off_n);
bool tec_is_enabled(void);
#endif // __TEC_H_