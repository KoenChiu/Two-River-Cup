#include"includes.h"
/*
************************************************************************
*  �������ƣ�
*  �������ܣ�
*  �����βΣ�  	
*  ����ֵ��	
*
************************************************************************
*/
void LED_Init(void)
{
	/*ͨ��GPIO�ṹ������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED_1_Clock,ENABLE);
	RCC_APB2PeriphClockCmd(LED_2_Clock,ENABLE);
	/*�������*/	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	/*�����ٶ�*/ 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	/*LED_1���ƹܽź�����*/
	GPIO_InitStructure.GPIO_Pin=LED_1_Pin;
	/*�������ó�ʼ���ܽŷ���*/	
	GPIO_Init(LED_1_Port,&GPIO_InitStructure);
	/*LED_2���ƹܽź�����*/
	GPIO_InitStructure.GPIO_Pin=LED_2_Pin;
	/*�������ó�ʼ���ܽŷ���*/	
	GPIO_Init(LED_2_Port,&GPIO_InitStructure);
	LED_1(OFF);
	LED_2(OFF);

}











