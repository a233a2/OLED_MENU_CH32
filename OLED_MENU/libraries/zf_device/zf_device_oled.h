/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_oled
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�             ��Ƭ���ܽ�
*                   D0                  �鿴 zf_device_oled.h �� OLED_D0_PIN �궨��
*                   D1                  �鿴 zf_device_oled.h �� OLED_D1_PIN �궨��
*                   RES                 �鿴 zf_device_oled.h �� OLED_RES_PIN �궨��
*                   DC                  �鿴 zf_device_oled.h �� OLED_DC_PIN �궨��
*                   CS                  �鿴 zf_device_oled.h �� OLED_CS_PIN �궨��
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_oled_h_
#define _zf_device_oled_h_

#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"

#include "zf_driver_delay.h"
//#include "zf_driver_soft_spi.h"
#include "zf_driver_spi.h"

#define OLED_USE_SOFT_SPI               0                                       // Ĭ��ʹ��Ӳ�� SPI ��ʽ���� ����ʹ��Ӳ�� SPI ��ʽ����
#if OLED_USE_SOFT_SPI                                                           // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================��� SPI ����====================================================
#define OLED_SOFT_SPI_DELAY             1                                       // ��� SPI ��ʱ����ʱ���� ��ֵԽС SPI ͨ������Խ��
#define OLED_D0_PIN                     D4                                      // ��� SPI SCK ����
#define OLED_D1_PIN                     D6                                      // ��� SPI MOSI ����
//====================================================��� SPI ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define OLED_SPI_SPEED                  system_clock/2                          // Ӳ�� SPI ����
#define OLED_SPI                        SPI_2                                   // Ӳ�� SPI ��
#define OLED_D0_PIN                     SPI2_SCK_B13                             // Ӳ�� SPI SCK ����
#define OLED_D1_PIN                     SPI2_MOSI_B15                            // Ӳ�� SPI MOSI ����
//====================================================Ӳ�� SPI ����====================================================
#endif

#define OLED_RES_PIN                    B7                                     // Һ����λ���Ŷ���
#define OLED_DC_PIN                     D7                                      // Һ������λ���Ŷ���
#define OLED_CS_PIN                     D4                                      // CS Ƭѡ����

#define OLED_RES(x)                     (x?(gpio_high(OLED_RES_PIN)):(gpio_low(OLED_RES_PIN)))
#define OLED_DC(x)                      (x?(gpio_high(OLED_DC_PIN)):(gpio_low(OLED_DC_PIN)))
#define OLED_CS(x)                      (x?(gpio_high(OLED_CS_PIN)):(gpio_low(OLED_CS_PIN)))

typedef enum
{
    OLED_CROSSWISE                      = 0,                                    // ����ģʽ
    OLED_CROSSWISE_180                  = 1,                                    // ����ģʽ  ��ת180
}oled_dir_enum;

typedef enum
{
    OLED_6x8_FONT                      = 0,                                     // 6x8 ����
    OLED_8x16_FONT                      = 1,                                    // 8x16 ����
}oled_font_size_enum;

#define OLED_BRIGHTNESS                 0x7f                                    // ����OLED���� Խ��Խ�� ��Χ0-0XFF
#define OLED_DEFAULT_DISPLAY_DIR        OLED_CROSSWISE                          // Ĭ�ϵ���ʾ����
#define OLED_DEFAULT_DISPLAY_FONT       OLED_6x8_FONT                           // Ĭ�ϵ���ʾ����
#define OLED_X_MAX                      128
#define OLED_Y_MAX                      64

void    oled_clear                      (void);
void    oled_full                       (const uint8 color);
void    oled_set_dir                    (oled_dir_enum dir);
void    oled_set_font                   (oled_font_size_enum font);
void    oled_draw_point                 (uint16 x, uint16 y, const uint8 color);

void    oled_show_string                (uint16 x, uint16 y, const char ch[]);
void    oled_show_int                   (uint16 x, uint16 y, const int32 dat, uint8 num);
void    oled_show_uint                  (uint16 x, uint16 y, const uint32 dat, uint8 num);
void    oled_show_float                 (uint16 x, uint16 y, const float dat, uint8 num,uint8 pointnum);

void    oled_show_binary_image          (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void    oled_show_gray_image            (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold);

void    oled_show_wave                  (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void    oled_show_chinese               (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number);

#define oled_displayimage7725(p,width,height)                                   oled_show_binary_image(0,0,p,width,height,128,64)

#define oled_displayimage032(p,width,height,x)                                  oled_show_gray_image(0,0,p,width,height,128,64,x)
#define oled_displayimage032_zoom(p,width,height,dis_width,dis_height,x)        oled_show_gray_image(0,0,p,width,height,dis_width,dis_height,x)

void    oled_init                       (void);

#endif
