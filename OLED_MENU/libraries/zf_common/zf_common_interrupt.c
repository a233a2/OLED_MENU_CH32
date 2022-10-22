/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_interrupt
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V1.1 2022.03.16 �޸Ŀ������жϺ������ƣ������ж�Ƕ�׼���
********************************************************************************************************************/


#include "zf_common_interrupt.h"

static uint32 interrupt_nest_count = 0;


//-------------------------------------------------------------------------------------------------------------------
// @brief       ָ���ж�ʹ��
// @param       irqn            ָ���жϺ� �ɲ鿴 isr.c ��Ӧ�жϷ������ı�ע
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_enable (IRQn_Type irqn)
{
    NVIC_EnableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ָ���ж�����
// @param       irqn            ָ���жϺ� �ɲ鿴 isr.c ��Ӧ�жϷ������ı�ע
// @return      void
// Sample usage:                interrupt_disable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_disable (IRQn_Type irqn)
{
    NVIC_DisableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ָ���ж��������ȼ�
// @param       irqn            ָ���жϺ� �ɲ鿴 isr.c ��Ӧ�жϷ������ı�ע
// @param       priority        bit7-bit5Ϊ��ռ���ȼ���bit4-bit0Ϊ�����ȼ���ֵԽ�����ȼ�Խ��
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn, (1<<5) || 2);
//                              ��ռ���ȼ�����Ϊ1,�����ȼ�����Ϊ2
//-------------------------------------------------------------------------------------------------------------------
void interrupt_set_priority (IRQn_Type irqn, uint8 priority)
{
    NVIC_SetPriority(irqn, priority);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ж����ʼ�� clock_init �ڲ�����
// @param       void
// @return      void
// Sample usage:                interrupt_init();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_init (void)
{
    //NVIC_SetPriorityGrouping(4);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ȫ���ж�ʹ��
// @param       void            ��ԭ��Ƕ�ײ� ������� 0 ��ֱ�����Ƕ�ײ����������ж�
// @return      void
// Sample usage:                interrupt_global_enable();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_global_enable (void)
{
    if(interrupt_nest_count)
        interrupt_nest_count--;

    if(interrupt_nest_count == 0)
    {
        extern void en_interrupt();
        en_interrupt();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ȫ���ж�����
// @param       void
// @return      void
// Sample usage:                interrupt_disable_all();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_global_disable (void)
{
    if(!interrupt_nest_count)
    {
        extern void dis_interrupt();
        dis_interrupt();
    }
    interrupt_nest_count++;
    //���ص�ǰmstatus�Ĵ�����ֵ
//    uint32 temp;
//    temp = __get_MSTATUS();
//    return temp;
}
