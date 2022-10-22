/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_absolute_encoder
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // Ӳ�� SPI ����
*                   SCLK                �鿴 zf_device_absolute_encoder.h �� ABSOLUTE_ENCODER_SCLK_PIN �궨��
*                   MOSI                �鿴 zf_device_absolute_encoder.h �� ABSOLUTE_ENCODER_MOSI_PIN �궨��
*                   MISO                �鿴 zf_device_absolute_encoder.h �� ABSOLUTE_ENCODER_MISO_PIN �궨��
*                   CS                  �鿴 zf_device_absolute_encoder.h �� ABSOLUTE_ENCODER_CS_PIN �궨��
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_absolute_encoder_h_
#define _zf_device_absolute_encoder_h_

#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_function.h"

#include "zf_driver_delay.h"
//#include "zf_driver_soft_spi.h"
#include "zf_driver_spi.h"

#define ABSOLUTE_ENCODER_USE_SOFT_SPI       (0)                                 // Ĭ��ʹ��Ӳ�� SPI ��ʽ����
#if ABSOLUTE_ENCODER_USE_SOFT_SPI                                               // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================��� SPI ����====================================================
#define ABSOLUTE_ENCODER_SOFT_SPI_DELAY     (1)                                 // ��� SPI ��ʱ����ʱ���� ��ֵԽС SPI ͨ������Խ��
#define ABSOLUTE_ENCODER_SCLK_PIN           (B13)                               // Ӳ�� SPI SCK ����
#define ABSOLUTE_ENCODER_MOSI_PIN           (B15)                               // Ӳ�� SPI MOSI ����
#define ABSOLUTE_ENCODER_MISO_PIN           (B14)                               // Ӳ�� SPI MISO ����
//====================================================��� SPI ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define ABSOLUTE_ENCODER_SPI_SPEED          (system_clock / 6)                  // Ӳ�� SPI ����
#define ABSOLUTE_ENCODER_SPI                (SPI_2)                             // Ӳ�� SPI ��
#define ABSOLUTE_ENCODER_SCLK_PIN           (SPI2_SCK_B13)                      // Ӳ�� SPI SCK ����
#define ABSOLUTE_ENCODER_MOSI_PIN           (SPI2_MOSI_B15)                     // Ӳ�� SPI MOSI ����
#define ABSOLUTE_ENCODER_MISO_PIN           (SPI2_MISO_B14)                     // Ӳ�� SPI MISO ����
//====================================================Ӳ�� SPI ����====================================================
#endif

#define ABSOLUTE_ENCODER_CS_PIN             (B12)
#define ABSOLUTE_ENCODER_CSN(x)             ((x) ? (gpio_high(ABSOLUTE_ENCODER_CS_PIN)): (gpio_low(ABSOLUTE_ENCODER_CS_PIN)))

#define ABSOLUTE_ENCODER_TIMEOUT_COUNT      (100)
#define ABSOLUTE_ENCODER_DEFAULT_ZERO       (0)

//====================================================�Ƕȴ���������====================================================
#define ABS_ENCODER_SPI_W                   (0x80)
#define ABS_ENCODER_SPI_R                   (0x40)

#define ZERO_L_REG                          (0x00)
#define ZERO_H_REG                          (0x01)
#define DIR_REG                             (0X09)
//====================================================�Ƕȴ���������====================================================

int16       absolute_encoder_get_location   (void);
int16       absolute_encoder_get_offset     (void);
uint8       absolute_encoder_init           (void);

#endif
