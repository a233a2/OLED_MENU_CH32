#include "zf_common_headfile.h"
uint8 key_flag = 0;  //保存按键状态的标志位
int main(void)
{
    interrupt_global_disable();            // 关闭总中断
    clock_init(SYSTEM_CLOCK_144M);      // 务必保留，设置系统时钟。
    debug_init();                       // 务必保留，本函数用于初始化MPU 时钟 调试串口

    Key_Init();
    OLED_Init();//IIC通信屏幕init 4针
    OLED_ColorTurn(0);                       //0正常显示，1 反色显示
    OLED_DisplayTurn(0);                       //0正常显示 1 屏幕翻转显示

    interrupt_global_enable();            // 总中断最后开启

//    oled_init();  //SPI通信屏幕init 7针
//    uart_rx_irq(UART_3, ENABLE);


    //本例程作用  点按显示派蒙图片 长按则显示文字
    while(1)
    {
//        Example_Class();
        Menu_key_set();
    }
}


void Example_Class(void)
{
        key_status=Key_Scan();    //按键扫描函数
        if(key_status==1)//点按
        {
            key_flag=1;
        }
        else if(key_status==2)    //长按
        {
            key_flag=2;
        }
        if(key_flag==1)    //点按
        {
            //OLED显示图片的例程
            OLED_ShowPicture(0,0,128,8,paimeng);
        }
        else if(key_flag==2)    //长按
        {
            //OLED显示文字的例程
            OLED_ShowChinese(0,0,0,16);//中
            OLED_ShowChinese(18,0,1,16);//景
            OLED_ShowChinese(36,0,2,16);//园
            OLED_ShowChinese(54,0,3,16);//电
            OLED_ShowChinese(72,0,4,16);//子
            OLED_ShowChinese(90,0,5,16);//科
            OLED_ShowChinese(108,0,6,16);//技
            OLED_Refresh();
        }
      oled_show_float(0, 0, 123.2, 3, 3);
}
