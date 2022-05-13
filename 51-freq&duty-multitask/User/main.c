#include "public.h"
#include "lcd1602.h"
#include "freq-duty.c"

void voidf(void){}
#define APP_NUM 3

typedef void (*void_func)(void);//定义函数指针,便于移植应用的中断函数
void_func timer0_func=voidf;
void_func timer1_func=voidf;
//定义应用函数结构体
typedef struct{
    void_func app9;
    char ctrlc;
    void_func exit9;
}func_app;

//定义应用池
func_app app_pool[APP_NUM];
char app_num=APP_NUM;

//应用1：方波信号占空比&频率测定
void freq_duty_init(){
    timer0_func=timer0_fd;
    timer1_func=timer1_fd;
    main_fd(&(app_pool[0].ctrlc));
    //这里只有用取地址才能实现控制应用
}
void freq_duty_exit(void){
    timer0_func=voidf;
    timer1_func=voidf;
}
//应用池初始化
void app_pool_init(void)
{
    app_pool[0].app9=freq_duty_init;
    app_pool[0].ctrlc=0;
    app_pool[0].exit9=freq_duty_exit;
    app_pool[1].app9=voidf;
    app_pool[1].ctrlc=0;
    app_pool[1].exit9=voidf;
    app_pool[2].app9=voidf;
    app_pool[2].ctrlc=0;
    app_pool[2].exit9=voidf;
}

void main()
{	
	EA=1;//打开总中断
    //初始化外部中断0，用来切换应用
	IT0=1;//跳变沿触发方式（下降沿）
	EX0=1;//打开INT1的中断允许
    lcd1602_init();
    app_pool_init();
	while(1)
	{
        //切换应用
        if(app_num==APP_NUM){
            lcd1602_clear();
            LCD1602_V0=0;
            delay_ms(100);
        }
        else{
            lcd1602_clear();
            LCD1602_V0=!LCD1602_V0;
            app_pool[app_num].ctrlc=1;
            app_pool[app_num].app9();
        } 
	}
}
//外部中断0用来切换应用
void exti0() interrupt 0 
{
    //切换应用
    if(app_num<APP_NUM)
    {
        app_pool[app_num].ctrlc=0;
        app_pool[app_num].exit9();
        //退出可以在这里实现，但进入不行，否则将难以被打断
        app_num++;
    }else app_num=0;
    //app_num=APP_NUM表示停止
}

void timer1() interrupt 3
{
    timer1_func();
}
void timer0() interrupt 1//定时器0中断函数
{
    timer0_func();
}
