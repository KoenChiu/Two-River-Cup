#include"includes.h"
//������ʼ������
void KEY_Init(void)
{
	/*ͨ��GPIO�ṹ������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*����KEYʱ��*/
	RCC_APB2PeriphClockCmd(KEY_1_Clock,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_2_Clock,ENABLE);
	/*�����������û���ⲿ��������Ļ��ͱ��뽫�������Ƴ������������ʽ */	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	/*KEY_1���ƹܽź�����*/
	GPIO_InitStructure.GPIO_Pin=KEY_1_Pin;
	/*�������ó�ʼ���ܽŷ���*/	
	GPIO_Init(KEY_1_Port,&GPIO_InitStructure);
	/*KEY_2���ƹܽź�����*/
	GPIO_InitStructure.GPIO_Pin=KEY_2_Pin;
	/*�������ó�ʼ���ܽŷ���*/	
	GPIO_Init(KEY_2_Port,&GPIO_InitStructure);

}

