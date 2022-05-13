#include "public.h"
#include "lcd1602.h"
//#include "freq-duty.c"

sbit LCD1602_V0=P2^4;//背光pwm

//定义按键
sbit CHOICE=P1^1;
sbit STOP=P1^2;

sbit Tdt=P3^5;

u8 show_opt=3;
long Freq=121212;
long freq_s;
u16 Duty=0;
u8 start=1;
long up=0,total=0;

//显示占空比
void prtduty(u16 duty)
{
	static u16 dutyt=0;
	static u8 show[5]={'0','0','.','0','0'};
	//显示占空比
	if(dutyt==duty)return;
	show[0]=duty/1000+'0';
	show[1]=duty/100%10+'0';
	show[3]=duty/10%10+'0';
	show[4]=duty%10+'0';
	dutyt=duty;
	lcd1602_show_string(7,1,show);
}
//显示数字
void prtnum(long num) reentrant
{
	static long numc=0;
	static u8 flag,show[6];//记录显示的各个数字，因为乘除法计算很慢，亮度下降
	if(num==numc){
		return;
	}else{
		numc=num;flag=0;
		show[5]='0'+num%10;
		num/=10;if(!num)flag=1;
		show[4]=flag?' ':num%10+'0';
		num/=10;if(!flag&&!num)flag=1;
		show[3]=flag?' ':num%10+'0';
		num/=10;if(!flag&&!num)flag=1;
		show[2]=flag?' ':num%10+'0';
		num/=10;if(!flag&&!num)flag=1;
		show[1]=flag?' ':num%10+'0';
		num/=10;if(!flag&&!num)flag=1;
		show[0]=flag?' ':num%10+'0';
	}
	lcd1602_show_string(5,0,show);
}
/*
void exti1_init(void)
{
	IT1=1;//跳变沿触发方式（下降沿）
	EX1=1;//打开INT1的中断允许
	EA=1;//打开总中断
}*/
void time0_init_fd(void)
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1
	TH0=0XF2;	
	TL0=0XFA;	
	ET0=1;//打开定时器0中断允许
	TR0=0;//暂停定时器		
}
void time1_init_fd(void)
{
	TMOD|=0X50;
	TH1=0;
	TL1=0;		
	TR1=0;//打开计数器	
	ET1=0;
}
void main_fd(char *ctrlc)
{	
	bit choice_state=0;
    bit stop_state=0;
	EA=1;//打开总中断
	time1_init_fd();
	time0_init_fd();
	lcd1602_init();//LCD1602初始化
	lcd1602_show_string(0,0,"Freq:      Hz");
	lcd1602_show_string(0,1,"dt:    00.00%");
	
	while(*ctrlc)
	{
		LCD1602_V0=!LCD1602_V0;//背光调节

        if(STOP==0){
			if(stop_state==0){
			delay_10us(100);//消斗
			if(STOP==0){
				show_opt^=1;//是否停止
				freq_s=Freq;
				stop_state=1;
			}}
		}else{
			if(stop_state)stop_state=0;
		}

		if(CHOICE==0){//按键按下
		delay_10us(100);//延时一段时间，等待按键稳定
		if(CHOICE==0){
			if(choice_state==0){//按键状态为0，即按下状态
			}else{//按键状态为1，即松开状态
				choice_state=0;
				show_opt^=2;//频率、占空比
				if(show_opt==0||show_opt==1){//占空比
					ET0=0;//关闭定时器0中断
					TR0=0;//暂停定时器0
					TMOD=0x20;
					TH1=0x80;
					TL1=0X80;
					TR1=1;//开启定时器1
					ET1=1;
					total=0;up=0;
				}else{
					TMOD=0;//初始化
					time1_init_fd();//频率
					time0_init_fd();
					start=1;
				}
			}
		}}else{
			if(choice_state==0){//按键松开
				choice_state=1;
			}
		}
        if(start==1&&show_opt==3||show_opt==2){
			TR0=1;
			TR1=1;
			start=0;
		}
		if(show_opt==2){//显示固定频率
			prtnum(freq_s);
		}else if(show_opt==0){//显示固定占空比
			prtduty(Duty);
		}else if(show_opt==3){//显示实时频率
			prtnum(Freq);
		}else if(show_opt==1){//显示实时占空比
			//更新占空比
			if((total>20000&&Freq>10000)||total>80000){
				ET1=0;//关闭定时器1中断
				Duty=10000*up/total;
				total=0;
				up=0;
				ET1=1;//开启定时器1中断
			}
			prtduty(Duty);
		}
	}	
}
void timer1_fd()
{
	total++;
	up+=(Tdt==1);
}
void timer0_fd()
{
	static int time;
	TH0=0XF2;	//给定时器赋初值，定时1ms
	TL0=0XFA;
	time++;
	if(time==247&&TH1>50){//0.25s设卡，如果频率过高直接截断，避免T1溢出
		time=0;
		TR0=0;
		TR1=0;
		Freq=(TH1*256+TL1);
		Freq=4*Freq;//-Freq/150;//在freq里乘，因为其是long
		TH1=0;
		TL1=0;
		start=1;
	}else if(time==985){//模拟1s校准值，根据实际情况调整
		time=0;
		TR1=0;    //停止计数
		TR0=0;          //停止定时
		Freq=(TH1*256+TL1);   //求出频率值   就是1秒内脉冲次数
		//Freq+=Freq/3000*(Freq/5000);//修正偏差
		//注意：较高频产生的偏差较为随机，难以修正。46k以下较为准确
		TH1=0;          //计数值清零
		TL1=0;
		start=1;  
	}
}
