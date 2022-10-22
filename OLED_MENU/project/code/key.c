#include "zf_common_headfile.h"
#define Long_Press_Time_Judge 250  //�������ֵ�ͱ�ʾ��������ע�����ε㰴�ļ�����ܳ������ֵ

#define Long_Press  2   //��������ֵΪ2
#define Short_Press 1   //�㰴����ֵΪ1

uint8 Key_Flag; //������־λ
uint8 key_status = 0;  //����״̬ʵʱ��ȡ
//������ʼ��
void Key_Init(void) {
    gpio_init(B3, GPO, 1, GPI_PULL_UP);  //����IO״̬Ϊ0  δ����Ϊ1
}

//key_status=Key_Scan();
uint8_t Key_Scan(void) //����:https://blog.csdn.net/weixin_44155115/article/details/124263473
{
    uint8_t long_press_flag;  //������־λ
    uint8_t short_press_flag;  //�㰴��־λ
    if (USR_KEY == 0)  //�����������
            {
        system_delay_ms(10);     //��������    system_delay_ms(50);

        if (USR_KEY == 0) //����֮����ȻΪ����
                {
            short_press_flag = 1; //���е�����˵��ȷʵ�а������£�Ĭ���ж�Ϊ�̰�

            system_delay_ms(Long_Press_Time_Judge);  //�㰴��ʱ�䲻�ܶ���250ms

            if (USR_KEY == 0)  //�ȴ���Long_Press_Time_Judge�������ǰ���˵���˴�Ϊ����
                    {
                long_press_flag = 1;
            }
            while(!USR_KEY);
            //�ȴ������ɿ�
            if (long_press_flag == 1)  //���ж��Ƿ񳤰�����Ϊ�����а����˵㰴
                    {
                return Long_Press;
            }
            if (short_press_flag == 1) //���ж��Ƿ�㰴
                    {
                return Short_Press;
            }
        }
    }
    return 0;
}

