#include "zf_common_headfile.h"

#define USR_KEY  gpio_get_level(B3)

extern uint8 key;  //按下为0监测点接通GND 为0  抬起为监测点不接GND 为1
extern uint8 key_status;  //按键状态实时获取

extern void Key_Init(void);
extern uint8_t Key_Scan(void);
