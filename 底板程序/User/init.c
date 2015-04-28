#include "init.h"
#include "System_init.h"


u8 flag =0;

void system_init(void)
{
  	 Stm32_Clock_Init(9);			//ϵͳʱ������Ϊ�ⲿ����9��Ƶ
	 delay_init(72);				//ϵͳSysTick��ʼ��
	 Usart_Configuration();    		//���ڳ�ʼ��
	 TIM1_Configuration();			//��ʱ��һ��ʼ��
	 GPIO_Configuration();		    //gpio��ʼ��
	 Timer3_configuration();		//��ʱ������ʼ��
	 NVIC_Configuration();			//�ж�����ʹ�ܳ�ʼ��
	 I2C_Congiguration();			//IIC��ʼ��
	 MPU6050_Init();				//������ ���ٶ�  ��ʼ��
	 TIM_Init();					//��ʱ����������׽��ʼ��
	 //��PWM��ʼ����ֵ
	 TIM3->CCR2 = 3600;	//�����������Ĵ��� TIM3->CCR2 = 40;	//�����������Ĵ���
     TIM3->CCR1 = 3600;	//�����������Ĵ��� TIM3->CCR2 = 40;	//�����������Ĵ���
     TIM3->CCR3 = 3600;
	 TIM3->CCR4 = 3600;
	 	
}

void TIM2_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //����һ����ʱ���ṹ�����

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��2
  
  TIM_TimeBaseStructure.TIM_Period = (10000 - 1);	  //����100�Σ���Ϊ��0��ʼ�����Լ�1
  TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);	  //ʱ��72��Ƶ����Ϊ0����Ƶ�����Լ�1
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	      // ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	  //��ʼ����ʱ��2

  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		  //�����ʱ��2�жϱ�־λ
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		  //�򿪶�ʱ��2�ж�

  TIM_Cmd(TIM2, ENABLE);                              //������ʹ�ܣ���ʼ����
}


void NVIC_Configuration(void)
{ 
   NVIC_InitTypeDef NVIC_InitStructure;
  
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/********************ע�ᶨʱ��2�жϴ�����***********************/
 
   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;

   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

/******************ע�ᴮ��1�жϷ�����************************/
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//���ô����ж�
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ��ʽ���ȼ�����Ϊ0
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�����Ϊ0
   //NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;//�жϽ�ֹ
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�жϿ���
   NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
}

//����ԭ������								        
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
                        |RCC_APB2Periph_GPIOB
						|RCC_APB2Periph_GPIOC
						|RCC_APB2Periph_GPIOD,
						ENABLE);
 //****** TIM3 CH1  ch2 (PA6   PA7) ******************************
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;       //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //****** TIM3 CH3,4  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;       //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8| GPIO_Pin_9;	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;       //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  //*******************Ѱ������GPIO	 PC0
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //*******************Ѱ������GPIO  PC0
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //���뿪�� PD2
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;//��������ģʽ 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void Timer3_configuration(void)
{
  	//TIM_DeInit(TIM1);
  	TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure;
  	TIM_OCInitTypeDef TIM_OCInitStructure;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
 
  	TIM1_TimeBaseStructure.TIM_Period = 7200-1;                              // 0xFFFF; ������ֵ        
  	TIM1_TimeBaseStructure.TIM_Prescaler = 200;                          //0xF;   ��Ƶ
  	TIM1_TimeBaseStructure.TIM_ClockDivision = 0x0;
  	TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        
  	TIM_TimeBaseInit(TIM3, &TIM1_TimeBaseStructure);                      // ��������ʱ��Ƶ��36MHz,����=36M/(36000-1+1)/1000
  
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = 3600;                                    //��������504   504/7119=7%,�Ƕ���м�λ��
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	//��ʼ��ͨ��1-PA6
  	TIM_OC1Init(TIM3,&TIM_OCInitStructure);                     
  	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
 	//��ʼ��ͨ��2-PA7
  	TIM_OC2Init(TIM3,&TIM_OCInitStructure);                     
  	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  	//��ʼ��ͨ��3-PB0
  	TIM_OC3Init(TIM3,&TIM_OCInitStructure);                     
  	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
  	//��ʼ��ͨ��4-PB1
  	TIM_OC4Init(TIM3,&TIM_OCInitStructure);                     
  	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
  	TIM_ARRPreloadConfig(TIM3, ENABLE);                                        
  	//ʹ�ܶ�ʱ��3
  	TIM_Cmd(TIM3,ENABLE);
  
  	TIM_ARRPreloadConfig(TIM3, ENABLE);                                       
  	TIM_Cmd(TIM3,ENABLE);

}

void TIM1_Configuration(void)
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//����һ����ʱ���ṹ�����
  	//TIM_TimeBaseInitTypeDef  TIM_BaseInitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  	TIM_TimeBaseStructure.TIM_Period = (10000 - 1);	//����100�Σ���Ϊ��0��ʼ�����Լ�1
  	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);	//ʱ��72��Ƶ����Ϊ0����Ƶ�����Լ�1
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
  	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
  	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	//��ʼ����ʱ��1  
  	//���жϣ�����һ�����жϺ����������ж�
  	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
  	//ʹ��TIM1�ж�Դ
  	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  	//TIM1�ܿ��أ�����
 	TIM_Cmd(TIM1, ENABLE);
}

//��ʱ��1�ж�
void TIM1_UP_IRQHandler(void)
{
 	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
 	{
   		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update); //���ж�

  		if ( 0==flag )
		{ 
	 		flag=1;  
		}  
 	}
}


