#ifndef __OLED_H
#define __OLED_H

#include "system_ch32v30x.h"
#include "stdlib.h"
#include "zf_common_headfile.h"

//-----------------OLED�˿ڶ���----------------

#define OLED_SCLK_Clr() gpio_set_level(A5, 0);
#define OLED_SCLK_Set() gpio_set_level(A5, 1);

#define OLED_SDIN_Clr() gpio_set_level(A7, 0);
#define OLED_SDIN_Set() gpio_set_level(A7, 1);


//#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_11)//SCL
//#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_11)
//
//#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_10)//DIN
//#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define OLED_CMD  0 //д����
#define OLED_DATA 1 //д����

#define u8 unsigned char
#define u32 unsigned int

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);                                    //�����Դ浽OLED    PS:����ʾһ�ζ�����һ�������Դ�
void OLED_Clear(void);                                      //����
void OLED_DrawPoint(u8 x,u8 y);                             //����
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);                //����
void OLED_DrawCircle(u8 x,u8 y,u8 r);                       //��Բ
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);              //��ʾ�ַ���ASICII
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);           //��ʾ�ַ���
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);       //��ʾ2������
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);           //��ʾ����
void OLED_ScrollDisplay(u8 num,u8 space);
void OLED_WR_BP(u8 x,u8 y);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);    //��ʾͼƬ   ͼƬ�ֱ���128*64
void OLED_Init(void);                                       //OLED��ʼ��




#endif
