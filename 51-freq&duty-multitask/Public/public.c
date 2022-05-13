#include "public.h"
void delay10us(void)   //误差 -0.1us
{
    unsigned char a;
    for(a=14;a>0;a--);
}

void delay_10us(u16 ten_us)
{
	while(ten_us--)delay10us();	
}
void delay1ms(void)   //误差 -0.1us
{
    unsigned char a,b,c;
    for(c=26;c>0;c--)
        for(b=25;b>0;b--)
            for(a=1;a>0;a--);
}
void delay_ms(u16 ms)
{
	while(ms--)delay1ms();
}