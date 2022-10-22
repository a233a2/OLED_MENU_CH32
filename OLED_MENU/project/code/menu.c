#include "zf_common_headfile.h"

typedef struct
{
    uint8_t current;//当前状态索引号
    uint8_t next; //向下一个      key_status=1 点按
    uint8_t enter; //确定           key_status=2 长按
    void (*current_operation)(void); //当前状态应该执行的操作
} Menu_table;

Menu_table  table[13]=   //索引号 点按 长按 函数
{
    {0,1,3,(*Main_1)},//11
    {1,2,4,(*Main_2)},//12
    {2,0,5,(*Main_3)},//13

    {3,3,0,(*Branch_11)},//21
    {4,4,1,(*Branch_21)},//22
    {5,5,2,(*Branch_31)},//23
};

uint8_t  func_index = 0;//主程序此时所在界面的索引值
void (*current_operation_index)(); //执行当前操作函数


void Menu_key_set(void)
{
    key_status=Key_Scan();    //按键扫描函数
    if(key_status==1)//点按
    {
        func_index=table[func_index].next;//按键next按下后的索引号
        OLED_Clear();
    }
    if(key_status==2)//长按
    {
        func_index=table[func_index].enter;//按键next按下后的索引号
        OLED_Clear();
    }
    key_status=0;

    current_operation_index=table[func_index].current_operation;//执行当前索引号所对应的功能函数。
    (*current_operation_index)();//执行当前操作函数
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
void Branch_11(void)//图片显示
{
    OLED_ShowPicture(0,0,128,8,paimeng);
}
void Branch_21(void)//文字显示
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
void Branch_31(void)//数字显示
{
    OLED_ShowString(0,0,(u8 *)"   202011050094",12);
    OLED_Refresh();
}
