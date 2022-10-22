#include "zf_common_headfile.h"
#define Long_Press_Time_Judge 250  //超过这个值就表示长按，且注意两次点按的间隔不能超过这个值

#define Long_Press  2   //长按返回值为2
#define Short_Press 1   //点按返回值为1

uint8 Key_Flag; //按键标志位
uint8 key_status = 0;  //按键状态实时获取
//按键初始化
void Key_Init(void) {
    gpio_init(B3, GPO, 1, GPI_PULL_UP);  //按下IO状态为0  未按下为1
}

//key_status=Key_Scan();
uint8_t Key_Scan(void) //作者:https://blog.csdn.net/weixin_44155115/article/details/124263473
{
    uint8_t long_press_flag;  //长按标志位
    uint8_t short_press_flag;  //点按标志位
    if (USR_KEY == 0)  //如果按键按下
            {
        system_delay_ms(10);     //按键消抖    system_delay_ms(50);

        if (USR_KEY == 0) //消抖之后依然为按下
                {
            short_press_flag = 1; //运行到这里说明确实有按键按下，默认判断为短按

            system_delay_ms(Long_Press_Time_Judge);  //点按的时间不能多于250ms

            if (USR_KEY == 0)  //等待了Long_Press_Time_Judge按键还是按下说明此次为长按
                    {
                long_press_flag = 1;
            }
            while(!USR_KEY);
            //等待按键松开
            if (long_press_flag == 1)  //先判断是否长按，因为长按中包括了点按
                    {
                return Long_Press;
            }
            if (short_press_flag == 1) //后判断是否点按
                    {
                return Short_Press;
            }
        }
    }
    return 0;
}

