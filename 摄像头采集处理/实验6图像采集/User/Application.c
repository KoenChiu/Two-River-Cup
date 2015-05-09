#include"includes.h"

	extern uint8_t PIC[R][C];
/*
------------------------------------------------------------------------
*  �������ƣ�void Application(void)
*  �������ܣ�����ͼ�����ݿɶ�ʱ��ȡ����
*  �����βΣ��� 	
*  ����ֵ����	
*
------------------------------------------------------------------------
*/
void Application(void)
{
	while(1)/*��ѭ���￪����*/
	{ 		
		if(OV_State==Read)// �������ͷ���������Զ�����
		{
			//�ر��ж�
			EXTI_Config(DIS);
			//��ȡͼƬ��Ϣ
			Read_Pic();
			
			//Send_Pic();

			//V_control(PIC);
			PIC_Process();
			//PIC_Process_2();

			//ͨ�����ڷ���ͼƬ��Ϣ
			//Send_Pic();
			//Send_MidLine();
			USART_SendData(USART1,Curve_Value);
			
			//״̬ת��
			OV_State=Wait_Vsync1;
			//�����ж�
			EXTI_Config(EN);
		}
	}
}
/*
-----------------------------------------------------------------------------
*  �������ƣ�void Board_Init(void)
*  �������ܣ����ø���ģ���ʼ���ӿں���
*  �����βΣ���  	
*  ����ֵ����	
*
-----------------------------------------------------------------------------
*/
void Board_Init(void)
{
	/*ѡ���ⲿ8M������Ϊʱ��Դ*/
	SystemInit();
	/*��ʱ��ʼ��*/
	delay_init(72);
	/*LED��ʼ��*/
	LED_Init();
	/*���ڳ�ʼ��*/
	USART_Config();
	NVIC_Config();
	/*OV7670ģ���ʼ��*/	 	
	OV7670_Init();
}
