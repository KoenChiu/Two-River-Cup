#include "System_init.h"
#include "math.h"
/********************************************************************
                             ��������
********************************************************************/
extern int PWM_kongzhi;
extern int zhuanwan_pwm;

#define FILTER_COUNT  16
float g_fAngle = 0;                   // �˲�֮��ĽǶ�   
float g_fAngle_Dot = 0;                 // �˲�֮��Ľ��ٶ�
						 
s16 g_nStandCtrOut = 0;              // ֱ��PWM���

s16 gx_buf[FILTER_COUNT], ax_buf[FILTER_COUNT], ay_buf[FILTER_COUNT],az_buf[FILTER_COUNT],gy_buf[FILTER_COUNT];

s16 Accel_x,Accel_y,Accel_z,iGyro_y;
float Angle,Gyro_y,Angle_ax,Angle_gy,Gyro;

#define DEAD 30
int speedl;
int speedr;
float kp = 0.0;        //360  200  210  265
//float kd = 14.5;	 

float jiaodu_PWM;
float speed_right,speed_left,RotateError,IRotateError,RotateCompen;
/*********�ٶȱջ��������************/
float fSpeed_Vechile; 
float fSpeed_Vechile_F;
double fPosition;
float sudu_pwm;
/********************************************************************
                  ���ռ�ձ�������ⲿ���ã�
********************************************************************/
int speedL=0;
int speedR=0;
void SpeedL(int SpeedL)
{
			if (SpeedL>3590) SpeedL=3590;
			if (SpeedL<-3590) SpeedL=-3590;

			TIM3->CCR1 = 3600+SpeedL;
			TIM3->CCR2 = 3600-SpeedL;
}

void SpeedR(int SpeedR)
{
			if (SpeedR>3590) SpeedR=3590;
			if (SpeedR<-3590) SpeedR=-3590;

			TIM3->CCR3 = 3600+SpeedR;
			TIM3->CCR4 = 3600-SpeedR;
}
/********************************************************************
                  ���ٶȼ������˲����ڲ����ã�
********************************************************************/
void acc_filter(void)
{
  u8 i;
  s32 ax_sum = 0, ay_sum = 0, az_sum = 0;

  for(i = 1 ; i < FILTER_COUNT; i++)
  {
    ax_buf[i - 1] = ax_buf[i];
	ay_buf[i - 1] = ay_buf[i];
	az_buf[i - 1] = az_buf[i];
  }

  ax_buf[FILTER_COUNT - 1] = Accel_x;
  ay_buf[FILTER_COUNT - 1] = Accel_y;
  az_buf[FILTER_COUNT - 1] = Accel_z;

  for(i = 0 ; i < FILTER_COUNT; i++)
  {											  
    ax_sum += ax_buf[i];
	ay_sum += ay_buf[i];
	az_sum += az_buf[i];
  }
  Accel_x = (s16)(ax_sum / FILTER_COUNT);
  Accel_y = (s16)(ay_sum / FILTER_COUNT);
  Accel_z = (s16)(az_sum / FILTER_COUNT);
}

/********************************************************************
                  �����Ǽ��ٶȲɼ��������ⲿ���ã�
********************************************************************/
void Angle_Calculate(void)
{
	static	uint8_t DataBuffer[2];	   //���ݻ���
	static  uint8_t x_DataBuffer[2]; 
	static  uint8_t y_DataBuffer[2]; 
	static  uint8_t z_DataBuffer[2]; 

    /****************************���ٶ�Y�� ���ٶȼ�*****************************/
	I2C_ReadBuffer(x_DataBuffer, ACCEL_XOUT_H, 2);
	I2C_ReadBuffer(y_DataBuffer, ACCEL_YOUT_H, 2);
	I2C_ReadBuffer(z_DataBuffer, ACCEL_ZOUT_H, 2);
	/****************************���ٶ������� x �� ********************************/
	I2C_ReadBuffer(DataBuffer, GYRO_XOUT_H, 2);
	//	 
	Accel_x  = (short)((x_DataBuffer[0]<<8)+ x_DataBuffer[1]);	  //��ȡx����ٶ�
    Accel_y  = (short)((y_DataBuffer[0]<<8)+ y_DataBuffer[1]);	  //��ȡy����ٶ�	
	Accel_z  = (short)((z_DataBuffer[0]<<8)+ z_DataBuffer[1]);	  //��ȡz����ٶ�	
	//
	iGyro_y  = (short)((DataBuffer[0]<<8)+DataBuffer[1]);    	 
	//�˲�
	acc_filter();
    //���ü��ٶȼƼ������
	Angle_gy = (float)(Accel_y);
		
	Angle_ax = ((float)(acos((Accel_y) / sqrt(Accel_x * Accel_x + Accel_y * Accel_y + Accel_z*Accel_z))));		
	Angle = (90.0-((Angle_ax )*(180.0)/3.14));     //����ת��Ϊ��,

   	//�����ǻ������	  
	Gyro = (float)(iGyro_y); 
	Gyro_y = (((float)(iGyro_y+29.0)/16.4));  	
	Kalman_Filter(Angle,Gyro_y);
		 														  		
}

/********************************************************************
                     �ٶȱջ�+λ�ñջ��������ڲ����ã�
********************************************************************/
float sudu_p = 0.0;   //
float weizhi_p = 0.0;		 //0.04

void pid_sudu(float p,float i,float d)
{			
	speed_right = Get_Speed_R();
	speed_left = Get_Speed_L();	
	fSpeed_Vechile = ((speed_right)+(speed_left))*0.5;
	fSpeed_Vechile_F = fSpeed_Vechile_F * 0.7 + fSpeed_Vechile * 0.3;
	/*�ۼ���λ��*/
	fPosition += fSpeed_Vechile_F;
	/*λ�Ƶ�����*/
	fPosition -= PWM_kongzhi;
	/*λ������,�����޴�����*/
	if(fPosition > 50000.0)
	{																//5000		
		fPosition = 50000.0;
	}
    else if(fPosition < -50000.0)
    {		
		fPosition = -50000.0;
    }
    RotateError   = speed_left - speed_right;
    IRotateError  += RotateError;
    RotateCompen  =  RotateError * 0.5 + IRotateError * 0.0;

    speed_right =0;		
    speed_left =0;
      
    if((40.0>=Angle)&&(-40.0<=Angle))
    {
    kp =p+ Fuzzy(g_fAngle,g_fAngle_Dot);
    jiaodu_PWM = kp * (g_fAngle) + d * g_fAngle_Dot;
    sudu_pwm = sudu_p * fSpeed_Vechile_F + weizhi_p * fPosition + RotateCompen;
   }
   else
   {
    jiaodu_PWM =0;
	sudu_pwm = 0;
   }   
}


/********************************************************************
                     ����ۼӺ������ڲ����ã�
********************************************************************/
void PWM_out(void)
{

   int pwm_out;
   int left_pwm_out=0,right_pwm_out=0;
   pwm_out = (int)(-jiaodu_PWM - sudu_pwm);
   left_pwm_out = pwm_out + zhuanwan_pwm;
   right_pwm_out = pwm_out - zhuanwan_pwm;

   if (left_pwm_out>0)	
	left_pwm_out+=DEAD;
   else 
	left_pwm_out-=(DEAD);

   if (right_pwm_out>0) 
	right_pwm_out+=DEAD;
   else 		   
   	right_pwm_out-=(DEAD);

    SpeedL(left_pwm_out);
    SpeedR(right_pwm_out);
  	
}

/********************************************************************
                     �������˲��������ڲ����ã�
********************************************************************/

void Kalman_Filter(float Angle_Kal,float Gyro_Kal)
{
     float Q_angle=0.003; 
     float Q_gyro=0.001;
     float R_angle=0.01;
     float dt=0.01;//10ms    
     static float P[2][2] = {	
                                { 1, 0 },
                                { 0, 1 } 
                              };     
     static float q_bias=0; 
     static float angle_err=0;
     static float PCt_0=0;
     static float PCt_1=0;
     static float K_1=0;
     static float t_0=0;
     static float t_1=0;
     
     static float E=0; 
     static float K_0=0;
     
     static float Pdot[4] ={0,0,0,0};
     
     static const char C_0 = 1;
     
     g_fAngle += (Gyro_Kal - q_bias) * dt;                  //�������
    
     Pdot[0] = Q_angle - P[0][1] - P[1][0];              // Pk-' ����������Э�����΢��
     Pdot[1] =- P[1][1];
     Pdot[2] =- P[1][1];
     Pdot[3] = Q_gyro;
     
     P[0][0] += Pdot[0] * dt;                        // Pk- ����������Э����΢�ֵĻ��� = ����������Э����
     P[0][1] += Pdot[1] * dt;
     P[1][0] += Pdot[2] * dt;
     P[1][1] += Pdot[3] * dt;
     
     angle_err = Angle_Kal - g_fAngle;                  //zk-�������
     
     PCt_0 = C_0 * P[0][0];
     PCt_1 =C_0 *  P[1][0];
     
     E =  R_angle+ C_0 *PCt_0;
     K_0 = PCt_0 / E;               //Kk
     K_1 = PCt_1 / E;
    
     t_0 = PCt_0;
     t_1 = P[0][1];                 //C_0 * 	
     
     P[0][0] -= K_0 * t_0;          //����������Э����
     P[0][1] -= K_0 * t_1;
     P[1][0] -= K_1 * t_0;
     P[1][1] -= K_1 * t_1;
    
     g_fAngle  += K_0 * angle_err;         //�������
    
     q_bias += K_1 * angle_err;         //�������
    
     g_fAngle_Dot = Gyro_Kal - q_bias;     //���ֵ��������ƣ���΢�� = ���ٶ�
}

void upright_Adjust(float p,float i,float d)
{
  Angle_Calculate();//���ٶ� ������
  pid_sudu(p,i,d);
  PWM_out(); 
   
}
