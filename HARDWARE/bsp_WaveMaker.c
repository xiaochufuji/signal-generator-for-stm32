#include "stm32f10x.h"
#include <stdio.h>
#include "stdlib.h"
#include "bsp_WaveMaker.h"
#include "math.h"

SIN_T    g_Sine;	     /*���Ҳ�*/
RECT_T   g_Rect;	     /*���β�*/
SAW_T    g_Saw;		     /*��ݲ�*/
NOISE_T  g_Noise;	     /*����  */
TRIA_T   g_Tria;	     /*���ǲ�*/

uint16_t g_Wave[100];

/*******************************************************************************
*	������: InitWaveMaker
*	��  ��:	��
*	��  ��:	��
*	����˵������ʼ�����η�������
*******************************************************************************/
void InitWaveMaker(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

		/*��ʼ��DAC ͨ��1 */	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;                  					 
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	    
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	
	 /* ����PA4ΪDAC_OUT1  */		
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
/*
*********************************************************************************************************
*	�� �� ��: InitWaveFreq(uint32_t _usFrequency, uint8_t _ucDMABuffuerSize)
*	����˵��: ���ڳ�ʼ�����ֲ��εĲ���
*	��    �Σ�_usFrequency     ��ʾҪ�������εĴ���Ƶ�ʣ�
*			 			_ucDMABuffuerSize ��ʾDMAҪ�������������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitWaveFreq(uint32_t _usFrequency, uint8_t _ucDMABuffuerSize)
{
	DMA_InitTypeDef DMA_InitStructure;

	/*��ʼ����ʱ��*/	   														  
	TIM_PrescalerConfig(TIM6, 0, TIM_PSCReloadMode_Update);  
	TIM_SetAutoreload(TIM6,  _usFrequency);	  
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	
	/*��ʼ��DMA */	
	DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_Wave;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						     
	DMA_InitStructure.DMA_BufferSize = _ucDMABuffuerSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						    
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low ;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	DMA_Cmd(DMA2_Channel3, ENABLE);			
}
/*
*********************************************************************************************************
*	�� �� ��: SetupWaveType
*	����˵��: ���ڽ����������ֲ���
*	��    �Σ�_ucType		��ʾҪ�����Ĳ��ε�����
						_ulFrequency	��ʾ���εĴ���Ƶ��
						_usAmplitude	��ʾ���εķ�ֵ
						_ucDutyCycle	��ʾ���β���ռ�ձ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SetupWaveType(uint8_t _ucType,uint32_t _ulFrequency,uint16_t _usAmplitude,	
                    uint8_t _ucDutyCycle)
{
  uint8_t i;

  TIM_Cmd(TIM6, DISABLE); 	
	DAC_Cmd(DAC_Channel_1, DISABLE);		 		 	
	switch (_ucType)
	{
		case WT_RECT:					      /* ���� */
				InitWaveFreq(720000 / _ulFrequency, DMA_BufferSize_100);  /*Ҫ�ȳ�ʼ�����*/
				for (i = 0; i < _ucDutyCycle; i++)
				{// ����ʱ����
					g_Wave[i] = _usAmplitude;
				}
				for (i = _ucDutyCycle; i < 100; i++)
				{
					g_Wave[i] = 0;
				}
				break; 
		case WT_SIN:						/* ���Ҳ� */
		    InitWaveFreq(720000 / _ulFrequency,DMA_BufferSize_100);			
				for (i = 0; i < DMA_BufferSize_100; i++)
				{
					g_Wave[i] = 2047+_usAmplitude*sin(2*3.1415926f*99*i/100); 
				}
				break;
		case WT_SAW:					    /* ��ݲ� */
		   	InitWaveFreq(720000 / _ulFrequency,DMA_BufferSize_100);			
				for (i = 0; i < DMA_BufferSize_100; i++)
				{
					g_Wave[i] = i*_usAmplitude; 
				}
				break;
		case WT_TRIA:					     /* ���ǲ�*/
				InitWaveFreq(720000 / _ulFrequency,DMA_BufferSize_100);			
				for (i = 0; i < DMA_BufferSize_100; i++)
				{
					g_Wave[i] = abs(50-i)*_usAmplitude; 
				}
				break;
		case WT_NOISE:					     /*���� */
				InitWaveFreq(720000 / _ulFrequency,DMA_BufferSize_100);			
				for (i = 0; i < DMA_BufferSize_100; i++)
				{
					g_Wave[i] = abs(rand()%_usAmplitude); 
				}
				break;  
		default : break;
	}
	DAC_Cmd(DAC_Channel_1, ENABLE);		 	 
	TIM_Cmd(TIM6, ENABLE);
}








