/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_flash
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 * @note            version:
 *                  V1.1 2022.01.17 �޸�����������ö���嶨��
 ********************************************************************************************************************/

#ifndef _zf_flash_h
#define _zf_flash_h

#include "ch32v30x_misc.h"
#include "ch32v30x_flash.h"
#include "zf_common_typedef.h"

#define FLASH_BASE_ADDR             (0x08000000)                // FALSH�׵�ַ
#define FLASH_PAGE_SIZE             (0x00000400)                // 1K byte
#define FLASH_SECTION_SIZE          (FLASH_PAGE_SIZE*4)         // 4K byte
#define FLASH_OPERATION_TIME_OUT    0x0FFF

#define FLASH_DATA_BUFFER_SIZE      (FLASH_PAGE_SIZE/sizeof(flash_data_union))  // �Զ�����ÿ��ҳ�ܹ����¶��ٸ�����

typedef union                                                                   // �̶������ݻ��嵥Ԫ��ʽ
{
    float   float_type;                                                       // float  ����
    uint32  uint32_type;                                                      // uint32 ����
    int32   int32_type;                                                       // int32  ����
    uint16  uint16_type;                                                      // uint16 ����
    int16   int16_type;                                                       // int16  ����
    uint8   uint8_type;                                                       // uint8  ����
    int8    int8_type;                                                        // int8   ����
}flash_data_union;                                                              // �����������ݹ���ͬһ�� 32bit ��ַ


/*
     ����Դ��ʹ�õ���64K RAM+ 256KFLASH
  flash���� һ��64��������һ������4K��
*/


// ö�� Flash ����  ��ö�ٶ��岻�����û��޸�
typedef enum
{
    FLASH_SECTION_00,
    FLASH_SECTION_01,
    FLASH_SECTION_02,
    FLASH_SECTION_03,
    FLASH_SECTION_04,
    FLASH_SECTION_05,
    FLASH_SECTION_06,
    FLASH_SECTION_07,
    FLASH_SECTION_08,
    FLASH_SECTION_09,
    FLASH_SECTION_10,
    FLASH_SECTION_11,
    FLASH_SECTION_12,
    FLASH_SECTION_13,
    FLASH_SECTION_14,
    FLASH_SECTION_15,
    FLASH_SECTION_16,
    FLASH_SECTION_17,
    FLASH_SECTION_18,
    FLASH_SECTION_19,
    FLASH_SECTION_20,
    FLASH_SECTION_21,
    FLASH_SECTION_22,
    FLASH_SECTION_23,
    FLASH_SECTION_24,
    FLASH_SECTION_25,
    FLASH_SECTION_26,
    FLASH_SECTION_27,
    FLASH_SECTION_28,
    FLASH_SECTION_29,
    FLASH_SECTION_30,
    FLASH_SECTION_31,
    FLASH_SECTION_32,
    FLASH_SECTION_33,
    FLASH_SECTION_34,
    FLASH_SECTION_35,
    FLASH_SECTION_36,
    FLASH_SECTION_37,
    FLASH_SECTION_38,
    FLASH_SECTION_39,
    FLASH_SECTION_40,
    FLASH_SECTION_41,
    FLASH_SECTION_42,
    FLASH_SECTION_43,
    FLASH_SECTION_44,
    FLASH_SECTION_45,
    FLASH_SECTION_46,
    FLASH_SECTION_47,
    FLASH_SECTION_48,
    FLASH_SECTION_49,
    FLASH_SECTION_50,
    FLASH_SECTION_51,
    FLASH_SECTION_52,
    FLASH_SECTION_53,
    FLASH_SECTION_54,
    FLASH_SECTION_55,
    FLASH_SECTION_56,
    FLASH_SECTION_57,
    FLASH_SECTION_58,
    FLASH_SECTION_59,
    FLASH_SECTION_60,
    FLASH_SECTION_61,
    FLASH_SECTION_62,
    FLASH_SECTION_63
}flash_section_enum;

// ö�� Flash ҳ���� ��ö�ٶ��岻�����û��޸�
typedef enum
{
    FLASH_PAGE_0,
    FLASH_PAGE_1,
    FLASH_PAGE_2,
    FLASH_PAGE_3,
}flash_page_enum;

extern flash_data_union flash_data_union_buffer[FLASH_DATA_BUFFER_SIZE];

uint8 flash_check         (flash_section_enum sector_num, flash_page_enum page_num);
uint8 flash_erase_page    (flash_section_enum sector_num, flash_page_enum page_num);
void    flash_read_page     (flash_section_enum sector_num, flash_page_enum page_num, uint32 *buf, uint16 len);
uint8 flash_write_page    (flash_section_enum sector_num, flash_page_enum page_num, const uint32 *buf, uint16 len);

void    flash_read_page_to_data_buffer      (flash_section_enum sector_num, flash_page_enum page_num);
uint8 flash_write_page_to_data_buffer     (flash_section_enum sector_num, flash_page_enum page_num);
void    flash_data_buffer_clear             (void);


#endif
