/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_wireless_uart
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V2.0 ��д���ߴ���������
********************************************************************************************************************/

#include "zf_device_wireless_uart.h"
#include "zf_common_fifo.h"
#include "zf_device_type.h"

static  fifo_struct         wireless_uart_fifo;
static  uint8               wireless_uart_buffer[WIRELESS_UART_BUFFER_SIZE];    // ���ݴ������

static          uint8       wireless_uart_data;
#if WIRELESS_UART_AUTO_BAUD_RATE
static volatile uint32      wireless_auto_baud_flag = 0;
static volatile uint8       wireless_auto_baud_data[3] = {0x00, 0x01, 0x03};
#endif
//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת�����жϻص�����
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_callback (void)
{
    uart_query_byte(WIRELESS_UART_INDEX, &wireless_uart_data);
    fifo_write_buffer(&wireless_uart_fifo, &wireless_uart_data, 1);
#if WIRELESS_UART_AUTO_BAUD_RATE                                                // �����Զ�������
    if(wireless_auto_baud_flag == 1 && fifo_used(&wireless_uart_fifo) == 3)
    {
        wireless_auto_baud_flag = 3;
        fifo_read_buffer(&wireless_uart_fifo, (uint8 *)wireless_auto_baud_data, (uint32 *)&wireless_auto_baud_flag, FIFO_READ_AND_CLEAN);
    }
#endif
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ�� ��������
// @param       data            8bit ����
// @return      uint32          ʣ�෢�ͳ���
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_send_byte (uint8 data)
{
    uint16 time_count = 0;
    while(gpio_get_level(WIRELESS_UART_RTS_PIN))                                      // ���RTSΪ�͵�ƽ���������������
    {
        if(time_count++ > WIRELESS_UART_TIMEOUT_COUNT)
            return 1;                                                           // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        system_delay_ms(1);
    }
    uart_write_byte(WIRELESS_UART_INDEX, data);                                 // ������������

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ�� ���ڽ�������
// @param       buff            ���ݵ�ַ
// @param       len             ���ݳ���
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_read_buff (uint8 *buff, uint32 len)
{
    uint32 data_len = len;
    fifo_read_buffer(&wireless_uart_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}





//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ�� ���ڷ�������
// @param       buff            ���ݵ�ַ
// @param       len             ���ݳ���
// @return      uint32_t        ��ʾ��Ϣ
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_send_buff ( uint8 *buff, uint32 len)
{
    uint16 time_count = 0;
    while(len > 30)
    {
        time_count = 0;
        while(gpio_get_level(WIRELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                  // ���RTSΪ�͵�ƽ���������������
            system_delay_ms(1);
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
            return len;                                                                         // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        uart_write_buffer(WIRELESS_UART_INDEX, buff, 30);

        buff += 30;                                                                             // ��ַƫ��
        len -= 30;                                                                              // ����
    }

    time_count = 0;
    while(gpio_get_level(WIRELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                      // ���RTSΪ�͵�ƽ���������������
        system_delay_ms(1);
    if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        return len;                                                                             // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
    uart_write_buffer(WIRELESS_UART_INDEX, buff, len);                                          // ������������

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ�� �����ַ���
// @param       *str            Ҫ���͵��ַ�����ַ
// @return      uint32          ʣ�෢�ͳ���
// Sample usage:                wireless_uart_send_string("Believe in yourself.");
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_send_string ( uint8 *str)
{
    uint16 time_count = 0;
    uint32 len = strlen((const char *)str);
    while(len > 30)
    {
        time_count = 0;
        while(gpio_get_level(WIRELESS_UART_RTS_PIN))                                  // ���RTSΪ�͵�ƽ���������������
        {
            if(time_count++ > WIRELESS_UART_TIMEOUT_COUNT)
                return len;                                                     // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
            system_delay_ms(1);
        }
        uart_write_buffer(WIRELESS_UART_INDEX, str, 30);

        str += 30;                                                              // ��ַƫ��
        len -= 30;                                                              // ����
    }

    time_count = 0;
    while(gpio_get_level(WIRELESS_UART_RTS_PIN))                                      // ���RTSΪ�͵�ƽ���������������
    {
        if(time_count++ > WIRELESS_UART_TIMEOUT_COUNT)
            return len;                                                         // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        system_delay_ms(1);
    }
    uart_write_buffer(WIRELESS_UART_INDEX, str, len);                           // ������������

    return 0;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ�� ��������ͷͼ������λ���鿴ͼ��
// @param       *image_addr     ��Ҫ���͵�ͼ���ַ
// @param       image_size      ͼ��Ĵ�С
// @return      void
// Sample usage:                wireless_uart_send_image(&mt9v03x_image[0][0], MT9V03X_IMAGE_SIZE);
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_send_image ( uint8 *image_addr, uint32 image_size)
{
    extern uint8 camera_send_image_frame_header[4];
    wireless_uart_send_buff(camera_send_image_frame_header, 4);
    wireless_uart_send_buff((uint8 *)image_addr, image_size);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ���ʼ��
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_init (void)
{
    set_wireless_type(WIRELESS_UART);

    fifo_init(&wireless_uart_fifo, wireless_uart_buffer, WIRELESS_UART_BUFFER_SIZE);
    gpio_init(WIRELESS_UART_RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
#if(0 == WIRELESS_UART_AUTO_BAUD_RATE)                                          // �ر��Զ�������
    // ������ʹ�õĲ�����Ϊ115200 Ϊ����ת����ģ���Ĭ�ϲ����� ������������������������ģ�鲢�޸Ĵ��ڵĲ�����
    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // ��ʼ������
    uart_rx_interrupt(WIRELESS_UART_INDEX, 1);
#elif(1 == WIRELESS_UART_AUTO_BAUD_RATE)                                        // �����Զ�������
    uint8 rts_init_status;
    uint16 time_count = 0;

    wireless_auto_baud_flag = 0;
    wireless_auto_baud_data[0] = 0;
    wireless_auto_baud_data[1] = 1;
    wireless_auto_baud_data[2] = 3;

    rts_init_status = gpio_get(WIRELESS_UART_RTS_PIN);
    gpio_init(WIRELESS_UART_RTS_PIN, GPO, rts_init_status, GPO_PUSH_PULL);      // ��ʼ����������

    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // ��ʼ������
    uart_rx_irq(WIRELESS_UART_INDEX, 1);

    system_delay_ms(5);                                                         // ģ���ϵ�֮����Ҫ��ʱ�ȴ�
    gpio_set(WIRELESS_UART_RTS_PIN, !rts_init_status);                          // RTS�������ߣ������Զ�������ģʽ
    system_delay_ms(100);                                                       // RTS����֮�������ʱ20ms
    gpio_toggle(WIRELESS_UART_RTS_PIN);                                         // RTS����ȡ��

    wireless_auto_baud_flag = 1;

    uart_putchar(WIRELESS_UART_INDEX, wireless_auto_baud_data[0]);              // �����ض����� ����ģ���Զ��жϲ�����
    uart_putchar(WIRELESS_UART_INDEX, wireless_auto_baud_data[1]);              // �����ض����� ����ģ���Զ��жϲ�����
    uart_putchar(WIRELESS_UART_INDEX, wireless_auto_baud_data[2]);              // �����ض����� ����ģ���Զ��жϲ�����
    system_delay_ms(20);

    time_count = 0;
    if(3 != wireless_auto_baud_flag)                                            // �����Զ��������Ƿ����
    {
        while(time_count++)
            system_delay_ms(1);
    }
    if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        return 1;                                                               // ���������뵽������� ˵���Զ�������ʧ����

    time_count = 0;
    if( 0xa5 != wireless_auto_baud_data[0] &&                                   // �����Զ��������Ƿ���ȷ
        0xff != wireless_auto_baud_data[1] &&                                   // �����Զ��������Ƿ���ȷ
        0xff != wireless_auto_baud_data[2] )                                    // �����Զ��������Ƿ���ȷ
    {
        while(time_count++)
            system_delay_ms(1);
    }
    if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        return 1;                                                               // ���������뵽������� ˵���Զ�������ʧ����
    wireless_auto_baud_flag = 0;

    gpio_init(WIRELESS_UART_RTS_PIN, GPI, 0, GPI_PULL_UP);                      // ��ʼ����������
    system_delay_ms(10);                                                        // ��ʱ�ȴ� ģ��׼������
#endif
    return 0;
}



