#include "delay.h"
#include "usart.h"
#include "bsp_WaveMaker.h"


void rect(){
	/* 方波 */
	g_Rect.Frequency = 2000;  //10k
	g_Rect.Amplitude = 4095;   //2000
	g_Rect.DutyCycle = 50;     //50
	InitWaveMaker();           
	SetupWaveType(WT_RECT,g_Rect.Frequency,g_Rect.Amplitude,g_Rect.DutyCycle);  
}

void sine(){
	/* 正弦波 */
	g_Sine.Frequency = 2000;
	g_Sine.Amplitude = 2048;
	InitWaveMaker();
	SetupWaveType(WT_SIN, g_Sine.Frequency, g_Sine.Amplitude, 0);
}

void saw(){
	/* 锯齿波 */
	g_Saw.Frequency = 2000;
	g_Saw.Amplitude = 40;
	InitWaveMaker();
	SetupWaveType(WT_SAW, g_Saw.Frequency, g_Saw.Amplitude, 0);
}

void tria(){
	/* 三角波 */
	g_Tria.Frequency = 2000;
	g_Tria.Amplitude = 80;
	InitWaveMaker();
	SetupWaveType(WT_TRIA, g_Tria.Frequency, g_Tria.Amplitude, 0);
}

void noise(){
	/* 噪声波 */
	g_Noise.Frequency = 200;
	g_Noise.Amplitude = 4095;
	InitWaveMaker();
	SetupWaveType(WT_NOISE, g_Noise.Frequency, g_Noise.Amplitude, 0);
}

int main(void)
{		
	int i = 2;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	switch(i){
		case 1: 
			rect();
			break;
		case 2:
			sine();
			break;
		case 3:
			saw();
			break;
		case 4:
			tria();
			break;
		case 5:
			noise();
			break;
		default:
			sine();
			break;
	}
}
