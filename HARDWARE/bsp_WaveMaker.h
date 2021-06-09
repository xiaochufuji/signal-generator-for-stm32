#ifndef __BSP_WAVEMAKER_H
#define __BSP_WAVEMAKER_H

#include "stm32f10x.h"


#define DAC_DHR12RD_Address     0x40007420
#define DAC_DHR8R1_Address      0x40007410
#define DAC_DHR12R1_Address		0x40007408

/* DMA �������ݳ��� */
enum
{   
  DMA_BufferSize_36   = 36,
	DMA_BufferSize_50   = 50,
	DMA_BufferSize_100  = 100,
};
/* �������� */
enum
{   
  WT_RECT  = 0,	/* ���β� */
	WT_SIN   = 1,	/* ���Ҳ� */
	WT_TRIA	 = 2,	/* ���ǲ� */
	WT_SAW   = 3,   /* ��ݲ� */
	WT_NOISE = 4,   /* ����   */
};
/* ���Ҳ� */
typedef struct
{
  uint32_t Frequency;     //Ƶ��
  uint16_t Amplitude;     //��ֵ
}SIN_T;

/* ���ǲ� */
typedef struct
{
  uint16_t Frequency;     //Ƶ��
  uint16_t Amplitude;     //��ֵ
}TRIA_T;

/* ��ݲ� */
typedef struct
{
  uint32_t Frequency;     //Ƶ��
  uint16_t Amplitude;     //��ֵ
}SAW_T;

/* ���β� */
typedef struct
{
  uint32_t Frequency;     //Ƶ��
  uint16_t Amplitude;     //��ֵ
  uint8_t  DutyCycle;     //ռ�ձ�
}RECT_T;

/* ���� */
typedef struct
{
  uint16_t Frequency;     //Ƶ��
  uint16_t Amplitude;     //��ֵ
}NOISE_T;

extern SIN_T    g_Sine;
extern RECT_T   g_Rect;
extern SAW_T    g_Saw;
extern NOISE_T  g_Noise;
extern TRIA_T   g_Tria;


void InitWaveMaker(void);
void SetupWaveType(uint8_t _ucType,uint32_t _ulFrequency,uint16_t _usAmplitude,	
                    uint8_t _ucDutyCycle);
#endif
