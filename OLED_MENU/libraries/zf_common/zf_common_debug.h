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

#ifndef _zf_common_debug_h_
#define _zf_common_debug_h_

#include "zf_common_typedef.h"
#define PRINTF_ENABLE               (1)                                         // ʹ��printf
// ����޸Ĵ��ڲ������� debug UART ���жϽ��� ��Ҫͬ������ debug_interrupr_handler ��������Ӧ���жϷ�����
#define DEBUG_UART_INDEX            (UART_3)                                    // ָ�� debug uart ��ʹ�õĵĴ���
#define DEBUG_UART_BAUDRATE         (115200)                                    // ָ�� debug uart ��ʹ�õĵĴ��ڲ�����
#define DEBUG_UART_TX_PIN           (UART3_TX_B10 )                             // ָ�� debug uart ��ʹ�õĵĴ�������
#define DEBUG_UART_RX_PIN           (UART3_RX_B11)                              // ָ�� debug uart ��ʹ�õĵĴ�������

typedef struct
{
    uint16 type_index;

    uint16 display_x_max;
    uint16 display_y_max;

    uint8 font_x_size;
    uint8 font_y_size;

    void (*output_uart)             (const char *str);
    void (*output_screen)           (uint16 x, uint16 y, const char *str);
    void (*output_screen_clear)     (void);
}debug_output_struct;

#define zf_assert(x)                (debug_assert_handler((x), __FILE__, __LINE__))     // ���� һ�����ڲ����ж� zf_assert(0) �Ͷ��Ա���
#define zf_log(x, str)              (debug_log_handler((x), (str), __FILE__, __LINE__)) // ������Ϣ��� ������һЩ������߾���֮������

#define DEBUG_UART_USE_INTERRUPT    (0)                                         // �Ƿ����� debug uart �����ж�
#if DEBUG_UART_USE_INTERRUPT                                                    // ������� debug uart �����ж�
#define DEBUG_RING_BUFFER_LEN       (64)                                        // ���廷�λ�������С Ĭ�� 64byte

extern uint8  debug_ring_buffer[DEBUG_RING_BUFFER_LEN];                         // ���λ�����
extern uint32 debug_ring_index;                                                 // �������±�

void        debug_interrupr_handler (void);
uint32      debug_read_ring_buffer  (uint8 *data);
#endif

void        debug_assert_enable         (void);
void        debug_assert_disable        (void);
void        debug_assert_handler        (uint8 pass, char *file, int line);
void        debug_log_handler           (uint8 pass, char *str, char *file, int line);
void        debug_output_struct_init    (debug_output_struct *info);
void        debug_output_init           (debug_output_struct *info);
void        debug_init                  (void);

#endif
