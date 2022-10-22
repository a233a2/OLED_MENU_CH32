/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_dvp
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#ifndef _zf_dvp_h
#define _zf_dvp_h

#include "ch32v30x.h"
#include "stdio.h"

#include "ch32v30x_rcc.h"
#include "ch32v30x_misc.h"
#include "ch32v30x_gpio.h"

#include "zf_common_interrupt.h"
#include "zf_common_typedef.h"


void dvp_camera_init(uint32 *image0, uint32 *image1, uint16 col_len, uint16 row_len);
void dvp_gpio_init(
        uint8 dvp_d0,uint8 dvp_d1,uint8 dvp_d2,uint8 dvp_d3,
        uint8 dvp_d4,uint8 dvp_d5,uint8 dvp_d6,uint8 dvp_d7,
        uint8 dvp_pclk,uint8 dvp_vsync,uint8 dvp_hsync);
#endif
