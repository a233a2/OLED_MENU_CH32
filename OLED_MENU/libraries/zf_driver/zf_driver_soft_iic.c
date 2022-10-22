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


#include "zf_driver_soft_iic.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC ��ʱ �ڲ�����
// @param       void
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_delay (uint32 delay)
{
    volatile uint32 count = delay;
    while(count--);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڳ�ʼ�� Ĭ�� MASTER ģʽ ���ṩ SLAVE ģʽ
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ��Žṹ���ָ��
// @param       addr            ��� IIC ��ַ ������Ҫע�� ��׼��λ��ַ ���λ���� д��ʱ�����ȷ������
// @param       delay           ��� IIC ��ʱ ����ʱ�Ӹߵ�ƽʱ�� Խ�� IIC ����Խ��
// @param       scl_pin         ��� IIC ʱ������ ���� zf_driver_gpio.h �� gpio_pin_enum ö���嶨��
// @param       sda_pin         ��� IIC �������� ���� zf_driver_gpio.h �� gpio_pin_enum ö���嶨��
// @return      void            
// Sample usage:                soft_iic_init(&soft_iic_obj, addr, 100, B6, B7);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_init (soft_iic_info_struct *soft_iic_obj, uint8 addr, uint32 delay, gpio_pin_enum scl_pin, gpio_pin_enum sda_pin)
{
    soft_iic_obj->scl_pin = scl_pin;
    soft_iic_obj->sda_pin = sda_pin;
    soft_iic_obj->addr = addr;
    soft_iic_obj->delay = delay;
    gpio_init(scl_pin, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // ��ȡ��ӦIO���� AF���ܱ���
    gpio_init(sda_pin, GPO, GPIO_HIGH, GPO_OPEN_DTAIN);                         // ��ȡ��ӦIO���� AF���ܱ���
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC START �ź� �ڲ�����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_start (soft_iic_info_struct *soft_iic_obj)
{
    gpio_high(soft_iic_obj->scl_pin);                                           // SCL �ߵ�ƽ
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA �ߵ�ƽ

    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->sda_pin);                                            // SDA ������
    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL ������
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC STOP �ź� �ڲ�����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_stop (soft_iic_info_struct *soft_iic_obj)
{
    gpio_low(soft_iic_obj->sda_pin);                                            // SDA �͵�ƽ
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ

    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->scl_pin);                                           // SCL ������
    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA ������
    soft_iic_delay(soft_iic_obj->delay);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC ���� ACK/NAKC �ź� �ڲ�����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_send_ack (soft_iic_info_struct *soft_iic_obj, uint8 ack)
{
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ

    soft_iic_delay(soft_iic_obj->delay);
    if(ack)
        gpio_high(soft_iic_obj->sda_pin);                                       // SDA ����
    else
        gpio_low(soft_iic_obj->sda_pin);                                        // SDA ����

    gpio_high(soft_iic_obj->scl_pin);                                           // SCL ����
    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL ����
    soft_iic_delay(soft_iic_obj->delay);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC ��ȡ ACK/NAKC �ź� �ڲ�����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @return      uint8         ACK ״̬
//-------------------------------------------------------------------------------------------------------------------
static uint8 soft_iic_wait_ack (soft_iic_info_struct *soft_iic_obj)
{
    uint8 temp = 0;
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ
    soft_iic_delay(soft_iic_obj->delay);

    gpio_high(soft_iic_obj->scl_pin);                                           // SCL �ߵ�ƽ
    soft_iic_delay(soft_iic_obj->delay);

    if(gpio_get_level(soft_iic_obj->sda_pin))
        temp = 1;
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ
    soft_iic_delay(soft_iic_obj->delay);

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC ���� 8bit ���� �ڲ�����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       data            ����
// @return      uint8         ACK ״̬
//-------------------------------------------------------------------------------------------------------------------
static uint8 soft_iic_send_data (soft_iic_info_struct *soft_iic_obj, uint8 data)
{
    uint8 temp = 8;

    while(temp--)
    {
        if(data & 0x80)
            gpio_high(soft_iic_obj->sda_pin);                                   // SDA �ߵ�ƽ
        else
            gpio_low(soft_iic_obj->sda_pin);                                    // SDA �͵�ƽ
        data = data << 1;

        soft_iic_delay(soft_iic_obj->delay);
        gpio_high(soft_iic_obj->scl_pin);                                       // SCL ����
        soft_iic_delay(soft_iic_obj->delay);
        gpio_low(soft_iic_obj->scl_pin);                                        // SCL ����
    }
    if(soft_iic_wait_ack(soft_iic_obj) == 1)
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC ��ȡ 8bit ���� �ڲ�����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       ack             ACK �� NACK
// @return      uint8         ����
//-------------------------------------------------------------------------------------------------------------------
static uint8 soft_iic_read_data (soft_iic_info_struct *soft_iic_obj, uint8 ack)
{
    uint8 data = 0x00;
    uint8 temp = 8;
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ
    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA �ߵ�ƽ �ͷ� SDA

    while(temp--)
    {
        soft_iic_delay(soft_iic_obj->delay);
        gpio_low(soft_iic_obj->scl_pin);                                        // SCL ����
        soft_iic_delay(soft_iic_obj->delay);
        gpio_high(soft_iic_obj->scl_pin);                                       // SCL ����
        soft_iic_delay(soft_iic_obj->delay);
        data = data << 1;
        if(gpio_get_level(soft_iic_obj->sda_pin))
            data |= 1;
    }
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL �͵�ƽ
    soft_iic_delay(soft_iic_obj->delay);
    soft_iic_send_ack(soft_iic_obj, ack);
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ�д 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       data            Ҫд�������
// @return      void            
// Sample usage:                soft_iic_write_8bit_register(soft_iic_obj,0x01);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit (soft_iic_info_struct *soft_iic_obj, uint8 data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ�д 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       *data           ���ݴ�Ż�����
// @param       len             ����������
// @return      void            
// Sample usage:                soft_iic_write_8bit_arrray(soft_iic_obj,data,6);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_arrray (soft_iic_info_struct *soft_iic_obj, uint8 *data, uint32 len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    while(len--)
    {
        soft_iic_send_data(soft_iic_obj, *data++);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ���д 16bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       data            Ҫд�������
// @return      void            
// Sample usage:                soft_iic_write_16bit(soft_iic_obj,0x0101);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit (soft_iic_info_struct *soft_iic_obj, uint16 data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (uint8)((data & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8)(data & 0x00FF));
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ�д 16bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       *data           ���ݴ�Ż�����
// @param       len             ����������
// @return      void            
// Sample usage:                soft_iic_write_16bit_array(soft_iic_obj,data,6);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_array (soft_iic_info_struct *soft_iic_obj, uint16 *data, uint32 len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    while(len--)
    {
        soft_iic_send_data(soft_iic_obj, (uint8)((*data & 0xFF00)>>8));
        soft_iic_send_data(soft_iic_obj, (uint8)(*data++ & 0x00FF));
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ��򴫸����Ĵ���д 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @param       data            Ҫд�������
// @return      void            
// Sample usage:                soft_iic_write_8bit_register(soft_iic_obj,0x01,0x01);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_register (soft_iic_info_struct *soft_iic_obj, uint8 register_name, uint8 data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ��򴫸����Ĵ���д 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @param       *data           ���ݴ�Ż�����
// @param       len             ����������
// @return      void            
// Sample usage:                soft_iic_write_8bit_registers(soft_iic_obj,0x01,data,6);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_registers (soft_iic_info_struct *soft_iic_obj, uint8 register_name, uint8 *data, uint32 len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    while(len--)
    {
        soft_iic_send_data(soft_iic_obj, *data++);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ��򴫸����Ĵ���д 16bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @param       data            Ҫд�������
// @return      void            
// Sample usage:                soft_iic_write_16bit_register(soft_iic_obj,0x0101,0x0101);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_register (soft_iic_info_struct *soft_iic_obj, uint16 register_name, uint16 data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8)((register_name & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8)(register_name & 0x00FF));
    soft_iic_send_data(soft_iic_obj, (uint8)((data & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8)(data & 0x00FF));
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ��򴫸����Ĵ���д 16bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @param       *data           ���ݴ�Ż�����
// @param       len             ����������
// @return      void            
// Sample usage:                soft_iic_write_16bit_registers(soft_iic_obj,0x0101,data,6);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_registers (soft_iic_info_struct *soft_iic_obj, uint16 register_name, uint16 *data, uint32 len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8)((register_name & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8)(register_name & 0x00FF));
    while(len--)
    {
        soft_iic_send_data(soft_iic_obj, (uint8)((*data & 0xFF00)>>8));
        soft_iic_send_data(soft_iic_obj, (uint8)(*data++ & 0x00FF));
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڶ�ȡ 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @return      uint8         ���ض�ȡ�� 8bit ����
// Sample usage:                soft_iic_read_8bit(soft_iic_obj);
//-------------------------------------------------------------------------------------------------------------------
uint8 soft_iic_read_8bit (soft_iic_info_struct *soft_iic_obj)
{
    uint8 temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @param       *data           Ҫ��ȡ�����ݵĻ�����ָ��
// @param       len             Ҫ��ȡ�����ݳ���
// @return      void            
// Sample usage:                soft_iic_read_8bit_array(soft_iic_obj,data,8);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_8bit_array (soft_iic_info_struct *soft_iic_obj, uint8 *data, uint32 len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while(len--)
    {
        *data++ = soft_iic_read_data(soft_iic_obj, len==0);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڶ�ȡ 16bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @return      uint16        ���ض�ȡ�� 16bit ����
// Sample usage:                soft_iic_read_16bit(soft_iic_obj);
//-------------------------------------------------------------------------------------------------------------------
uint16 soft_iic_read_16bit (soft_iic_info_struct *soft_iic_obj)
{
    uint16 temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp = ((temp << 8)| soft_iic_read_data(soft_iic_obj, 1));
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڶ�ȡ 16bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       *data           Ҫ��ȡ�����ݵĻ�����ָ��
// @param       len             Ҫ��ȡ�����ݳ���
// @return      void            
// Sample usage:                soft_iic_read_16bit_array(soft_iic_obj,data,8);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_16bit_array (soft_iic_info_struct *soft_iic_obj, uint16 *data, uint32 len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while(len--)
    {
        *data = soft_iic_read_data(soft_iic_obj, 0);
        *data = ((*data << 8)| soft_iic_read_data(soft_iic_obj, len==0));
        data++;
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @return      uint8         ���ض�ȡ�� 8bit ����
// Sample usage:                soft_iic_read_8bit_register(soft_iic_obj,0x01);
//-------------------------------------------------------------------------------------------------------------------
uint8 soft_iic_read_8bit_register (soft_iic_info_struct *soft_iic_obj, uint8 register_name)
{
    uint8 temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @param       *data           Ҫ��ȡ�����ݵĻ�����ָ��
// @param       len             Ҫ��ȡ�����ݳ���
// @return      void            
// Sample usage:                soft_iic_read_8bit_registers(soft_iic_obj,0x01,data,8);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_8bit_registers (soft_iic_info_struct *soft_iic_obj, uint8 register_name, uint8 *data, uint32 len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while(len--)
    {
        *data++ = soft_iic_read_data(soft_iic_obj, len==0);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 16bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @return      uint16        ���ض�ȡ�� 16bit ����
// Sample usage:                soft_iic_read_16bit_register(soft_iic_obj,0x0101);
//-------------------------------------------------------------------------------------------------------------------
uint16 soft_iic_read_16bit_register (soft_iic_info_struct *soft_iic_obj, uint16 register_name)
{
    uint16 temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8)((register_name & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8)(register_name & 0x00FF));
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp = ((temp << 8)| soft_iic_read_data(soft_iic_obj, 1));
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 16bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @param       *data           Ҫ��ȡ�����ݵĻ�����ָ��
// @param       len             Ҫ��ȡ�����ݳ���
// @return      void            
// Sample usage:                soft_iic_read_16bit_registers(soft_iic_obj,0x0101,data,8);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_16bit_registers (soft_iic_info_struct *soft_iic_obj, uint16 register_name, uint16 *data, uint32 len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8)((register_name & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8)(register_name & 0x00FF));
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while(len--)
    {
        *data = soft_iic_read_data(soft_iic_obj, 0);
        *data = ((*data << 8)| soft_iic_read_data(soft_iic_obj, len==0));
        data++;
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ� SCCB ģʽ�򴫸����Ĵ���д 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @param       data            Ҫд�������
// @return      void            
// Sample usage:                soft_iic_sccb_write_register(soft_iic_obj,0x01,0x01);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_sccb_write_register (soft_iic_info_struct *soft_iic_obj, uint8 register_name, uint8 data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��� IIC �ӿ� SCCB ģʽ�Ӵ������Ĵ�����ȡ 8bit ����
// @param       *soft_iic_obj   ��� IIC ָ����Ϣ ���Բ��� zf_driver_soft_iic.h ��ĸ�ʽ����
// @param       register_name   �������ļĴ�����ַ
// @return      uint8         ���ض�ȡ�� 8bit ����
// Sample usage:                soft_iic_sccb_read_register(soft_iic_obj,0x01);
//-------------------------------------------------------------------------------------------------------------------
uint8 soft_iic_sccb_read_register (soft_iic_info_struct *soft_iic_obj, uint8 register_name)
{
    uint8 temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_stop(soft_iic_obj);

    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

