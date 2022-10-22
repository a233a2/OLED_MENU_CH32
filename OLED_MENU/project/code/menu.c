#include "zf_common_headfile.h"

typedef struct
{
    uint8_t current;//��ǰ״̬������
    uint8_t next; //����һ��      key_status=1 �㰴
    uint8_t enter; //ȷ��           key_status=2 ����
    void (*current_operation)(void); //��ǰ״̬Ӧ��ִ�еĲ���
} Menu_table;

Menu_table  table[13]=   //������ �㰴 ���� ����
{
    {0,1,3,(*Main_1)},//11
    {1,2,4,(*Main_2)},//12
    {2,0,5,(*Main_3)},//13

    {3,3,0,(*Branch_11)},//21
    {4,4,1,(*Branch_21)},//22
    {5,5,2,(*Branch_31)},//23
};

uint8_t  func_index = 0;//�������ʱ���ڽ��������ֵ
void (*current_operation_index)(); //ִ�е�ǰ��������


void Menu_key_set(void)
{
    key_status=Key_Scan();    //����ɨ�躯��
    if(key_status==1)//�㰴
    {
        func_index=table[func_index].next;//����next���º��������
        OLED_Clear();
    }
    if(key_status==2)//����
    {
        func_index=table[func_index].enter;//����next���º��������
        OLED_Clear();
    }
    key_status=0;

    current_operation_index=table[func_index].current_operation;//ִ�е�ǰ����������Ӧ�Ĺ��ܺ�����
    (*current_operation_index)();//ִ�е�ǰ��������
}

void Main_1(void)
{
    OLED_ShowString(0,0,(u8 *)"  >Picture display",12);
    OLED_ShowString(0,14,(u8 *)"   Text display",12);
    OLED_ShowString(0,28,(u8 *)"   Digital display",12);
    OLED_Refresh();
}

void Main_2(void)
{
    OLED_ShowString(0,0,(u8 *)"   Picture display",12);
    OLED_ShowString(0,14,(u8 *)"  >Text display",12);
    OLED_ShowString(0,28,(u8 *)"   Digital display",12);
    OLED_Refresh();
}
void Main_3(void)
{
    OLED_ShowString(0,0,(u8 *)"   Picture display",12);
    OLED_ShowString(0,14,(u8 *)"   Text display",12);
    OLED_ShowString(0,28,(u8 *)"  >Digital display",12);
    OLED_Refresh();
}
void Branch_11(void)//ͼƬ��ʾ
{
    OLED_ShowPicture(0,0,128,8,paimeng);
}
void Branch_21(void)//������ʾ
{
    //OLED��ʾ���ֵ�����
    OLED_ShowChinese(0,0,0,16);//��
    OLED_ShowChinese(18,0,1,16);//��
    OLED_ShowChinese(36,0,2,16);//԰
    OLED_ShowChinese(54,0,3,16);//��
    OLED_ShowChinese(72,0,4,16);//��
    OLED_ShowChinese(90,0,5,16);//��
    OLED_ShowChinese(108,0,6,16);//��
    OLED_Refresh();
}
void Branch_31(void)//������ʾ
{
    OLED_ShowString(0,0,(u8 *)"   202011050094",12);
    OLED_Refresh();
}
