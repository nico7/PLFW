#ifndef __LASER_H_
#define __LASER_H_

#include <stdint.h>

void laser_init(void);
void laser_enable(bool on_off_n);
bool laser_is_enabled(void);
#endif // __LASER_H_