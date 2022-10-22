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

#include "zf_device_wireless_ch573.h"
#include "zf_common_fifo.h"
#include "zf_device_type.h"

static  fifo_struct       wireless_ch573_fifo;
static  uint8             wireless_ch573_buffer[WIRELESS_CH573_BUFFER_SIZE];                                // ��Ӧ��һ�� ���ߴ��� ���ݴ������

static  uint8             wireless_ch573_data;

//-------------------------------------------------------------------------------------------------------------------
// @brief       CH573����ģ���жϻص�����
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void wireless_ch573_callback (void)
{
    uart_query_byte(WIRELESS_CH573_INDEX, &wireless_ch573_data);
    fifo_write_buffer(&wireless_ch573_fifo, &wireless_ch573_data, 1);

}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ�� ��������
// @param       data            8bit ����
// @return      uint32          ʣ�෢�ͳ���
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_ch573_send_byte (const uint8 data)
{
    uint16 time_count = 0;
    while(gpio_get_level(WIRELESS_CH573_RTS_PIN))                                      // ���RTSΪ�͵�ƽ���������������
    {
        if(time_count++ > WIRELESS_CH573_TIMEOUT_COUNT)
            return 1;                                                           // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        system_delay_ms(1);
    }
    uart_write_byte(WIRELESS_CH573_INDEX, data);                                 // ������������

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       CH573����ģ�� ���ڷ�������
// @param       buff            ���ݵ�ַ
// @param       len             ���ݳ���
// @return      uint32        ��ʾ��Ϣ
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_ch573_send_buff (const uint8 *buff, uint32 len)
{
    uint16 time_count = 0;
    uint8 pack_len = 62;
    while(len > pack_len)
    {
        time_count = 0;
        while(gpio_get_level(WIRELESS_CH573_RTS_PIN) && time_count++ < WIRELESS_CH573_TIMEOUT_COUNT)  // ���RTSΪ�͵�ƽ���������������
            system_delay_ms(1);
        if(time_count >= WIRELESS_CH573_TIMEOUT_COUNT)
            return len;                                                                             // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        uart_write_buffer(WIRELESS_CH573_INDEX, buff, 30);

        buff += pack_len;                                                                           // ��ַƫ��
        len -= pack_len;                                                                            // ����
    }

    time_count = 0;
    while(gpio_get_level(WIRELESS_CH573_RTS_PIN) && time_count++ < WIRELESS_CH573_TIMEOUT_COUNT)  // ���RTSΪ�͵�ƽ���������������
        system_delay_ms(1);
    if(time_count >= WIRELESS_CH573_TIMEOUT_COUNT)
        return len;                                                                                 // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
    uart_write_buffer(WIRELESS_CH573_INDEX, buff, len);                                            // ������������

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       CH573����ģ�� ���ڽ�������
// @param       buff            ���ݵ�ַ
// @param       len             ���ݳ���
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_ch573_read_buff (uint8 *buff, uint32 len)
{
    uint32 data_len = len;
    fifo_read_buffer(&wireless_ch573_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ�� ��������ͷͼ������λ���鿴ͼ��
// @param       *image_addr     ��Ҫ���͵�ͼ���ַ
// @param       image_size      ͼ��Ĵ�С
// @return      void
// Sample usage:                wireless_uart_send_image(&mt9v03x_image[0][0], MT9V03X_IMAGE_SIZE);
//-------------------------------------------------------------------------------------------------------------------
void wireless_ch573_send_image (const uint8 *image_addr, uint32 image_size)
{
    extern uint8 camera_send_image_frame_header[4];
    wireless_ch573_send_buff(camera_send_image_frame_header, 4);
    wireless_ch573_send_buff((uint8 *)image_addr, image_size);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       CH573����ģ���ʼ��
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_ch573_init (void)
{

    set_wireless_type(WIRELESS_CH573);

    fifo_init(&wireless_ch573_fifo, wireless_ch573_buffer, WIRELESS_CH573_BUFFER_SIZE);
    gpio_init(WIRELESS_CH573_RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);

    uart_init (WIRELESS_CH573_INDEX, WIRELESS_CH573_BUAD_RATE, WIRELESS_CH573_RX_PIN, WIRELESS_CH573_TX_PIN);
    uart_rx_interrupt(WIRELESS_CH573_INDEX, 1);

    return 0;
}



