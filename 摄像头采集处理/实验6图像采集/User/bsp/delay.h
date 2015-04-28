		
#ifndef __DELAY_H
#define __DELAY_H  
#include"stm32f10x.h"     
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
//����delay_us,delay_ms  
//����ԭ��@SCUT
//2008/12/13  
static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������
//��ʼ���ӳٺ���
void delay_init(u8 SYSCLK); 
//��ʱNms
//ע��Nms�ķ�Χ
//Nms<=0xffffff*8/SYSCLK
//��72M������,Nms<=1864 
void delay_ms(u16 nms);
//��ʱus           
void delay_us(u32 Nus);


#endif

