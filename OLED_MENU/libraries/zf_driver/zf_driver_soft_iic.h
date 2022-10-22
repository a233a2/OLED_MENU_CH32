/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_driver_soft_iic
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef _zf_driver_soft_iic_h_
#define _zf_driver_soft_iic_h_

#include "zf_common_debug.h"
#include "zf_driver_gpio.h"

typedef struct
{
    gpio_pin_enum       scl_pin;                                                // ���ڼ�¼��Ӧ�����ű��
    gpio_pin_enum       sda_pin;                                                // ���ڼ�¼��Ӧ�����ű��
    uint8             addr;                                                   // ������ַ
    uint32            delay;                                                  // ģ�� IIC ����ʱʱ��
}soft_iic_info_struct;

void        soft_iic_init                   (soft_iic_info_struct *soft_iic_obj, uint8 addr, uint32 delay, gpio_pin_enum scl_pin, gpio_pin_enum sda_pin);

void        soft_iic_write_8bit             (soft_iic_info_struct *soft_iic_obj, uint8 data);
void        soft_iic_write_8bit_arrray      (soft_iic_info_struct *soft_iic_obj, uint8 *data, uint32 len);

void        soft_iic_write_16bit            (soft_iic_info_struct *soft_iic_obj, uint16 data);
void        soft_iic_write_16bit_arrray     (soft_iic_info_struct *soft_iic_obj, uint16 *data, uint32 len);

void        soft_iic_write_8bit_register    (soft_iic_info_struct *soft_iic_obj, uint8 register_name, uint8 data);
void        soft_iic_write_8bit_registers   (soft_iic_info_struct *soft_iic_obj, uint8 register_name, uint8 *data, uint32 len);

void        soft_iic_write_16bit_register   (soft_iic_info_struct *soft_iic_obj, uint16 register_name, uint16 data);
void        soft_iic_write_16bit_registers  (soft_iic_info_struct *soft_iic_obj, uint16 register_name, uint16 *data, uint32 len);

uint8     soft_iic_read_8bit              (soft_iic_info_struct *soft_iic_obj);
void        soft_iic_read_8bit_array        (soft_iic_info_struct *soft_iic_obj, uint8 *data, uint32 len);

uint16    soft_iic_read_16bit             (soft_iic_info_struct *soft_iic_obj);
void        soft_iic_read_16bit_array       (soft_iic_info_struct *soft_iic_obj, uint16 *data, uint32 len);

uint8     soft_iic_read_8bit_register     (soft_iic_info_struct *soft_iic_obj, uint8 register_name);
void        soft_iic_read_8bit_registers    (soft_iic_info_struct *soft_iic_obj, uint8 register_name, uint8 *data, uint32 len);

uint16    soft_iic_read_16bit_register    (soft_iic_info_struct *soft_iic_obj, uint16 register_name);
void        soft_iic_read_16bit_registers   (soft_iic_info_struct *soft_iic_obj, uint16 register_name, uint16 *data, uint32 len);

void        soft_iic_sccb_write_register    (soft_iic_info_struct *soft_iic_obj, uint8 register_name, uint8 data);
uint8     soft_iic_sccb_read_register     (soft_iic_info_struct *soft_iic_obj, uint8 register_name);

#endif

