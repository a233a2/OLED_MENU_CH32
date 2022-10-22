/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
* @file             zf_common_function
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#include "zf_common_function.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ�����������Լ�� ��������֮���������
// @param       num1            ����1
// @param       num2            ����2
// @return      uint32          ���Լ��
// Sample usage:                return get_greatest_common_divisor(144, 36);    // ��ȡ 144 �� 36 �����Լ��
//-------------------------------------------------------------------------------------------------------------------
uint32 get_greatest_common_divisor (uint32 num1, uint32 num2)
{
    while(num1 != num2)
    {
        if(num1 > num2)
            num1 = num1 -num2;
        if(num1 < num2)
            num2 = num2 - num1;
    }
    return num1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ���� ���ݷ�Χ�� [-32767,32767]
// @param       dat             ��Ҫ�����ֵ����
// @return      int             ���ؾ���ֵ
// Sample usage:                dat = myabs(dat);                               // ��dat�������
//-------------------------------------------------------------------------------------------------------------------
//int  myabs (int dat)                                                          // ����鿴 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       �޷� ���ݷ�Χ�� [-32768,32767]
// @param       x               ���޷�������
// @param       y               �޷���Χ(���ݻᱻ������-y��+y֮��)
// @return      int             �޷�֮�������
// Sample usage:                int dat = limit(500,300);                       // ���ݱ�������-300��+300֮��  ��˷��صĽ����300
//-------------------------------------------------------------------------------------------------------------------
//int limit(int x, int32 y)                                                     // ����鿴 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       ˫���޷� ���ݷ�Χ�� [-32768,32767]
// @param       x               ���޷�������
// @param       a               �޷���Χ��߽�
// @param       b               �޷���Χ�ұ߽�
// @return      int             �޷�֮�������
// Sample usage:                int dat = limit_ab(500,-300,400);               //���ݱ�������-300��+400֮��  ��˷��صĽ����400
//-------------------------------------------------------------------------------------------------------------------
//int limit_ab(int x, int a, int b)                                             // ����鿴 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       �����ʱ
// @param       t               ��ʱʱ��
// @return      void
// Sample usage:                my_delay(100);                                  // ��ʼ������ʱ��Ϊ 96MHz
//-------------------------------------------------------------------------------------------------------------------
void my_delay (volatile long t)
{
    while(t--);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ַ���ת�������� ���ݷ�Χ�� [-32768,32767]
// @param       str             �����ַ��� �ɴ�����
// @return      int             ת���������
// Sample usage:                int32 dat = str_to_int("-100");
//-------------------------------------------------------------------------------------------------------------------
int32 str_to_int (char *str)
{
    uint8 sign = 0;
    int32 temp;

    if('-' == *str)
    {
        sign = 1;
        str ++;
    }
    else if('+' == *str)
    {
        str ++;
    }

    while(('0' <= *str)&&('9' >= *str))
    {
        temp = temp*10+((uint8)(*str)-0x30);
        str ++;
    }

    if(sign)
        return -temp;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ַ���ת�������� ���ݷ�Χ�� [0,65535]
// @param       str             �����ַ��� �޷���
// @return      int             ת���������
// Sample usage:                uint32 dat = str_to_uint("100");
//-------------------------------------------------------------------------------------------------------------------
uint32 str_to_uint (char *str)
{
    uint32 temp;

    while(('0' <= *str)&&('9' >= *str))
    {
        temp = temp*10+((uint8)(*str)-0x30);
        str ++;
    }

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��������ת�ַ��� ���ݷ�Χ�� [-32768,32767]
// @param       *str            �ַ���ָ��
// @param       number          ���������
// @return      NULL
// Sample usage:                int32o_str(data_buffer, -300);
//-------------------------------------------------------------------------------------------------------------------
void int_to_str (char *str, int32 number)
{
    uint8 data_temp[16];
    uint8 bit = 0;

    if(0 > number)
    {
        *str++ = '-';
    }
    else if(0 == number)
    {
        *str = '0';
        return;
    }

    // int [-32768,32767]
    while(0 != number)
    {
        data_temp[bit++] = myabs(number%10);
        number /= 10;
    }
    while(0 != bit)
    {
        *str++ = (data_temp[bit-1]+0x30);
        bit--;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��������ת�ַ��� ���ݷ�Χ�� [0,65535]
// @param       *str            �ַ���ָ��
// @param       number          ���������
// @return      NULL
// Sample usage:                uint_to_str(data_buffer, 300);
//-------------------------------------------------------------------------------------------------------------------
void uint_to_str (char *str, uint32 number)
{
    int8 data_temp[16];
    uint8 bit = 0;

    if(0 == number)
    {
        *str = '0';
        return;
    }

    while(0 != number)
    {
        data_temp[bit++] = number%10;
        number /= 10;
    }
    while(0 != bit)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ַ���ת������
// @param       str             �����ַ��� �ɴ�����
// @return      float           ת���������
// Sample usage:                float dat = str_to_float("-100.2");
//-------------------------------------------------------------------------------------------------------------------
float str_to_float (char *str)
{
    uint8 sign = 0;
    float temp = 0.0;
    float temp_point = 0.0;
    uint8 point_bit = 0;

    if('-' == *str)
    {
        sign = 1;
        str ++;
    }
    else if('+' == *str)
    {
        str ++;
    }

    while(('0' <= *str)&&('9' >= *str))
    {
        temp = temp*10+((uint8)(*str)-0x30);
        str ++;
    }
    if('.' == *str)
    {
        str++;
        while(('0' <= *str)&&('9' >= *str))
        {
            temp_point = temp_point*10+((uint8)(*str)-0x30);
            point_bit++;
            str ++;
        }
        while(point_bit--)
        {
            temp_point /= 10;
        }
    }
    temp += temp_point;

    if(sign)
        return -temp;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��������ת�ַ���
// @param       *str            �ַ���ָ��
// @param       number          ���������
// @param       point_bit       С���㾫��
// @return      NULL
// Sample usage:                float_to_str(data_buffer, 3.1415, 2);           // ������ data_buffer = "3.14"
//-------------------------------------------------------------------------------------------------------------------
void float_to_str (char *str, float number, uint8 point_bit)
{
    int data_int = 0;
    int data_float = 0.0;
    int data_temp[5];
    int data_temp_point[5];
    uint8 bit = 0;

    data_int = (int)number;
    if(0 > number)
    {
        *str++ = '-';
    }
    else if(0.0 == number)
    {
        *str++ = '0';
        *str++ = '.';
        *str = '0';
        return;
    }

    number = number - data_int;
    while(point_bit--)
    {
        number = number*10;
    }
    data_float = (int)number;

    while(0 != data_int)
    {
        data_temp[bit++] = data_int%10;
        data_int /= 10;
    }
    while(0 != bit)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
    *str++ = '.';
    if(0 == data_float)
        *str = '0';
    else
    {
        while(0 != data_float)
        {
            data_temp_point[bit++] = data_float%10;
            data_float /= 10;
        }
        while(0 != bit)
        {
            *str++ = (myabs(data_temp_point[bit-1])+0x30);
            bit--;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ַ���ת������
// @param       str             �����ַ��� �ɴ�����
// @return      double          ת���������
// Sample usage:                double dat = str_to_double("-100.2");
//-------------------------------------------------------------------------------------------------------------------
double str_to_double (char *str)
{
    uint8 sign = 0;
    double temp = 0.0;
    double temp_point = 0.0;
    uint8 point_bit = 0;

    if('-' == *str)
    {
        sign = 1;
        str ++;
    }
    else if('+' == *str)
    {
        str ++;
    }

    while(('0' <= *str)&&('9' >= *str))
    {
        temp = temp*10+((uint8)(*str)-0x30);
        str ++;
    }
    if('.' == *str)
    {
        str++;
        while(('0' <= *str)&&('9' >= *str))
        {
            temp_point = temp_point*10+((uint8)(*str)-0x30);
            point_bit++;
            str ++;
        }
        while(point_bit--)
        {
            temp_point /= 10;
        }
    }
    temp += temp_point;

    if(sign)
        return -temp;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��������ת�ַ���
// @param       *str            �ַ���ָ��
// @param       number          ���������
// @param       point_bit       С���㾫��
// @return      NULL
// Sample usage:                double_to_str(data_buffer, 3.1415, 2);           // ������ data_buffer = "3.14"
//-------------------------------------------------------------------------------------------------------------------
void double_to_str (char *str, double number, uint8 point_bit)
{
    int data_int = 0;
    int data_float = 0.0;
    int data_temp[5];
    int data_temp_point[5];
    uint8 bit = 0;

    data_int = (int)number;
    if(0 > number)
    {
        *str++ = '-';
    }
    else if(0.0 == number)
    {
        *str++ = '0';
        *str++ = '.';
        *str = '0';
        return;
    }

    number = number - data_int;
    while(point_bit--)
    {
        number = number*10;
    }
    data_float = (int)number;

    while(0 != data_int)
    {
        data_temp[bit++] = data_int%10;
        data_int /= 10;
    }
    while(0 != bit)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
    *str++ = '.';
    if(0 == data_float)
        *str = '0';
    else
    {
        while(0 != data_float)
        {
            data_temp_point[bit++] = data_float%10;
            data_float /= 10;
        }
        while(0 != bit)
        {
            *str++ = (myabs(data_temp_point[bit-1])+0x30);
            bit--;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת��Ϊ ASCII ֵ �ڲ�����
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static uint8 number_conversion_ascii (uint32 dat, int8 *p, uint8 neg_type, uint8 radix)
{
    int32   neg_dat;
    uint32  pos_dat;
    uint8   temp_data = 0;
    uint8   valid_num = 0;

    if(neg_type)
    {
        neg_dat = (int32)dat;
        if(0 > neg_dat)   neg_dat = -neg_dat;
        while(1)
        {
            *p = neg_dat%radix + '0';
            neg_dat = neg_dat/radix;
            valid_num++;

            if(!neg_dat) break;
            p++;
        }
    }
    else
    {
        pos_dat = dat;
        while(1)
        {
            temp_data = pos_dat%radix;
            if(10 <= temp_data) temp_data += 'A'-10;
            else                temp_data += '0';

            *p = temp_data;

            pos_dat = pos_dat/radix;
            valid_num++;

            if(!pos_dat) break;
            p++;
        }
    }
    return valid_num;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       printf ��ʾת�� �ڲ�����
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void printf_reverse_order (int8 *d_buff, uint32 len)
{
    uint32 i;
    int8  temp_data;
    for(i=0;len/2>i;i++)
    {
        temp_data = d_buff[len-1-i];
        d_buff[len-1-i] = d_buff[i];
        d_buff[i] = temp_data; 
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       sprintf ����ʵ��
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_sprintf (int8 *buff, const int8 *format, ...)
{
    uint32 buff_len=0;
    va_list arg;
    va_start(arg, format);

    while (*format)
    {
        int8 ret = *format;
        if ('%' == ret)
        {
            switch (*++format)
            {
                case 'a':// ʮ������p��������������� ��δʵ��
                    {
                    }
                    break;

                case 'c':// һ���ַ�
                    {
                        int8 ch = (int8)va_arg(arg, uint32);
                        *buff = ch;
                        buff++;
                        buff_len++;
                    }
                    break;

                case 'd':
                case 'i':// �з���ʮ��������
                    {
                        int8 vstr[33];
                        int32 ival = (int32)va_arg(arg, int32);
                        uint8 vlen = number_conversion_ascii((uint32)ival, vstr, 1, 10);

                        if(0>ival)
                        {
                            vstr[vlen] = '-';
                            vlen++;
                        }
                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 'f':// �����������С�������λ  ����ָ���������
                case 'F':// �����������С�������λ  ����ָ���������
                {
                    int8 vstr[33];
                    double ival = (double)va_arg(arg, double);
                    uint8 vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);

                    if(0>ival)
                    {
                        vstr[vlen] = '-';
                        vlen++;
                    }
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff += vlen;
                    buff_len += vlen;

                    ival = ((double)ival - (int32)ival)*1000000;
                    if(ival)
                    {
                        vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
                    }
                    else
                    {
                        vstr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
                        vlen = 6;
                    }

                    while(6>vlen)
                    {
                        vstr[vlen] = '0';
                        vlen++;
                    }

                    vstr[vlen] = '.';
                    vlen++;

                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff_len += vlen;
                }
                break;

                case 'u':// �޷���ʮ��������
                    {
                        int8 vstr[33];
                        uint32 ival = (uint32)va_arg(arg, uint32);
                        uint8 vlen = number_conversion_ascii(ival, vstr, 0, 10);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 'o':// �޷��Ű˽�������
                    {
                        int8 vstr[33];
                        uint32 ival = (uint32)va_arg(arg, uint32);
                        uint8 vlen = number_conversion_ascii(ival, vstr, 0, 8);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;

                    }
                    break;

                case 'x':// �޷���ʮ����������
                case 'X':// �޷���ʮ����������
                    {
                        int8 vstr[33];
                        uint32 ival = (uint32)va_arg(arg, uint32);
                        uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 's':// �ַ���
                    {
                        int8 *pc = va_arg(arg, int8 *);
                        while (*pc)
                        {
                            *buff = *pc;
                            buff++;
                            buff_len++;
                            pc++;
                        }
                    }
                    break;

                case 'p':// ��16������ʽ���ָ��
                    {
                        int8 vstr[33];
                        //uint32 ival = (uint32)va_arg(arg, uint32);
                        //uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                        printf_reverse_order(vstr,8);
                        memcpy(buff,vstr,8);
                        buff += 8;
                        buff_len += 8;
                    }
                    break;

                case '%':// ����ַ�%
                    {
                        *buff = '%';
                        buff++;
                        buff_len++;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            *buff = (int8)(*format);
            buff++;
            buff_len++;
        }
        format++;
    }
    va_end(arg);

    return buff_len;
}

