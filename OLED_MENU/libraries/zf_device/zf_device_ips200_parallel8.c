/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_ips200_parallel8
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�                                        ��Ƭ���ܽ�
*                   BL                  �鿴 zf_device_ips200_parallel8.h �� IPS200_BL_PIN �궨��
*                   CS                  �鿴 zf_device_ips200_parallel8.h �� IPS200_CS_PIN �궨��
*                   RST                 �鿴 zf_device_ips200_parallel8.h �� IPS200_RST_PIN �궨��
*                   RS                  �鿴 zf_device_ips200_parallel8.h �� IPS200_RS_PIN �궨��
*                   WR                  �鿴 zf_device_ips200_parallel8.h �� IPS200_WR_PIN �궨��
*                   RD                  �鿴 zf_device_ips200_parallel8.h �� IPS200_RD_PIN �궨��
*                   D0-D7               �鿴 zf_device_ips200_parallel8.h �� ICM20602_CS_PIN �궨��
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
* @note            version:
*                  V1.1 2021.12.02 �޸ĺ������� (ips200_showfloat -> ips200_show_float)
*                  V1.2 2022.01.24 �޸�����ģʽ��ʾ��ȫ������
********************************************************************************************************************/
#include "zf_device_ips200_parallel8.h"

uint16 ips200_pencolor = IPS200_DEFAULT_PENCOLOR;
uint16 ips200_bgcolor = IPS200_DEFAULT_BGCOLOR;

ips200_dir_enum ips200_display_dir = IPS200_DEFAULT_DISPLAY_DIR;
uint16 ips200_x_max = 240;
uint16 ips200_y_max = 320;

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 д���� �ڲ�����
// @param       dat             ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_data (uint8 dat)
{
    IPS200_DATAPORT->OUTDR = (dat<< DATA_START_NUM) | (IPS200_DATAPORT->OUTDR & ~((uint32)(0xFF << DATA_START_NUM)));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 д���� �ڲ�����
// @param       command         ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_command (uint8 command)
{
    IPS200_RS(0);
    IPS200_RD(1);
    IPS200_DATAPORT->OUTDR = (command<< DATA_START_NUM) | (IPS200_DATAPORT->OUTDR & ~((uint32)(0xFF << DATA_START_NUM)));
    IPS200_CS(0);
    IPS200_WR(0);
    IPS200_WR(1);
    IPS200_CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 ��Һ����д 8bit ���� �ڲ�����
// @param       dat             ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_8bit_data (uint8 dat)
{
    IPS200_RS(1);
    IPS200_RD(1);
    ips200_write_data(dat);
    IPS200_CS(0);
    IPS200_WR(0);
    IPS200_WR(1);
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 ��Һ����д 16bit ���� �ڲ�����
// @param       dat             ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_16bit_data (uint16 dat)
{
    IPS200_RS(1);
    IPS200_RD(1);
    ips200_write_data((uint8)(dat>>8));
    IPS200_CS(0);
    IPS200_WR(0);
    IPS200_WR(1);
    ips200_write_data((uint8)(dat));
    IPS200_CS(0);
    IPS200_WR(0);
    IPS200_WR(1);
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 д�Ĵ��� �ڲ�����
// @param       com             ����
// @param       dat             ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
//static void ips200_write_register (uint8 com, uint8 dat)
//{
//    ips200_write_command(com);
//    ips200_write_8bit_data(dat);
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �ڲ�����
// @param       x1              ��ʼx������
// @param       y1              ��ʼy������
// @param       x2              ����x������
// @param       y2              ����y������
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_set_region(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
    ips200_write_command(0x2a);
    ips200_write_16bit_data(x1);
    ips200_write_16bit_data(x2);

    ips200_write_command(0x2b);
    ips200_write_16bit_data(y1);
    ips200_write_16bit_data(y2);

    ips200_write_command(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �������ֻ���ڳ�ʼ����Ļ֮ǰ���ò���Ч
// @param       dir             ��ʾ����  ���� zf_device_ips200.h �� ips200_dir_enum ö���嶨��
// @return      void
// Sample usage:                ips200_set_dir(IPS114_CROSSWISE);
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_dir (ips200_dir_enum dir)
{
    ips200_display_dir = dir;
    if(dir < 2)
    {
        ips200_x_max = 240;
        ips200_y_max = 320;
    }
    else
    {
        ips200_x_max = 320;
        ips200_y_max = 240;
    }
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ��ɫ
// @param       pen             ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @param       bgcolor         ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                ips200_set_color(WHITE,BLACK);
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_color (uint16 pen, uint16 bgcolor)
{
    ips200_pencolor = pen;
    ips200_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ������
//  @param      x               ����x��������
//  @param      y               ����y��������
//  @param      dat             ��Ҫ��ʾ����ɫ
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_drawpoint(0,0,RED);  //����0,0��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips200_draw_point(uint16 x,uint16 y,uint16 color)
{
    ips200_set_region(x,y,x,y);
    ips200_write_16bit_data(color);
}



//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����������
// @param       color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                ips200_clear(RGB565YELLOW);
//-------------------------------------------------------------------------------------------------------------------
void ips200_clear (const uint16 color)
{
    uint16 i,j;
    ips200_set_region(0,0,ips200_x_max-1,ips200_y_max-1);
    for(i=0;i<ips200_x_max;i++)
    {
        for (j=0;j<ips200_y_max;j++)
        {
            ips200_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ������
// @param       x_start         ����x��������
// @param       y_start         ����y��������
// @param       x_end           ����x������յ�
// @param       y_end           ����y������յ�
// @param       dat             ��Ҫ��ʾ����ɫ
// @return      void
// Sample usage:                ips200_drawpoint(0,0,RBB565_RED);  //����0,0��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips200_draw_line (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color)
{
    zf_assert(x_start < ips200_x_max);
    zf_assert(y_start < ips200_y_max);
    zf_assert(x_end < ips200_x_max);
    zf_assert(y_end < ips200_y_max);

    int16 x_dir = (x_start<x_end ? 1:-1);
    int16 y_dir = (y_start<y_end ? 1:-1);
    float temp_rate = 0;
    float temp_b = 0;
    if(x_start != x_end)
    {
        temp_rate = (float)(y_start-y_end)/(float)(x_start-x_end);
        temp_b = (float)y_start-(float)x_start*temp_rate;
    }
    else
    {
        while(y_start != y_end)
        {
            ips200_draw_point(x_start, y_start, color);
            y_start += y_dir;
        }
        return;
    }
    if(myabs(y_start-y_end)>myabs(x_start-x_end))
    {
        while(y_start != y_end)
        {
            ips200_draw_point(x_start, y_start, color);
            y_start += y_dir;
            x_start = (int16)(((float)y_start-temp_b)/temp_rate);
        }
    }
    else
    {
        while(x_start != x_end)
        {
            ips200_draw_point(x_start, y_start, color);
            x_start += x_dir;
            y_start = (int16)((float)x_start*temp_rate+temp_b);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ�
// @param       x               ����x�������� ������Χ 0 - (ips200_x_max-1)
// @param       y               ����y�������� ������Χ 0 - (ips200_y_max/16-1)
// @param       dat             ��Ҫ��ʾ���ַ�
// @return      void
// Sample usage:                ips200_showchar(0,0,'x');//����0,0дһ���ַ�x
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_char (uint16 x, uint16 y, const char dat)
{
    uint8 i,j;
    uint8 temp;

    for(i=0; i<16; i++)
    {
        ips200_set_region(x,y+i,x+7,y+i);
        temp = tft_ascii[(uint16)dat-32][i];//��32��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո���ascii�������32
        for(j=0; j<8; j++)
        {
            if(temp&0x01)   ips200_write_16bit_data(ips200_pencolor);
            else            ips200_write_16bit_data(ips200_bgcolor);
            temp>>=1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ���
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ���ַ���
// @return      void
// Sample usage:                ips200_show_string(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_string (uint16 x, uint16 y, const char dat[])
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint16 j = 0;
    while(dat[j] != '\0')
    {
        ips200_show_char(x+8*j,y,dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint32
// @param       num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// @return      void
// Sample usage:                ips200_showuint32(0,0,x,3);                     // x ����Ϊ int32 uint16 int16 uint8 int8 ����
// note:                        ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_int (uint16 x, uint16 y, const int32 dat, uint8 num)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);
    zf_assert(num > 0);
    zf_assert(num <= 10);

    int32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    if(num < 10)
    {
        for(; num>0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    int_to_str(data_buffer, dat_temp);
    ips200_show_string(x,y,(const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint32
// @param       num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// @return      void
// Sample usage:                ips200_show_uint32(0,0,x,3);                    // x ����Ϊ int32 uint16 int16 uint8 int8 ����
// note:                        ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_uint (uint16 x, uint16 y, const uint32 dat, uint8 num)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);
    zf_assert(num > 0);
    zf_assert(num <= 10);

    uint32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    if(num < 10)
    {
        for(; num>0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    uint_to_str(data_buffer, dat_temp);
    ips200_show_string(x,y,(const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ������(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������float��double
// @param       num             ����λ��ʾ����   ���8λ
// @param       pointnum        С��λ��ʾ����   ���6λ
// @return      void
// Sample usage:                ips200_show_float(0,0,x,2,3);                   // ��ʾ������   ������ʾ2λ   С����ʾ��λ
// @note                        �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//                              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//                              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//                              ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_float (uint16 x, uint16 y, const float dat, uint8 num, uint8 pointnum)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);
    zf_assert(num > 0);
    zf_assert(num <= 8);
    zf_assert(pointnum > 0);
    zf_assert(pointnum <= 6);

    float dat_temp = dat;
    float offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num+pointnum+2);

    if(num < 10)
    {
        for(; num>0; num--)
            offset *= 10;
        dat_temp = dat_temp-((int)dat_temp/(int)offset)*offset;
    }
    float_to_str(data_buffer, dat_temp, pointnum);
    ips200_show_string(x,y,data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 ��ʾ��ֵͼ�� ����ÿ�˸������һ���ֽ�����
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       *image          ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips200_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips200_y_max]
// @return      void
// Sample usage:                ips200_show_binary_image(0,0,ov7725_image_binary[0],OV7725_W,OV7725_H,OV7725_W,OV7725_H);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_binary_image (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32 i = 0, j = 0;
    uint8 temp = 0;
    uint32 width_index = 0, height_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(image+height_index*width/8+width_index/8);                 // ��ȡ���ص�
            if(0x80 & (temp << (width_index%8)))
                ips200_write_16bit_data(RGB565_WHITE);
            else
                ips200_write_16bit_data(RGB565_BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 ��ʾ 8bit �Ҷ�ͼ�� ����ֵ����ֵ
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       *image          ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips200_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips200_y_max]
// @param       threshold       ��ֵ����ʾ��ֵ 0-��������ֵ��
// @return      void
// Sample usage:                ips200_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,MT9V03X_W,MT9V03X_H,0);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_gray_image (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32 i = 0, j = 0;
    uint16 color = 0,temp = 0;
    uint32 width_index = 0, height_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(image+height_index*width+width_index);                     // ��ȡ���ص�
            if(threshold == 0)
            {
                color=(0x001f&((temp)>>3))<<11;
                color=color|(((0x003f)&((temp)>>2))<<5);
                color=color|(0x001f&((temp)>>3));
                ips200_write_16bit_data(color);
            }
            else if(temp < threshold)
                ips200_write_16bit_data(RGB565_BLACK);
            else
                ips200_write_16bit_data(RGB565_WHITE);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 ��ʾ RGB565 ��ɫͼ��
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       *image          ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips200_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips200_y_max]
// @param       color_mode      ɫ��ģʽ 0-��λ��ǰ 1-��λ��ǰ
// @return      void
// Sample usage:                ips200_show_rgb565_image(0,0,camera_buffer_addr,SCCB8660_W,SCCB8660_H,SCCB8660_W,SCCB8660_H,1);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_rgb565_image (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32 i = 0, j = 0;
    uint16 color = 0;
    uint32 width_index = 0, height_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            color = *(image+height_index*width+width_index);                    // ��ȡ���ص�
            if(color_mode)
                color = ((color&0xff)<<8) | (color>>8);
            ips200_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 ��ʾ����
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       *wave           ��������ָ��
// @param       width           ����ʵ�ʿ��
// @param       value_max       ����ʵ�����ֵ
// @param       dis_width       ������ʾ��� ������Χ [0, ips200_x_max]
// @param       dis_value_max   ������ʾ���ֵ ������Χ [0, ips200_y_max]
// @return      void
// Sample usage:                ips200_show_wave(0,0,data,128,64,64,32);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_wave (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32 i = 0, j = 0;
    uint32 width_index = 0, value_max_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_value_max-1);                  // ������ʾ����
    for(i=0;i<dis_value_max;i++)
    {
        for(j=0;j<dis_width;j++)
        {
            ips200_write_16bit_data(ips200_bgcolor);
        }
    }

    for(i=0;i<dis_width;i++)
    {
        width_index = i*width/dis_width;
        value_max_index = *(wave+width_index)*(dis_value_max-1)/value_max;
        ips200_draw_point(i+x, (dis_value_max-1)-value_max_index+y, ips200_pencolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       size            ȡģ��ʱ�����õĺ��������С Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��� ȡģ��ʱ����Ҫ������һ����
// @param       *chinese_buffer ��Ҫ��ʾ�ĺ�������
// @param       number          ��Ҫ��ʾ����λ
// @param       color           ��ʾ��ɫ
// @return      void
// Sample usage:                ips200_show_chinese(0,0,16,chinese_test[0],4,RBB565_RED);//��ʾfont�ļ������ ʾ��
// @Note                        ʹ��PCtoLCD2002���ȡģ           ���롢����ʽ��˳��   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_chinese (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    int i, j, k; 
    uint8 temp, temp1, temp2;
    const uint8 *p_data;
    
    temp2 = size/8;
    
    ips200_set_region(x,y,number*size-1+x,y+size-1);
    
    for(i=0;i<size;i++)
    {
        temp1 = number;
        p_data = chinese_buffer+i*temp2;
        while(temp1--)
        {
            for(k=0;k<temp2;k++)
            {
                for(j=8;j>0;j--)
                {
                    temp = (*p_data>>(j-1)) & 0x01;
                    if(temp)    ips200_write_16bit_data(color);
                    else        ips200_write_16bit_data(ips200_bgcolor);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }   
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      2.0�� IPSҺ��GPIO��ʼ��
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips200_gpio_init(void)
{
    uint8 i = 0;
    uint8 pin_value = (((uint32)IPS200_DATAPORT - GPIOA_BASE)/0x400) * 0x20 + DATA_START_NUM;  // ��ȡ�˿ڵ�ö����ֵ

    gpio_init(IPS200_RD_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_WR_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_RS_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_RST_PIN, GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_CS_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_BL_PIN,  GPO, 1, GPO_PUSH_PULL);

//#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
//#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
//#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
//#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
//#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)
//#define GPIOF_BASE            (APB2PERIPH_BASE + 0x1C00)
//#define GPIOG_BASE            (APB2PERIPH_BASE + 0x2000)

    for(i = 0;i < 8;i++)
    {
        gpio_init((gpio_pin_enum)(pin_value + i), GPO, 0, GPO_PUSH_PULL);
    }

}


//-------------------------------------------------------------------------------------------------------------------
// @brief       2�� IPSҺ����ʼ��
// @param       void
// @return      void
// Sample usage:                ips200_init();
//-------------------------------------------------------------------------------------------------------------------
void ips200_init (void)
{
    ips200_gpio_init();

    ips200_set_dir(ips200_display_dir);

    IPS200_BL(1);
    IPS200_RST(0);
    system_delay_ms(5);
    IPS200_RST(1);
    system_delay_ms(120);

    ips200_write_command(0x11);
    system_delay_ms(120);

    ips200_write_command(0x36);
    if      (ips200_display_dir==0) ips200_write_8bit_data(0x00);
    else if (ips200_display_dir==1) ips200_write_8bit_data(0xC0);
    else if (ips200_display_dir==2) ips200_write_8bit_data(0x70);
    else                            ips200_write_8bit_data(0xA0);

    ips200_write_command(0x3A);
    ips200_write_8bit_data(0x05);

    ips200_write_command(0xB2);
    ips200_write_8bit_data(0x0C);
    ips200_write_8bit_data(0x0C);
    ips200_write_8bit_data(0x00);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x33);

    ips200_write_command(0xB7);
    ips200_write_8bit_data(0x35);

    ips200_write_command(0xBB);
    ips200_write_8bit_data(0x29); //32 Vcom=1.35V

    ips200_write_command(0xC2);
    ips200_write_8bit_data(0x01);

    ips200_write_command(0xC3);
    ips200_write_8bit_data(0x19); //GVDD=4.8V

    ips200_write_command(0xC4);
    ips200_write_8bit_data(0x20); //VDV, 0x20:0v

    ips200_write_command(0xC5);
    ips200_write_8bit_data(0x1A);//VCOM Offset Set

    ips200_write_command(0xC6);
    ips200_write_8bit_data(0x01F); //0x0F:60Hz

    ips200_write_command(0xD0);
    ips200_write_8bit_data(0xA4);
    ips200_write_8bit_data(0xA1);

    ips200_write_command(0xE0);
    ips200_write_8bit_data(0xD0);
    ips200_write_8bit_data(0x08);
    ips200_write_8bit_data(0x0E);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x05);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x48);
    ips200_write_8bit_data(0x17);
    ips200_write_8bit_data(0x14);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x34);

    ips200_write_command(0xE1);
    ips200_write_8bit_data(0xD0);
    ips200_write_8bit_data(0x08);
    ips200_write_8bit_data(0x0E);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x48);
    ips200_write_8bit_data(0x17);
    ips200_write_8bit_data(0x14);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x34);

    ips200_write_command(0x21);

    ips200_write_command(0x29);

    ips200_clear(ips200_bgcolor);   //��ʼ��Ϊ����
}
