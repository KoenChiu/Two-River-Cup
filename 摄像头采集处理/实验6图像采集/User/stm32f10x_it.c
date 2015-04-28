/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include"includes.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

//�ⲿ�ж�
void EXTI15_10_IRQHandler(void)
{
	 //�б��Ƿ����½��� ����
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)				  
  	{
			LED_1(ON);
			LED_2(ON);
			//����ж������־
			printf("���� 1 �ж�\n");
    		EXTI_ClearITPendingBit(EXTI_Line10);
							  
  	}
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)				  
  	{
			LED_1(OFF);
			LED_2(OFF);
			//����ж������־
			printf("���� 2 �ж�\n");
    		EXTI_ClearITPendingBit(EXTI_Line11);
							  
  	}

}
//�ж�֮��
void EXTI0_IRQHandler(void)
{
	 //�б��Ƿ����½��� ����
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)				  
  	{
	
		switch(OV_State)
		{
			case Wait_Vsync1://����ͼƬ�Ĵ洢	��һ���ж�
			{	
				//�ر����ź��ж� ��һ���ǳ���Ҫ���ڲ���ִ����֮���ڿ����ж�
				EXTI_Config(DIS);			
				FIFO_WRST_H;
				FIFO_WRST_L;   			
				FIFO_WRST_H;//дָ�븴λ
				FIFO_WR_H;//дʹ�� 
				OV_State=Wait_Vsync2; //״̬ת��
				//�����ж�
				EXTI_Config(EN);				
			} break;

			case Wait_Vsync2://��ֹͼƬ�洢��������ͼƬ����	 �ڶ����ж�
			{
				//�ر����ź��ж� ��һ���ǳ���Ҫ���ڲ���ִ����֮���ڿ����ж�
				EXTI_Config(DIS);
				FIFO_WR_L;//дFIFO��ֹ 	
				FIFO_RRST_L;//��ָ�븴λ 
      			FIFO_RCK_L;//��ʱ��   
      			FIFO_RCK_H;	       		
      			FIFO_RCK_L;      
     			FIFO_RCK_H;
				FIFO_RRST_H;
				delay_ms(1);
				FIFO_OE_L;//����FIFO��� 
				OV_State=Read; 	
				TrueLine=0;	
				//�����ж�
				EXTI_Config(EN);	
			}break;

		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
//�ⲿ�ж�
void EXTI4_IRQHandler(void)
{
	 //�б��Ƿ����½��� ����
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)				  
  	{
			
			//printf("HERF\n");
    		EXTI_ClearITPendingBit(EXTI_Line4);
							  
  	}
}
//����1�жϺ�����ڣ��˺��������start_up_stm32f10x_md.s�ļ���������������һ��
void USART1_IRQHandler(void)
{
	u8 temp;
 	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
 	{
     	temp=USART_ReceiveData(USART1);
		printf("���յ���λ������ %c\n",temp);

	}	
}
