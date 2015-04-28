#include"includes.h"
/*
*******************************************************************************
*
*
*
*
*
*******************************************************************************
*/
//�������� ����,�������� GPIO ��GPIO_Pin
void IO_Input_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	/*ͨ��GPIO�ṹ������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin;
	/*����ʱ��*/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);		
	GPIO_Init(GPIOx,&GPIO_InitStructure);

}
//������� ����,�������� GPIO ��GPIO_Pin
void IO_Output_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	/*ͨ��GPIO�ṹ������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin;
	/*����ʱ��*/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);		
	GPIO_Init(GPIOx,&GPIO_InitStructure);

}
