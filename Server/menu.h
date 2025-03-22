#ifndef __MENU_H_
#define __MENU_H_

#include <stdint.h>

#define NONE_PRESSED  0
#define LEFT_PRESSED  1
#define RIGHT_PRESSED 2
#define UP_PRESSED    3
#define DOWN_PRESSED  4

void menu_sm(uint8_t button_pressed);

#endif //__MENU_H_