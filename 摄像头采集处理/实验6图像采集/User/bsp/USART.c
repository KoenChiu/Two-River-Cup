#include"includes.h"
/*

*/
/*���¶���printf����*/
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
	USART_SendData(USART1, (uint8_t) ch);
  	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ��������
  	return ch;
}
void USART_Config(void)
{
	/*������������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ��4��������USART����
	    - ������   = 115200 baud
	    - ���ݳ��� = 8 Bits
	    - 1��ֹͣλ
	    - ��У��
	    - ��ֹӲ������(����ֹRTS��CTS)
	    - ʹ�ܽ��պͷ���
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	/* ��5����ʹ�� USART�� ������� */
	USART_Cmd(USART1, ENABLE);
	/* 
		CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
	 	�巢����ɱ�־��Transmission Complete flag 
	*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ���ж�

	USART_ClearFlag(USART1, USART_FLAG_TC); 	
	//printf("����ͨ������");

}
/*
**************************************************************************************
*�������ƣ�
*�������ܣ�
*�����βΣ�
*����ֵ:
**************************************************************************************
*/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
   	#ifdef VECT_TAB_RAM  
   	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
   	#else  
   	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
   	#endif 
   	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   	NVIC_InitStructure.NVIC_IRQChannel= USART1_IRQn; //����1�ж�����   
   	/*��ռ���ȼ�*/
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/*�����ȼ�*/	     
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 
	/*ʹ��*/ 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //ʹ��
  	NVIC_Init(&NVIC_InitStructure);	 
}
