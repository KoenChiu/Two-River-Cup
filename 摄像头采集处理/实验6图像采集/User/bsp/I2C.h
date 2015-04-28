#ifndef __I2C_H__
#define __I2C_H__ 
/*I2C���ƽӿڶ���*/
void I2C_Configuration(void);
/*I2C������������*/ 
void I2C_Start(void);
/*I2C�����ͷź���*/
void I2C_Stop(void);
/*I2C���߼��ӻ�Ӧ����*/
uint8_t I2C_WaitAck(void);
/*I2C����д���ݺ���*/
void I2C_SendByte(uint8_t _ucByte);
/*I2C��ȡ���ݺ���*/
uint8_t I2C_ReadByte(void);
/*I2C����Ӧ��ݺ���*/
void I2C_Ack(void);
/*I2C���߷�Ӧ����*/
void I2C_NAck(void);
/*I2C����д�Ĵ�������*/
int I2C_WriteByte( uint16_t WriteAddress , uint8_t SendByte) ;
/*I2C���߶��Ĵ������ݺ���*/
uint8_t I2C_ReadOneByte(uint8_t Address);
#endif
