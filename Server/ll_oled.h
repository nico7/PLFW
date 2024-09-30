#ifndef __LL_OLED_H_
#define __LL_OLED_H_

#include <stdint.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define NUM_PAGES   8
#define PAGE_HEIGHT 8

#define SET_LOW_COL_ADDR        0x00    // to 0x0F
#define SET_HIGH_COL_ADDR       0x10    // to 0x1F
#define SET_PUMP_VOLT_VAL       0x30    // to 0x33
#define SET_DISP_START_LN       0x40    // to 0x7F
#define SET_CONTRAST_CTRL_MODE  0x81   
#define MOST_CONTRAST           0xFF
//#define SET_CONTRAST_CTRL_DATA  0x00    // to 0xFF   
#define SET_SEG_REMAP           0xA0    // to 0xA1
#define SET_DISPLAY_OFF         0xA4
#define SET_DISPLAY_ON          0xA5
#define SET_NRML_DISP           0xA6
#define SET_REVRS_DISP          0xA7
#define SET_MULTIPLEX_ROT_MODE  0xA8 
#define RECOMMENDED_MULTIPLEX   0x3F
//#define SET_MULTIPLEX_ROT_DATA  0x00    // to 0x3F 
#define SET_DC_DC_MODE          0xAD
#define SET_DC_DC_OFF           0x8A
#define SET_DC_DC_ON            0x8B
#define DISPLAY_OFF             0xAE
#define DISPLAY_ON              0xAF
#define SET_PAGE_ADDR           0xB0    // to 0xB7
#define SET_SCAN_DIR            0xC0    // to 0xC8
#define SET_DISP_OFFSET_MODE    0xD3
#define RECOMMENDED_OFFSET      0x00
//#define SET_DISP_OFFSET_DATA     0x00   // to 0x3F
#define SET_CLK_DIV_RATIO_FMODE 0xD5
#define RECOMMENDED_DIV_RATIO   0x80
//#define SET_CLK_DIV_RATIO_FDATA 0x00      // to 0xFF
#define SET_PRE_CHRG_MODE       0xD9
#define RECOMMENDED_PRE_CHG     0xF2
#define SET_PRE_CHRG_DATA       0x00    // to 0xFF
#define SET_COM_PADS_CFG_MODE   0xDA
#define SET_COM_PADS_CFG_DATA   0x12    // to 0x12
#define SET_VCOM_DESEL_LVL_MODE 0xDB
#define SET_VCOM_DESEL_LVL_DATA 0xFF    // to 0xFF
#define READ_MOD_WRITE          0xE0
#define END_READ_MOD_WRITE      0xEE
#define NOP                     0xE3


void ll_oled_init(void);
void ll_oled_start(void);
void ll_oled_end(void);
void ll_oled_send_cmd( uint8_t cmd);
void ll_oled_send_data(uint8_t * data, uint16_t size);
void rasterize(void);
void ll_oled_display_screen(void);
void ll_oled_clear(void);
void ll_oled_print(uint8_t * string, uint16_t size);


#endif //__LL_OLED_H_