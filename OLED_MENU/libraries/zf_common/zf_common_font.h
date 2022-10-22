
/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_font
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef _zf_common_font_h_
#define _zf_common_font_h_


#include "zf_common_typedef.h"

//-------������ɫ----------
#define RGB565_WHITE           0xFFFF  // ��ɫ
#define RGB565_BLACK           0x0000  // ��ɫ
#define RGB565_BLUE            0x001F  // ��ɫ
#define RGB565_PURPLE          0XF81F  // ��ɫ
#define RGB565_PINK            0XFE19  // ��ɫ
#define RGB565_RED             0xF800  // ��ɫ
#define RGB565_MAGENTA         0xF81F  // Ʒ��
#define RGB565_GREEN           0x07E0  // ��ɫ
#define RGB565_CYAN            0x07FF  // ��ɫ
#define RGB565_YELLOW          0xFFE0  // ��ɫ
#define RGB565_BROWN           0XBC40  // ��ɫ
#define RGB565_GRAY            0X8430  // ��ɫ

extern const uint8      tft_ascii[95][16];
extern const uint8      oled_6x8[][6];
extern const uint8      oled_8x16[];
extern const uint8      chinese_test[8][16];
extern const uint8      oled_16x16_chinese[][16];
extern const uint8      gImage_seekfree_logo[38400];

extern unsigned char     Hzk1[22][16];
extern unsigned char     Hzk2[3][72];
extern unsigned char     Hzk3[8][128];
extern unsigned char     Hzk4[8][512];


extern const unsigned char asc2_1206[95][12];

extern const unsigned char asc2_1608[95][16];

extern const unsigned char asc2_2412[95][36];


extern unsigned char paimeng[];


#endif
