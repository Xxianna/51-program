#  51-freq&duty-multitask

This is a C51 project that can be compiled with keil4 and can run on stc89c52rc.

It can measure the square wave signal of 1Hz ~ 150Hz (the tried range). Of course, when using, the signal source should be in common with the single chip microcomputer. The signal amplitude of 5V is available. I don't know other situations.

It can switch between multiple applications. The number of applications can be set in [main. C]. External interrupt is applied to the switching condition, and one pulse is given to ext0 to switch.

Here, if clock switching is used, it can be equivalent to multi process operation, but there are requirements for the function itself as an application program, that is, it can run step by step in a loop and save and load the state it runs to. This is similar to the operation mode of some simple operating systems I searched.

Frequency measurement is one of the applications, which will occupy timers t0 and T1, and the measurement pin is T1. The switching stop indication or continuous measurement is p12, and the switching frequency or duty cycle measurement is P11.

The output of this project is realized through LCD1602. The interface can be seen in [LCD 1602. H]. Note that the V0 pin of LCD1602 needs to set the backlight, and some resistors can be used to adjust its voltage. In this project, PWM method is adopted in [frequency duty cycle measurement], and p24 value is changed once per cycle, so as to output 2.5V to v0.

Other contents can be seen in the code and comments. This project is annotated in Chinese.

This project is a big assignment for my course. 12Mhz crystal oscillator is used in the course. In order to improve the performance, 40MHz crystal oscillator is used in my program. When in use, you can modify the initial value of interrupt timing in [freq duty. C] and the [delay] function in [public. C].

 这是一个可用keil4编译的C51项目，可在STC89C52RC上运行。

它可以实现对1Hz~150Hz（已经尝试过的范围）的方波信号进行测定。当然，使用时信号来源与单片机应共地，信号幅值为5V是可用的，其他情况我不知道。

它可以实现多应用之间的切换。在[main.c]中可以设置应用个数。切换条件应用了外部中断，给EXT0一个脉冲即可切换。

在这里，如果使用时钟切换、则可相当于多进程运行，但对作为应用程序的函数本身有要求，就是循环分步运行、能保存和加载自己运行到的状态。这一点与我搜索到的一些简单的操作系统的运行方式是类似的。

频率测量为其中一个应用，会占据计时器T0和T1，测量脚为T1。切换停止示数或继续测量为P12，切换频率或占空比测量为P11。

本项目的输出是通过LCD1602实现的。其接口情况可见[lcd1602.h]。注意LCD1602的V0脚需要设置背光情况，可用一些电阻来调节其电压实现。而本项目在[频率占空比测量]中采取了PWM的方法，每个循环周期改变一次P24值，从而将2.5V输出到V0。

其他内容可在代码及注释中看到。本项目为中文注释。

本项目是我所上课程的一个大作业。课程中用了12Mhz晶振，为提升性能，我的程序中使用了40Mhz晶振。使用时可对[freq-duty.c]中的中断定时初值进行修改、对[public.c]中的[delay]函数进行修改。