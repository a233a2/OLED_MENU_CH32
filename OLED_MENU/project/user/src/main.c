#include "zf_common_headfile.h"
uint8 key_flag = 0;  //���水��״̬�ı�־λ
int main(void)
{
    interrupt_global_disable();            // �ر����ж�
    clock_init(SYSTEM_CLOCK_144M);      // ��ر���������ϵͳʱ�ӡ�
    debug_init();                       // ��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    Key_Init();
    OLED_Init();//IICͨ����Ļinit 4��
    OLED_ColorTurn(0);                       //0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);                       //0������ʾ 1 ��Ļ��ת��ʾ

    interrupt_global_enable();            // ���ж������

//    oled_init();  //SPIͨ����Ļinit 7��
//    uart_rx_irq(UART_3, ENABLE);


    //����������  �㰴��ʾ����ͼƬ ��������ʾ����
    while(1)
    {
//        Example_Class();
        Menu_key_set();
    }
}


void Example_Class(void)
{
        key_status=Key_Scan();    //����ɨ�躯��
        if(key_status==1)//�㰴
        {
            key_flag=1;
        }
        else if(key_status==2)    //����
        {
            key_flag=2;
        }
        if(key_flag==1)    //�㰴
        {
            //OLED��ʾͼƬ������
            OLED_ShowPicture(0,0,128,8,paimeng);
        }
        else if(key_flag==2)    //����
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
      oled_show_float(0, 0, 123.2, 3, 3);
}
