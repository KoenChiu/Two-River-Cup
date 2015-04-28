#include"includes.h"

//�ⲿ�ж�����
void EXTI_Config(uint8_t flag)	
{
	
	NVIC_InitTypeDef NVIC_InitStructure;/*����Ƕ���жϽṹ��*/ 	
  	EXTI_InitTypeDef EXTI_InitStructure;/*�����ж����ýṹ��*/		
	GPIO_InitTypeDef GPIO_InitStructure;/*����ͨ��IO�����ýṹ��*/ 
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);/*����GPIO��AFIOʱ��*/
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	/*ѡ���ж����ȼ���1��*/    
	/*�ⲿ�ж�0 */	
 	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;					
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	/*��ռ���ȼ� 1*/    
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	/*�����ȼ�0*/ 		  
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/*ʹ��*/				 
  	NVIC_Init(&NVIC_InitStructure);	
	/*PA0*/
   	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	      
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	/*�ж�ģʽ*/
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;/*�½��ش���*/	
	if(flag==EN)
	{
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
	} 
	else
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;	
	}
  	 

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//PA0 					  
  	EXTI_Init(&EXTI_InitStructure);


  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4;	         	 	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   	 		//�ڲ���������
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

