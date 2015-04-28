#include "encoder.h" 
#include "System_init.h"

void TIM4_Mode_Config(void)	 //������
{

	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;	
/*----------------------------------------------------------------*/
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  //GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
/*----------------------------------------------------------------*/	
 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
   TIM_DeInit(TIM4);
   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
   TIM_TimeBaseStructure.TIM_Period =0xffff;       //
   TIM_TimeBaseStructure.TIM_Prescaler =0;	    //����Ԥ��Ƶ��
   TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
 

   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/*-----------------------------------------------------------------*/
	//��������                        ����ģʽ
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
    TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  //TIM_ICPolarity_Rising�����ز���
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;         //�Ƚ��˲���

    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
 
    //Reset counter
    TIM4->CNT =0;	
	TIM_Cmd(TIM4, ENABLE);   //ʹ�ܶ�ʱ��4
}
void TIM2_Mode_Config(void)	 //������
{

	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;	
/*----------------------------------------------------------------*/
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  //GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
/*----------------------------------------------------------------*/	
 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
   TIM_DeInit(TIM2);
   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
   TIM_TimeBaseStructure.TIM_Period =0xffff;       //
   TIM_TimeBaseStructure.TIM_Prescaler =0;	    //����Ԥ��Ƶ��
   TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
 

   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/*-----------------------------------------------------------------*/
	//��������                        ����ģʽ
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  //TIM_ICPolarity_Rising�����ز���
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;         //�Ƚ��˲���

    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
 
    //Reset counter
    TIM2->CNT =0;	
	TIM_Cmd(TIM2, ENABLE);   //ʹ�ܶ�ʱ��2
}
void TIM_Init(void)
{
  TIM4_Mode_Config();
  TIM2_Mode_Config();

}



s16 Get_Speed_L()
{
   s16 speed;

     speed =- TIM4->CNT ;
	 TIM4->CNT =0;
       
     return speed;
}
s16 Get_Speed_R()
{
   s16 speed;

     speed = TIM2->CNT ;
	 TIM2->CNT =0;

   return speed;//����
}

