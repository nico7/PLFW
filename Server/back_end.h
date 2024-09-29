#ifndef __BACK_END_H_
#define __BACK_END_H_

typedef enum wifi_status 
{
  BE_DISCONNECTED,
  BE_CONNECTED,
} WifiStatus_E;

void back_end_sm(void);




#endif // __BACK_END_H_