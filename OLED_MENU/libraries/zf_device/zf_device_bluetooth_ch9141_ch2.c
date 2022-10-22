/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_bluetooth_ch9141
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V1.1 2022.01.10 �Ի�CH1��CH2�����ź�
*                   V1.2 2022.03.16 ɾ����Բ���������ʹ�������λ���������
********************************************************************************************************************/

#include "zf_device_bluetooth_ch9141_ch2.h"
#include "zf_device_type.h"

static  fifo_struct         bluetooth_ch9141_fifo_ch2;
static  uint8             bluetooth_ch9141_buffer_ch2[BLUETOOTH_CH9141_BUFFER_SIZE_CH2];                                // ��Ӧ��һ�� ���ߴ��� ���ݴ������
static  uint8             bluetooth_ch9141_data_ch2;




//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� �����жϻص�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       �ú�����ISR�ļ� ����8�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_uart_callback_ch2 (void)
{
    uart_query_byte(BLUETOOTH_CH9141_INDEX_CH2, &bluetooth_ch9141_data_ch2);                // ��ȡ��������
    fifo_write_buffer(&bluetooth_ch9141_fifo_ch2, &bluetooth_ch9141_data_ch2, 1);           // ���� FIFO

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ʼ��
//  @param      mode    ����ģʽ MASTER_MODE(����)����SLAVE_MODE(�ӻ�)
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint8 bluetooth_ch9141_init_ch2 (void)
{

    set_wireless_type(BLUETOOTH_CH9141);


    fifo_init(&bluetooth_ch9141_fifo_ch2, bluetooth_ch9141_buffer_ch2, BLUETOOTH_CH9141_BUFFER_SIZE_CH2);
    uart_init(BLUETOOTH_CH9141_INDEX_CH2, BLUETOOTH_CH9141_BUAD_RATE_CH2, BLUETOOTH_CH9141_RX_PIN_CH2, BLUETOOTH_CH9141_TX_PIN_CH2);
    gpio_init(BLUETOOTH_CH9141_RTS_PIN_CH2, GPI, 1, GPI_PULL_UP);
    uart_rx_interrupt(BLUETOOTH_CH9141_INDEX_CH2, 1);

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ���ͺ���
//  @param      buff        ��Ҫ���͵����ݵ�ַ
//  @param      len         ���ͳ���
//  @return     uint32      ʣ��δ���͵��ֽ���
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint32 bluetooth_ch9141_send_buff_ch2 (uint8 *buff, uint32 len)
{
    uint16 time_count = 0;
    while(len > 30)
    {
        time_count = 0;
        while(gpio_get_level(BLUETOOTH_CH9141_RTS_PIN_CH2) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                  // ���RTSΪ�͵�ƽ���������������
            system_delay_ms(1);
        if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)
            return len;                                                                         // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        uart_write_buffer(BLUETOOTH_CH9141_INDEX_CH2, buff, 30);

        buff += 30;                                                                             // ��ַƫ��
        len -= 30;                                                                              // ����
    }

    time_count = 0;
    while(gpio_get_level(BLUETOOTH_CH9141_RTS_PIN_CH2) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                      // ���RTSΪ�͵�ƽ���������������
        system_delay_ms(1);
    if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)
        return len;                                                                             // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
    uart_write_buffer(BLUETOOTH_CH9141_INDEX_CH2, buff, len);                                              // ������������

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ��ȡ����
//  @param      buff        ��Ҫ��ȡ�����ݵ�ַ
//  @param      len         ���ͳ���
//  @return     uint32      ��ȡ�����ݳ��� ���û����ô����������ᱻ�޸�
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint32 bluetooth_ch9141_read_buff_ch2(uint8 *buff, uint32 len)
{
    uint32 data_len = len;
    fifo_read_buffer(&bluetooth_ch9141_fifo_ch2, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}





