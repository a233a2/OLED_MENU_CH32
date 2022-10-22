/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_wireless_ch573
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2022-03-02
* @note
********************************************************************************************************************/
#ifndef _zf_device_wireless_ch573_h_
#define _zf_device_wireless_ch573_


#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"
#include "zf_driver_delay.h"



#define WIRELESS_CH573_NUMBER            1                                           // ��������CH573����ģ������� Ĭ����������
#define WIRELESS_CH573_INDEX             UART_2                                      // CH573����ģ��1 ��Ӧʹ�õĴ��ں�
#define WIRELESS_CH573_BUAD_RATE         115200                                      // CH573����ģ��1 ��Ӧʹ�õĴ��ڲ�����
#define WIRELESS_CH573_TX_PIN            UART2_RX_D6                                 // CH573����ģ��1 ��Ӧģ��� TX Ҫ�ӵ���Ƭ���� RX
#define WIRELESS_CH573_RX_PIN            UART2_TX_D5                                 // CH573����ģ��1 ��Ӧģ��� RX Ҫ�ӵ���Ƭ���� TX
#define WIRELESS_CH573_RTS_PIN           E10                                         // CH573����ģ��1 ��Ӧģ��� RTS ����


#define WIRELESS_CH573_BUFFER_SIZE       64
#define WIRELESS_CH573_TIMEOUT_COUNT     0x64



void        wireless_ch573_callback      (void);

uint32      wireless_ch573_send_byte         (const uint8 data);

uint32      wireless_ch573_send_buff         (const uint8 *buff, uint32 len);
uint32      wireless_ch573_send_string       (const uint8 *str);
void        wireless_ch573_send_image        (const uint8 *image_addr, uint32 image_size);

uint32      wireless_ch573_read_buff     (uint8 *buff, uint32 len);

uint8       wireless_ch573_init          (void);

#endif
