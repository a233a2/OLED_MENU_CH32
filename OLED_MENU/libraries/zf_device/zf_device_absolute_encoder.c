/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_absolute_encoder
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @date             2022-04-11
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // Ӳ�� SPI ����
*                   SCLK                �鿴 zf_device_absolute_encoder.h �� ABSOLUTE_ENCODER_SCLK_PIN �궨��
*                   MOSI                �鿴 zf_device_absolute_encoder.h �� ABSOLUTE_ENCODER_MOSI_PIN �궨��
*                   MISO                �鿴 zf_device_absolute_encoder.h �� ABSOLUTE_ENCODER_MISO_PIN �궨��
*                   CS                  �鿴 zf_device_absolute_encoder.h �� ABSOLUTE_ENCODER_CS_PIN �궨��
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_device_absolute_encoder.h"

static int16 now_location = 0;
static int16 last_location = 0;

#if ABSOLUTE_ENCODER_USE_SOFT_SPI
static soft_spi_info_struct                 absolute_encoder_spi;
#define absolute_encoder_read()             (soft_spi_read_8bit(&absolute_encoder_spi))
#define absolute_encoder_write(data)        (soft_spi_write_8bit(&absolute_encoder_spi, (data)))
#else
#define absolute_encoder_read()             (spi_read_8bit(ABSOLUTE_ENCODER_SPI))
#define absolute_encoder_write(data)        (spi_write_8bit(ABSOLUTE_ENCODER_SPI, (data)))
#endif

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ������д�Ĵ��� �ڲ�����
// @param       reg             �Ĵ�����ַ
// @param       data            ����
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void absolute_encoder_write_register(uint8 reg, uint8 data)
{
    ABSOLUTE_ENCODER_CSN(0);                                                    // Ƭѡ����ѡ��
    absolute_encoder_write(reg | ABS_ENCODER_SPI_W);                            // �Ĵ���
    absolute_encoder_write(data);                                               // ����
    ABSOLUTE_ENCODER_CSN(1);                                                    // Ƭѡ�����ͷ�
    system_delay_us(1);                                                         // ��Ҫ����
    ABSOLUTE_ENCODER_CSN(0);                                                    // Ƭѡ����ѡ��
    absolute_encoder_read();                                                    // ����᷵��д���Ƿ�ɹ� �������ж�
    absolute_encoder_read();                                                    // ��Ҫ����
    ABSOLUTE_ENCODER_CSN(1);                                                    // Ƭѡ�����ͷ�
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ���������Ĵ��� �ڲ�����
// @param       reg             �Ĵ�����ַ
// @return      uint8           ����
//-------------------------------------------------------------------------------------------------------------------
static uint8 absolute_encoder_read_register(uint8 reg)
{
    uint8 data = 0;
    ABSOLUTE_ENCODER_CSN(0);                                                    // Ƭѡ����ѡ��
    absolute_encoder_write(reg | ABS_ENCODER_SPI_R);                            // �Ĵ���
    absolute_encoder_write(0x00);                                               // ռλ
    ABSOLUTE_ENCODER_CSN(1);                                                    // Ƭѡ�����ͷ�
    system_delay_us(1);                                                         // ��Ҫ����
    ABSOLUTE_ENCODER_CSN(0);                                                    // Ƭѡ����ѡ��
    data = absolute_encoder_read();                                             // ��ȡ��ȡ������
    absolute_encoder_read();                                                    // ��Ҫ����
    ABSOLUTE_ENCODER_CSN(1);                                                    // Ƭѡ�����ͷ�
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ��������λ�� �ڲ�����
// @param       void
// @return      uint16          λ��ֵ
//-------------------------------------------------------------------------------------------------------------------
static uint16 absolute_encoder_read_data (void)
{
    uint16 data = 0;
    ABSOLUTE_ENCODER_CSN(0);                                                    // Ƭѡ����ѡ��
    data = absolute_encoder_read();                                             // ��ȡ�߰�λ����
    data = (data & 0x00FF) << 8;                                                // ����λ��
    data |= absolute_encoder_read();                                            // ��ȡ�Ͱ�λ����
    ABSOLUTE_ENCODER_CSN(1);                                                    // Ƭѡ�����ͷ�
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ�������Լ� �ڲ�����
// @param       void
// @return      uint8           �Լ�״̬
//-------------------------------------------------------------------------------------------------------------------
static uint8 absolute_encoder_self_check (void)
{
    uint8 i = 0, return_state = 0;
    uint8 dat[6] = {0, 0, 0, 0xC0, 0xFF, 0x1C};
    uint16 time_count = 0;
    while(0x1C != absolute_encoder_read_register(6))                            // ��ȡ״̬�Ĵ���
    {
        for(i = 0; i < 6; i ++)
        {
            absolute_encoder_write_register(i + 1, dat[i]);                     // д��Ĭ�����ò���
            system_delay_ms(1);
        }
        if(time_count ++ > ABSOLUTE_ENCODER_TIMEOUT_COUNT)                      // �ȴ���ʱ
        {
            return_state = 1;
            break;
        }
    }
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ��������ȡ��ǰ�Ƕ�ֵ
// @param       void
// @return      int16           �Ƕ�ֵ
// Sample usage:                absolute_encoder_get_location();
//-------------------------------------------------------------------------------------------------------------------
int16 absolute_encoder_get_location (void)
{
    last_location = now_location;
    now_location = absolute_encoder_read_data() >> 4;
    return now_location;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ��������ȡ����ϴ�λ�õ�ƫ��ֵ
// @param       void
// @return      int16           ƫ��ֵ
// Sample usage:                absolute_encoder_get_offset();
//-------------------------------------------------------------------------------------------------------------------
int16 absolute_encoder_get_offset (void)
{
    if(myabs(now_location-last_location) > 2048)
        return (now_location > 2048 ? (now_location - 4096 - last_location) : (now_location + 4096 - last_location));
    else
        return (now_location - last_location);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ��������ʼ��
// @param       void
// @return      uint8           ��ʼ��״̬ 0-�ɹ� 1-ʧ��
// Sample usage:                absolute_encoder_init();
//-------------------------------------------------------------------------------------------------------------------
uint8 absolute_encoder_init (void)
{
    uint16 zero_position = ABSOLUTE_ENCODER_DEFAULT_ZERO;
#if ABSOLUTE_ENCODER_USE_SOFT_SPI
    soft_spi_init(&absolute_encoder_spi, 0, ABSOLUTE_ENCODER_SOFT_SPI_DELAY, ABSOLUTE_ENCODER_SCLK_PIN, ABSOLUTE_ENCODER_MOSI_PIN, ABSOLUTE_ENCODER_MISO_PIN, SOFT_SPI_PIN_NULL);
#else
    // �������������˶�����Ϣ ������ʾ����λ��������
    // ��ȥ������޸ĵ����ŵĺ궨���ǲ��Ǹ���� SPI �궨�岻һ��
    zf_assert(ABSOLUTE_ENCODER_SPI != (ABSOLUTE_ENCODER_SCLK_PIN & 0xF000 >> 12));
    zf_assert(ABSOLUTE_ENCODER_SPI != (ABSOLUTE_ENCODER_MOSI_PIN & 0xF000 >> 12));
    spi_init(ABSOLUTE_ENCODER_SPI, SPI_MODE0, ABSOLUTE_ENCODER_SPI_SPEED, ABSOLUTE_ENCODER_SCLK_PIN, ABSOLUTE_ENCODER_MOSI_PIN, ABSOLUTE_ENCODER_MISO_PIN, SPI_CS_NULL);
#endif
    gpio_init(ABSOLUTE_ENCODER_CS_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);

    if(absolute_encoder_self_check())
    {
        // �������������˶�����Ϣ ������ʾ����λ��������
        // ��ô���Ǿ���ֵ�������Լ������ʱ�˳���
        // ���һ�½�����û������ ���û������ܾ��ǻ���
        zf_assert(0);
    }
    absolute_encoder_write_register(DIR_REG, 0x00);                             // ������ת���� ��ת��ֵ��С��0x00   ��ת��ֵ���0x80
    zero_position = (uint16)(4096 - zero_position);
    zero_position = zero_position << 4;
    absolute_encoder_write_register(ZERO_L_REG, (uint8)zero_position);          // ������λ
    absolute_encoder_write_register(ZERO_H_REG, zero_position >> 8);
    return 0;
}
