#include "jiaodu.h"
#include "System_init.h"


/********************************************************************
                             变量定义
********************************************************************/
#define FILTER_COUNT  16

extern int PWM_kongzhi;
extern int zhuanwan_pwm;

float g_fAngle = 0;                   // 滤波之后的角度   
float g_fAngle_Dot = 0;                 // 滤波之后的角速度
						 
s16 g_nStandCtrOut = 0;              // 直立PWM输出
s16 gx_buf[FILTER_COUNT], ax_buf[FILTER_COUNT], ay_buf[FILTER_COUNT],az_buf[FILTER_COUNT],gy_buf[FILTER_COUNT];
s16 Accel_x,Accel_y,Accel_z,iGyro_y;
float Angle,Gyro_y,Angle_ax,Angle_gy,Gyro;

s16 speed_buf[FILTER_COUNT]={0};


#define DEAD 30
int speedl;
int speedr;
float kp = 0.0;        //360  200  210  265 

float jiaodu_PWM;
float speed_right,speed_left,RotateError,IRotateError,RotateCompen;
/*********速度闭环所需参数************/
float fSpeed_Vechile; 
float fSpeed_Vechile_F=0;
double fPosition;
float sudu_pwm;

/*************************************************
名称：void speed_filter(void)
功能：速度滤波
输入参数：无
输出参数：无
返回值：  无
**************************************************/
float speed_filter(float right_seed,float left_speed)
{
  	u8 i;
  	s32 speed_sum = 0; 
  	float speed_out = 0;

  	for(i = 1 ; i < FILTER_COUNT; i++) //数据向左移一位
  	{	
    	speed_buf[i - 1] = speed_buf[i];
  	}

  	speed_buf[FILTER_COUNT - 1] = ((right_seed + left_speed) / 2); //最后一位更新数据，左右轮的平均速度

  	for(i = 0 ; i < FILTER_COUNT; i++)  //求速度总和
  	{
    	speed_sum += speed_buf[i];
  	}
   
  	speed_out = (float)(speed_sum / FILTER_COUNT); //求速度总和的平均值
  	return  speed_out;
}


/********************************************************************
                  电机占空比输出（外部调用）
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
                  加速度计数据滤波（内部调用）
********************************************************************/
void acc_filter(void)
{
  	u8 i;
  	s32 ax_sum = 0, ay_sum = 0, az_sum = 0;

  	for(i = 1 ; i < FILTER_COUNT; i++)  //全向左移一位
  	{
    	ax_buf[i - 1] = ax_buf[i];
		ay_buf[i - 1] = ay_buf[i];
		az_buf[i - 1] = az_buf[i];
  	}

  	ax_buf[FILTER_COUNT - 1] = Accel_x;  //更新数据
  	ay_buf[FILTER_COUNT - 1] = Accel_y;  //更新数据
  	az_buf[FILTER_COUNT - 1] = Accel_z;  //更新数据

  	for(i = 0 ; i < FILTER_COUNT; i++)//求总和
  	{											  
    	ax_sum += ax_buf[i];
		ay_sum += ay_buf[i];
		az_sum += az_buf[i];
  	}
  	Accel_x = (s16)(ax_sum / FILTER_COUNT);//求平均
  	Accel_y = (s16)(ay_sum / FILTER_COUNT);
  	Accel_z = (s16)(az_sum / FILTER_COUNT);
}

/********************************************************************
                  陀螺仪加速度采集函数（外部调用）
********************************************************************/
void Angle_Calculate(void)
{

	static	uint8_t DataBuffer[2];	   //数据缓存
	static  uint8_t x_DataBuffer[2];   //X轴方向数据缓存
	static  uint8_t y_DataBuffer[2];   //Y轴方向数据缓存
	static  uint8_t z_DataBuffer[2];   //Z轴方向数据缓存

    /****************************加速度Y轴 加速度计*****************************/
	I2C_ReadBuffer(x_DataBuffer, ACCEL_XOUT_H, 2);
	I2C_ReadBuffer(y_DataBuffer, ACCEL_YOUT_H, 2);
	I2C_ReadBuffer(z_DataBuffer, ACCEL_ZOUT_H, 2);
	/****************************角速度陀螺仪 x 轴 ********************************/
	I2C_ReadBuffer(DataBuffer, GYRO_XOUT_H, 2);
	//	 
	Accel_x  = (short)((x_DataBuffer[0]<<8)+ x_DataBuffer[1]);	  //读取x轴加速度
    Accel_y  = (short)((y_DataBuffer[0]<<8)+ y_DataBuffer[1]);	  //读取y轴加速度	
	Accel_z  = (short)((z_DataBuffer[0]<<8)+ z_DataBuffer[1]);	  //读取z轴加速度	
	//
	iGyro_y  = (short)((DataBuffer[0]<<8)+DataBuffer[1]);		//读取x轴陀螺仪数据    	 
	//滤波
	acc_filter();
    //利用加速度计计算倾角
	Angle_gy = (float)(Accel_y);
		
	Angle_ax = ((float)(acos((Accel_y) / sqrt(Accel_x * Accel_x + Accel_y * Accel_y + Accel_z*Accel_z))));		
	Angle = (90.0-((Angle_ax )*(180.0)/3.14));     //弧度转换为度,

   	//陀螺仪积分倾角	  
	Gyro = (float)(iGyro_y); 
	Gyro_y = (((float)(iGyro_y+29.0)/16.4));  	
	Kalman_Filter(Angle,Gyro_y);		 														  		
}
/********************************************************************
                     速度闭环+位置闭环函数（内部调用）
********************************************************************/
float sudu_p = 4.5;          //4.1
float weizhi_p = 0.05;		 //0.02

void pid_sudu(float p,float i,float d)
{			
	speed_right = Get_Speed_R();
	speed_left = Get_Speed_L();	 

	fSpeed_Vechile = (speed_right+speed_left) * 0.5;
	fSpeed_Vechile_F = fSpeed_Vechile_F * 0.7 + fSpeed_Vechile * 0.3;//滤波
	/*累加求位移*/
	fPosition += fSpeed_Vechile_F; //对速度进行积分求得位移
	/*位移调节量*/
	fPosition -= PWM_kongzhi;
	/*位移限制,上下限待调节*/
	/*减少小车悬空、空转对小车的影响*/
	if(fPosition > 50000.0)                  //5000	
	{																	
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
      
    if((40.0>=Angle)&&(-40.0<=Angle))//在Angle在[-40.0，40.0]区间内调节
    {
    	kp =p+ Fuzzy(g_fAngle,g_fAngle_Dot);  //模糊算法，不建议使用
    	jiaodu_PWM = kp * (g_fAngle+1.5) + d * g_fAngle_Dot;
    	sudu_pwm = sudu_p * fSpeed_Vechile_F + weizhi_p * fPosition + RotateCompen;
   	}
   	else
   	{
    	jiaodu_PWM =0;
		sudu_pwm = 0;
   	}   
}
/********************************************************************
                     电机累加函数（内部调用）
********************************************************************/
void PWM_out(void)
{
   	int pwm_out;
   	int left_pwm_out=0,right_pwm_out=0;
   	pwm_out = (int)(jiaodu_PWM - sudu_pwm);
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
	//SpeedL(3500);
	//SpeedR(3500);
  	
}		  

/********************************************************************
                     卡尔曼滤波函数（内部调用）
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
     
     g_fAngle += (Gyro_Kal - q_bias) * dt;                  //先验估计
    
     Pdot[0] = Q_angle - P[0][1] - P[1][0];              // Pk-' 先验估计误差协方差的微分
     Pdot[1] =- P[1][1];
     Pdot[2] =- P[1][1];
     Pdot[3] = Q_gyro;
     
     P[0][0] += Pdot[0] * dt;                        // Pk- 先验估计误差协方差微分的积分 = 先验估计误差协方差
     P[0][1] += Pdot[1] * dt;
     P[1][0] += Pdot[2] * dt;
     P[1][1] += Pdot[3] * dt;
     
     angle_err = Angle_Kal - g_fAngle;                  //zk-先验估计
     
     PCt_0 = C_0 * P[0][0];
     PCt_1 =C_0 *  P[1][0];
     
     E =  R_angle+ C_0 *PCt_0;
     K_0 = PCt_0 / E;               //Kk
     K_1 = PCt_1 / E;
    
     t_0 = PCt_0;
     t_1 = P[0][1];                 //C_0 * 	
     
     P[0][0] -= K_0 * t_0;          //后验估计误差协方差
     P[0][1] -= K_0 * t_1;
     P[1][0] -= K_1 * t_0;
     P[1][1] -= K_1 * t_1;
    
     g_fAngle  += K_0 * angle_err;         //后验估计
    
     q_bias += K_1 * angle_err;         //后验估计
    
     g_fAngle_Dot = Gyro_Kal - q_bias;     //输出值（后验估计）的微分 = 角速度
}

void upright_Adjust(float p,float i,float d)
{
  	Angle_Calculate();//角速度 陀螺仪
  	pid_sudu(p,i,d);
  	PWM_out(); 
   
}
