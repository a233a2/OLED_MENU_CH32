#include "zf_common_headfile.h"

#define USR_KEY  gpio_get_level(B3)

extern uint8 key;  //����Ϊ0�����ͨGND Ϊ0  ̧��Ϊ���㲻��GND Ϊ1
extern uint8 key_status;  //����״̬ʵʱ��ȡ

extern void Key_Init(void);
extern uint8_t Key_Scan(void);
