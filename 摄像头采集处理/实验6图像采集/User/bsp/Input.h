#ifndef __INPUT_H__
#define __INPUT_H__
//�������� ����,�������� GPIO ��GPIO_Pin
void IO_Input_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
//������� ����,�������� GPIO ��GPIO_Pin
void IO_Output_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif
