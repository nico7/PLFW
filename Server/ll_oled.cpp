#include "ll_oled.h"

#include <Arduino.h>
#include <SPI.h>
#include <string.h>

#include "gpios.h"
#include "symbols.h"


static void ll_oled_init_symbols(void);

const SPISettings spiSettings(4000000, MSBFIRST, SPI_MODE0);

static uint8_t myScreen[64][128] = { 0 };
static uint8_t rastScreen[NUM_PAGES][128] = {0};

static uint8_t scrx, scry;    // These keep track of where symbols are written on the screen

void ll_oled_start(void) {
  SPI.begin(PL_SCK, PL_MISO, PL_MOSI);
  digitalWrite(OLED_CS_N, LOW);
  SPI.beginTransaction(spiSettings);
}

void ll_oled_end(void) {
  digitalWrite(OLED_CS_N, HIGH);
  SPI.endTransaction();
  SPI.end();
}

void ll_oled_init(void) {
  scrx = 0;
  scry = 0;
  
  digitalWrite(OLED_RST_N, LOW);
  delay(10);
  digitalWrite(OLED_RST_N, HIGH);
  delay(1);
  ll_oled_init_symbols();
  // All "Recommendations" were made by Chatgpt lol
  ll_oled_start();
  /*ll_oled_send_cmd(SET_CLK_DIV_RATIO_FMODE);
 */

  ll_oled_send_cmd(SET_MULTIPLEX_ROT_MODE);
  ll_oled_send_cmd(RECOMMENDED_MULTIPLEX);

  ll_oled_send_cmd(SET_DC_DC_MODE);
  ll_oled_send_cmd(SET_DC_DC_OFF);
  ll_oled_send_cmd(SET_DISP_OFFSET_MODE);
  
  ll_oled_send_cmd(RECOMMENDED_OFFSET);
  ll_oled_send_cmd(SET_DISP_START_LN);
  ll_oled_send_cmd(SET_CONTRAST_CTRL_MODE);
  ll_oled_send_cmd(MOST_CONTRAST);
  
  ll_oled_send_cmd(SET_PRE_CHRG_MODE);
  ll_oled_send_cmd(RECOMMENDED_PRE_CHG);  // Longer pre-charge for brighter pixels
  ll_oled_send_cmd(SET_COM_PADS_CFG_MODE);
  ll_oled_send_cmd(SET_COM_PADS_CFG_DATA);
  
  ll_oled_send_cmd(SET_VCOM_DESEL_LVL_MODE);
  ll_oled_send_cmd(SET_VCOM_DESEL_LVL_DATA);
  ll_oled_send_cmd(SET_NRML_DISP);
  ll_oled_send_cmd(DISPLAY_ON);

  ll_oled_clear(CLEAR_OLED);
  char str[100] = "0123456789   ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnop\n\rqrstuvwxyz.[µ";
  ll_oled_print((uint8_t *)str, sizeof(str));

  ll_oled_display_screen();
  
  ll_oled_end();
}


void ll_oled_send_cmd(uint8_t cmd) {
  digitalWrite(DATA_CMD_N, LOW);
  SPI.transfer(cmd);
}

void ll_oled_send_data(uint8_t* data, uint16_t size) {
  uint16_t i;

  digitalWrite(DATA_CMD_N, HIGH);

  for (i = 0; i < size; i++) {
    SPI.transfer(data[i]);
  }
}

void rasterize(void)
{
  uint8_t page, col, row;


  for(page = 0; page < NUM_PAGES; page++)
  {
    row = page * PAGE_HEIGHT;
    for(col = 0; col < SCREEN_WIDTH; col++)
    {
      rastScreen[page][col] = ((myScreen[row][col] & 0x01) ) | ((myScreen[row + 1][col] & 0x01) << 1) | ((myScreen[row + 2][col] & 0x01) << 2) | ((myScreen[row + 3][col] & 0x01) << 3)
                              | ((myScreen[row + 4][col] & 0x01) << 4) | ((myScreen[row + 5][col] & 0x01) << 5) | ((myScreen[row + 6][col] & 0x01) << 6) | ((myScreen[row + 7][col] & 0x01) << 7);
    }
  }
}

void ll_oled_display_screen(void)
{
  uint8_t col, page;
  uint32_t ran;

  rasterize();


  for(page = 0; page < NUM_PAGES; page++)
  {
    ll_oled_send_cmd(SET_PAGE_ADDR | page);
    ll_oled_send_cmd(SET_LOW_COL_ADDR + 2 );
    ll_oled_send_cmd(SET_HIGH_COL_ADDR);

    ll_oled_send_data(rastScreen[page], SCREEN_WIDTH);
  }
}

void ll_oled_clear(uint8_t reset_locations)
{
    if(reset_locations)
    {
      scrx = 0;
      scry = 0;
    }

    memset(myScreen, 0, sizeof(myScreen));
}

static void ll_draw_char( const uint8_t * charArray, uint8_t charWidth)
{
  uint8_t height, width;

  if(scrx + A_10_WIDTH > SCREEN_WIDTH)
  {
    scrx = 0;

    if(scry + LINE_HEIGHT > SCREEN_HEIGHT)
    {
      scry = 0;
    }
    else
    {
      scry += LINE_HEIGHT;
    }
  }
  for(height = 0; height < SYMBOL_HEIGHT_10; height++)
  {
    for(width = 0; width < charWidth; width++)
    {
      myScreen[height + scry][ width + scrx] = (charArray[height] >> (7 - width));
    }
  }
 
  scrx += charWidth + 2;
 
}

void ll_oled_print(uint8_t * str, uint16_t size)
{
  uint16_t i;
  uint8_t height, width;
  

  for(i = 0; i < size; i++)
  {
  
    height = 0;
    width = 0;
    switch(str[i])
    {
      case ' ':
        scrx += 3;
        break;
      case '\n':
        if(scry >= SCREEN_HEIGHT - LINE_HEIGHT)
        {
          scry = 0;
        }
        else
        {
          scry += LINE_HEIGHT;
        }
        break;
      case '\r':
        scrx = 0;
        break;
      default:

        ll_draw_char(letterMap[(uint8_t (str[i]))], widthMap[str[i]]);
        break;
    }
  }


}

static void ll_oled_init_symbols(void)
{
  letterMap[0x41] = A_10;
  letterMap[0x42] = B_10;
  letterMap[0x43] = C_10;
  letterMap[0x44] = D_10;
  letterMap[0x45] = E_10;
  letterMap[0x46] = F_10;
  letterMap[0x47] = G_10;
  letterMap[0x48] = H_10;
  letterMap[0x49] = I_10;
  letterMap[0x4A] = J_10;
  letterMap[0x4B] = K_10;
  letterMap[0x4C] = L_10;
  letterMap[0x4D] = M_10;
  letterMap[0x4E] = N_10;
  letterMap[0x4F] = O_10;
  letterMap[0x50] = P_10;
  letterMap[0x51] = Q_10;
  letterMap[0x52] = R_10;
  letterMap[0x53] = S_10;
  letterMap[0x54] = T_10;
  letterMap[0x55] = U_10;
  letterMap[0x56] = V_10;
  letterMap[0x57] = W_10;
  letterMap[0x58] = X_10;
  letterMap[0x59] = Y_10;
  letterMap[0x5A] = Z_10;
  letterMap[0x61] = a_10;
  letterMap[0x62] = b_10;
  letterMap[0x63] = c_10;
  letterMap[0x64] = d_10;
  letterMap[0x65] = e_10;
  letterMap[0x66] = f_10;
  letterMap[0x67] = g_10;
  letterMap[0x68] = h_10;
  letterMap[0x69] = i_10;
  letterMap[0x6A] = j_10;
  letterMap[0x6B] = k_10;
  letterMap[0x6C] = l_10;
  letterMap[0x6D] = m_10;
  letterMap[0x6E] = n_10;
  letterMap[0x6F] = o_10;
  letterMap[0x70] = p_10;
  letterMap[0x71] = q_10;
  letterMap[0x72] = r_10;
  letterMap[0x73] = s_10;
  letterMap[0x74] = t_10;
  letterMap[0x75] = u_10;
  letterMap[0x76] = v_10;
  letterMap[0x77] = w_10;
  letterMap[0x78] = x_10;
  letterMap[0x79] = y_10;
  letterMap[0x7A] = z_10;
  letterMap[0xB5] = µ_10;
  letterMap[0x5B] = Ω_10;
  letterMap[0x30] = num_0_10;
  letterMap[0x31] = num_1_10;
  letterMap[0x32] = num_2_10;
  letterMap[0x33] = num_3_10;
  letterMap[0x34] = num_4_10;
  letterMap[0x35] = num_5_10;
  letterMap[0x36] = num_6_10;
  letterMap[0x37] = num_7_10;
  letterMap[0x38] = num_8_10;
  letterMap[0x39] = num_9_10;
  letterMap[0x2E] = period_10;
  
  widthMap[0x41] = A_10_WIDTH;
  widthMap[0x42] = B_10_WIDTH;
  widthMap[0x43] = C_10_WIDTH;
  widthMap[0x44] = D_10_WIDTH;
  widthMap[0x45] = E_10_WIDTH;
  widthMap[0x46] = F_10_WIDTH;
  widthMap[0x47] = G_10_WIDTH;
  widthMap[0x48] = H_10_WIDTH;
  widthMap[0x49] = I_10_WIDTH;
  widthMap[0x4A] = J_10_WIDTH;
  widthMap[0x4B] = K_10_WIDTH;
  widthMap[0x4C] = L_10_WIDTH;
  widthMap[0x4D] = M_10_WIDTH;
  widthMap[0x4E] = N_10_WIDTH;
  widthMap[0x4F] = O_10_WIDTH;
  widthMap[0x50] = P_10_WIDTH;
  widthMap[0x51] = Q_10_WIDTH;
  widthMap[0x52] = R_10_WIDTH;
  widthMap[0x53] = S_10_WIDTH;
  widthMap[0x54] = T_10_WIDTH;
  widthMap[0x55] = U_10_WIDTH;
  widthMap[0x56] = V_10_WIDTH;
  widthMap[0x57] = W_10_WIDTH;
  widthMap[0x58] = X_10_WIDTH;
  widthMap[0x59] = Y_10_WIDTH;
  widthMap[0x5A] = Z_10_WIDTH;
  widthMap[0x61] = a_10_WIDTH;
  widthMap[0x62] = b_10_WIDTH;
  widthMap[0x63] = c_10_WIDTH;
  widthMap[0x64] = d_10_WIDTH;
  widthMap[0x65] = e_10_WIDTH;
  widthMap[0x66] = f_10_WIDTH;
  widthMap[0x67] = g_10_WIDTH;
  widthMap[0x68] = h_10_WIDTH;
  widthMap[0x69] = i_10_WIDTH;
  widthMap[0x6A] = j_10_WIDTH;
  widthMap[0x6B] = k_10_WIDTH;
  widthMap[0x6C] = l_10_WIDTH;
  widthMap[0x6D] = m_10_WIDTH;
  widthMap[0x6E] = n_10_WIDTH;
  widthMap[0x6F] = o_10_WIDTH;
  widthMap[0x70] = p_10_WIDTH;
  widthMap[0x71] = q_10_WIDTH;
  widthMap[0x72] = r_10_WIDTH;
  widthMap[0x73] = s_10_WIDTH;
  widthMap[0x74] = t_10_WIDTH;
  widthMap[0x75] = u_10_WIDTH;
  widthMap[0x76] = v_10_WIDTH;
  widthMap[0x77] = w_10_WIDTH;
  widthMap[0x78] = x_10_WIDTH;
  widthMap[0x79] = y_10_WIDTH;
  widthMap[0x7A] = z_10_WIDTH;
  widthMap[0xB5] = µ_10_WIDTH;
  widthMap[0x5B] = Ω_10_WIDTH;
  widthMap[0x30] = NUM_0_WIDTH;
  widthMap[0x31] = NUM_1_WIDTH;
  widthMap[0x32] = NUM_2_WIDTH;
  widthMap[0x33] = NUM_3_WIDTH;
  widthMap[0x34] = NUM_4_WIDTH;
  widthMap[0x35] = NUM_5_WIDTH;
  widthMap[0x36] = NUM_6_WIDTH;
  widthMap[0x37] = NUM_7_WIDTH;
  widthMap[0x38] = NUM_8_WIDTH;
  widthMap[0x39] = NUM_9_WIDTH;
  widthMap[0x2E] = PERIOD_WIDTH;
}