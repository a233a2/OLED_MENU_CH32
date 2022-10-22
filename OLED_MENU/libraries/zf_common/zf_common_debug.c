/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_debug
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#include "zf_driver_uart.h"

static debug_output_struct debug_output_info;
static volatile uint8 zf_debug_init_flag = 0;
static volatile uint8 zf_debug_assert_enable = 1;

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ����ʱ���� �� 120MHz ����һ����ʱ�� ����Ƭ����Ҫ���ݸ���ʱ������
// @param       pass        �ж��Ƿ񴥷�����
// @param       *file       �ļ���
// @param       line        Ŀ������
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void debug_delay (void)
{
    vuint32 loop_1 = 0, loop_2 = 0;
    for(loop_1 = 0; loop_1 <= 0xFF; loop_1 ++)
        for(loop_2 = 0; loop_2 <= 0xFFFF; loop_2 ++)
            __NOP();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ��������ӿ� �˲��ֲ������û�����
// @param       *str        ��Ҫ������ַ���
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void debug_uart_str_output (const char *str)
{
    uart_write_string(DEBUG_UART_INDEX, str);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ����ӿ� �˲��ֲ������û�����
// @param       *file       �ļ���
// @param       line        Ŀ������
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void debug_output (char *file, int line)
{
    char *file_str;
    vuint16 i = 0, j = 0;
    vint16 len_origin = 0;
    vint16 show_len = 0;
    len_origin = strlen(file);

    char output_buffer[256];
    char file_path_buffer[64];

    if(debug_output_info.type_index)
        debug_output_info.output_screen_clear();

    if(zf_debug_init_flag)
    {
        if(debug_output_info.type_index)
        {

            // ��Ҫ���н��ļ���·�����������
            // <���������·�� ֻ���һ��Ŀ¼ ���� src/main.c>
            // ��� line : xxxx
            debug_output_info.output_screen(0, 0, "Assert error");

            file_str = file;
            len_origin = strlen(file);
            show_len = (debug_output_info.display_x_max / debug_output_info.font_x_size);

            while(*file_str++ != '\0');

            // ֻȡһ��Ŀ¼ ����ļ������̷���Ŀ¼ ���� MDK �Ĺ��̸�Ŀ¼ �ͻ�ֱ�������ǰĿ¼
            for(j = 0; (j < 2) && (len_origin >= 0); len_origin --)          // �������� '/'
            {
                file_str --;
                if((*file_str == '/') || (*file_str == 0x5C))
                    j ++;
            }

            // �ļ�·�����浽������
            if(len_origin >= 0)
            {
                file_str ++;
                sprintf(output_buffer, "file: %s", file_str);
            }
            else
            {
                if(0 == j)
                    sprintf(output_buffer, "file: mdk/%s", file_str);
                else
                    sprintf(output_buffer, "file: %s", file_str);
            }

            // ��Ļ��ʾ·��
            for(i = 0; i < ((strlen(output_buffer) / show_len) + 1); i ++)
            {
                for(j = 0; j < show_len; j ++)
                {
                    if(strlen(output_buffer) < (j + i * show_len))
                        break;
                    file_path_buffer[j] = output_buffer[j + i * show_len];
                }

                file_path_buffer[j] = '\0';                                 // ĩβ���\0

                debug_output_info.output_screen(0, debug_output_info.font_y_size * (i + 1), file_path_buffer);
            }

            // ��Ļ��ʾ�к�
            sprintf(output_buffer, "line: %d", line);
            debug_output_info.output_screen(0, debug_output_info.font_y_size * (i + 1), output_buffer);
        }
        else
        {
            char output_buffer[128];
            memset(output_buffer, 0, 128);
            sprintf(output_buffer, "Assert error in %s line %d.\r\n", file, line);
            debug_output_info.output_uart(output_buffer);
        }
    }
}

#if DEBUG_UART_USE_INTERRUPT                                                    // �������� ֻ�������ô����жϲű���

uint8  debug_ring_buffer[DEBUG_RING_BUFFER_LEN];                                // �������λ�����
uint32 debug_ring_index = 0;                                                    // ���λ������±�����

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug �����жϴ����� isr.c �ж�Ӧ�����жϷ���������
// @param       void
// @return      void
// Sample usage:            debug_interrupr_handler();
//-------------------------------------------------------------------------------------------------------------------
void debug_interrupr_handler (void)
{
    if(!zf_debug_init_flag) return;
    uart_query_byte(DEBUG_UART_INDEX, &debug_ring_buffer[debug_ring_index ++]); // ��ȡ���ݵ����λ�����
    if(DEBUG_RING_BUFFER_LEN == debug_ring_index)   debug_ring_index = 0;       // ���λ�������β�ν�
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ debug ���λ���������
// @param       *data       �������ݴ�ŵ�����ָ��
// @return      uint32    �������ݳ���
// Sample usage:            uint8 data[64]; len = debug_read_ring_buffer(data);
//-------------------------------------------------------------------------------------------------------------------
uint32 debug_read_ring_buffer (uint8 *data)
{
    if(0 == debug_ring_index)   return 0;
    uint32 temp = debug_ring_index;                                             // ��ȡ���ݳ���
    memcpy(data, debug_ring_buffer, temp);                                      // ��������
    debug_ring_index = 0;                                                       // ����±�
    return temp;
}

#endif

//-------------------------------------------------------------------------     // printf �ض��� �˲��ֲ������û�����
//-------------------------------------------------------------------------------------------------------------------
//  @brief      printf�ض���
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:              �ض���printf��DEBUG������
//-------------------------------------------------------------------------------------------------------------------
#if (1 == PRINTF_ENABLE)
int _write(int fd, char *buf, int size)
{
    int i;
    for(i=0; i<size; i++)
    {
        while (USART_GetFlagStatus((USART_TypeDef*)uart_index[DEBUG_UART_INDEX], USART_FLAG_TC) == RESET);
        USART_SendData((USART_TypeDef*)uart_index[DEBUG_UART_INDEX], *buf++);
    }
    return size;
}
#endif
//-------------------------------------------------------------------------     // printf �ض��� �˲��ֲ������û�����

//-------------------------------------------------------------------------------------------------------------------
// @brief       ���ö���
// @param       void
// @return      void
// Sample usage:                debug_assert_enable();
//-------------------------------------------------------------------------------------------------------------------
void debug_assert_enable (void)
{
    zf_debug_assert_enable = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ���ö���
// @param       void
// @return      void
// Sample usage:                debug_assert_disable();
//-------------------------------------------------------------------------------------------------------------------
void debug_assert_disable (void)
{
    zf_debug_assert_enable = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ���Դ����� �˲��ֲ������û�����
// @param       pass        �ж��Ƿ񴥷�����
// @param       *file       �ļ���
// @param       line        Ŀ������
// @return      void
// Sample usage:            �����������ֱ�ӵ��õ� ʹ�� zf_commmon_debug.h �е� zf_assert(x) �ӿ�
//-------------------------------------------------------------------------------------------------------------------
void debug_assert_handler (uint8 pass, char *file, int line)
{
    if(pass || !zf_debug_assert_enable)
        return;

    static uint8 assert_nest_index = 0;

    if(0 != assert_nest_index)
    {
        while(1);
    }
    assert_nest_index ++;

    interrupt_global_disable();

    while(1)
    {
        // ���������ת������ͣס��
        // һ����ĺ����������ݳ�����
        // �������Լ����õ� zf_assert(x) �ӿڴ�������

        // ��������� debug_init ��ʼ���� log ���
        // ���ڶ�Ӧ�������ȥ�鿴���ĸ��ļ�����һ�б���

        // ���û�г�ʼ�� debug
        // �ǾͿ������ file ���ַ���ֵ�� line ������
        // �Ǵ�������ļ�·�����ƺͶ�Ӧ��������

        // ��ȥ���Կ�����Ϊʲô��������
//        __NOP();

        debug_output(file, line);
        debug_delay();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ������Ϣ������ �˲��ֲ������û�����
// @param       bool        �ж��Ƿ񴥷�����
// @param       *str        Ҫ����ĵ�����Ϣ
// @param       *file       �ļ���
// @param       line        Ŀ������
// @return      void
// Sample usage:            �����������ֱ�ӵ��õ� ʹ�� zf_commmon_debug.h �е� zf_log(x, str) �ӿ�
//-------------------------------------------------------------------------------------------------------------------
void debug_log_handler (uint8 pass, char *str, char *file, int line)
{
    if(pass)
        return;
    if(zf_debug_init_flag)
    {
        printf("Log message from %s line %d :\"%s\".\r\n", file, line, str);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug �������Ϣ��ʼ�� �˲��ֲ������û�����
// @param       *info       debug �������Ϣ�ṹ��
// @return      void
// Sample usage:            debug_output_struct_init(info);
//-------------------------------------------------------------------------------------------------------------------
void debug_output_struct_init (debug_output_struct *info)
{
    info->type_index            = 0;

    info->display_x_max         = 0xFFFF;
    info->display_y_max         = 0xFFFF;

    info->font_x_size           = 0xFF;
    info->font_y_size           = 0xFF;

    info->output_uart           = NULL;
    info->output_screen         = NULL;
    info->output_screen_clear   = NULL;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ����󶨳�ʼ�� �˲��ֲ������û�����
// @param       *info       debug �������Ϣ�ṹ��
// @return      void
// Sample usage:            debug_output_init(info);
//-------------------------------------------------------------------------------------------------------------------
void debug_output_init (debug_output_struct *info)
{
    debug_output_info.type_index            = info->type_index;

    debug_output_info.display_x_max         = info->display_x_max;
    debug_output_info.display_y_max         = info->display_y_max;

    debug_output_info.font_x_size           = info->font_x_size;
    debug_output_info.font_y_size           = info->font_y_size;

    debug_output_info.output_uart           = info->output_uart;
    debug_output_info.output_screen         = info->output_screen;
    debug_output_info.output_screen_clear   = info->output_screen_clear;

    zf_debug_init_flag = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ���ڳ�ʼ�� �˲��ֲ������û�����
// @param       void
// @return      void
// Sample usage:            debug_init();
//-------------------------------------------------------------------------------------------------------------------
void debug_init (void)
{
    debug_output_struct info;
    debug_output_struct_init(&info);
    info.output_uart = debug_uart_str_output;
    debug_output_init(&info);

    uart_init(
        DEBUG_UART_INDEX,                                                       // �� zf_common_debug.h �в鿴��Ӧֵ
        DEBUG_UART_BAUDRATE,                                                    // �� zf_common_debug.h �в鿴��Ӧֵ
        DEBUG_UART_TX_PIN,                                                      // �� zf_common_debug.h �в鿴��Ӧֵ
        DEBUG_UART_RX_PIN);                                                     // �� zf_common_debug.h �в鿴��Ӧֵ

    if(DEBUG_UART_USE_INTERRUPT)                                                // ������ô����ж�
    {
        uart_rx_interrupt(DEBUG_UART_INDEX, 1);                                 // ʹ�ܶ�Ӧ���ڽ����ж�
    }
}




