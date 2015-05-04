#include"includes.h"

uint8_t PIC[R][C];
int Mid_Line[R]={0};
uint8_t PIC_Smooth[R][C]={0};
uint8_t PIC_Outline[R][C]={0};
uint8_t PIC_erzhihua[C];
//��ǰ��״̬
//��һ����״̬
OV7670_State OV_State=Wait_Vsync1;
u16 TrueLine;
u8 datareadyflag=0;
int left,right,data_last,data;
int left_flag,right_flag;
int Threshold = 100;
uint8_t y[R]={80};
uint8_t Curve_Value = 0;


/*
	����ͷģ���ʼ��

*/
void Data_Bus_Config(void)
{					  
	
	/*ͨ��GPIO�ṹ������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	
	/*����ʱ��*/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=D0_Pin;		
	GPIO_Init(D0_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=D1_Pin;		
	GPIO_Init(D1_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=D2_Pin;		
	GPIO_Init(D2_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=D3_Pin;		
	GPIO_Init(D3_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=D4_Pin;		
	GPIO_Init(D4_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=D5_Pin;		
	GPIO_Init(D5_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=D6_Pin;		
	GPIO_Init(D6_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=D7_Pin;		
	GPIO_Init(D7_Port,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	//PWDN
	GPIO_InitStructure.GPIO_Pin=PWDN_Pin;
	GPIO_Init(PWDN_Port,&GPIO_InitStructure);
	//RESET
	GPIO_InitStructure.GPIO_Pin=RESET_Pin;
	GPIO_Init(RESET_Port,&GPIO_InitStructure);
	//FIFO_RCK
	GPIO_InitStructure.GPIO_Pin=FIFO_RCK_Pin;
	GPIO_Init(FIFO_RCK_Port,&GPIO_InitStructure);
	//FIFO_EO
	GPIO_InitStructure.GPIO_Pin=FIFO_OE_Pin;
	GPIO_Init(FIFO_OE_Port,&GPIO_InitStructure);
	//FIFO_RRST
	GPIO_InitStructure.GPIO_Pin=FIFO_RRST_Pin;
	GPIO_Init(FIFO_RRST_Port,&GPIO_InitStructure);
	//FIFO_WR
	GPIO_InitStructure.GPIO_Pin=FIFO_WR_Pin;
	GPIO_Init(FIFO_WR_Port,&GPIO_InitStructure);
	//FIFO_WRST
	GPIO_InitStructure.GPIO_Pin=FIFO_WRST_Pin;
	GPIO_Init(FIFO_WRST_Port,&GPIO_InitStructure);
	//STORBE
	GPIO_InitStructure.GPIO_Pin=STORBE_Pin;
	GPIO_Init(STORBE_Port,&GPIO_InitStructure);
}
void OV7670_Init(void)
{
	/*I2C�������ų�ʼ��*/
	I2C_Configuration();
	/*����������������߳�ʼ��*/
	Data_Bus_Config();
	OV7670_Register_Init();
	/*���ϳ�ʼ������֮��ſ��Գ�ʼ�������*/
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_Config(EN);

	
}
void OV7670_Register_Init(void)
{
	PWDN_L;/*��Դ������ʽ*/
	RESET_L;//�Ĵ�����λ
	delay_ms(20);
	RESET_H;

	//YUV��ʽ
	I2C_WriteByte(0x12,0x80);
	I2C_WriteByte(0x3a, 0x04);
	I2C_WriteByte(0x40, 0xd0);
	I2C_WriteByte(0x12, 0x14);
	I2C_WriteByte(0x32, 0x80);
	I2C_WriteByte(0x17, 0x16);
	I2C_WriteByte(0x18, 0x04);
	I2C_WriteByte(0x19, 0x02);
	I2C_WriteByte(0x1a, 0x7b);
	I2C_WriteByte(0x03, 0x06);
	I2C_WriteByte(0x0c, 0x00);
	I2C_WriteByte(0x3e, 0x00);
	I2C_WriteByte(0x70, 0x3a);
	I2C_WriteByte(0x71, 0x35);
	I2C_WriteByte(0x72, 0x11);
	I2C_WriteByte(0x73, 0x00);
	I2C_WriteByte(0xa2, 0x02);
	I2C_WriteByte(0x11, 0x81); 	
	I2C_WriteByte(0x7a, 0x20);
	I2C_WriteByte(0x7b, 0x1c);
	I2C_WriteByte(0x7c, 0x28);
	I2C_WriteByte(0x7d, 0x3c);
	I2C_WriteByte(0x7e, 0x55);
	I2C_WriteByte(0x7f, 0x68);
	I2C_WriteByte(0x80, 0x76);
	I2C_WriteByte(0x81, 0x80);
	I2C_WriteByte(0x82, 0x88);
	I2C_WriteByte(0x83, 0x8f);
	I2C_WriteByte(0x84, 0x96);
	I2C_WriteByte(0x85, 0xa3);
	I2C_WriteByte(0x86, 0xaf);
	I2C_WriteByte(0x87, 0xc4);
	I2C_WriteByte(0x88, 0xd7);
	I2C_WriteByte(0x89, 0xe8);	
	I2C_WriteByte(0x13, 0xe0);
	I2C_WriteByte(0x00, 0x00); 	
	I2C_WriteByte(0x10, 0x00);
	I2C_WriteByte(0x0d, 0x00);
	I2C_WriteByte(0x14, 0x28);
	I2C_WriteByte(0xa5, 0x05);
	I2C_WriteByte(0xab, 0x07);
	I2C_WriteByte(0x24, 0x75);
	I2C_WriteByte(0x25, 0x63);
	I2C_WriteByte(0x26, 0xA5);
	I2C_WriteByte(0x9f, 0x78);
	I2C_WriteByte(0xa0, 0x68);
	I2C_WriteByte(0xa1, 0x03);
	I2C_WriteByte(0xa6, 0xdf);
	I2C_WriteByte(0xa7, 0xdf);
	I2C_WriteByte(0xa8, 0xf0);
	I2C_WriteByte(0xa9, 0x90);
	I2C_WriteByte(0xaa, 0x94);
	I2C_WriteByte(0x13, 0xe5);
	I2C_WriteByte(0x0e, 0x61);
	I2C_WriteByte(0x0f, 0x4b);
	I2C_WriteByte(0x16, 0x02);
	I2C_WriteByte(0x1e, 0x37);
	I2C_WriteByte(0x21, 0x02);
	I2C_WriteByte(0x22, 0x91);
	I2C_WriteByte(0x29, 0x07);
	I2C_WriteByte(0x33, 0x0b);
	I2C_WriteByte(0x35, 0x0b);
	I2C_WriteByte(0x37, 0x1d);
	I2C_WriteByte(0x38, 0x71);
	I2C_WriteByte(0x39, 0x2a);
	I2C_WriteByte(0x3c, 0x78);
	I2C_WriteByte(0x4d, 0x40);
	I2C_WriteByte(0x4e, 0x20);
	I2C_WriteByte(0x69, 0x00);
	I2C_WriteByte(0x6b, 0x60);
	I2C_WriteByte(0x74, 0x19);
	I2C_WriteByte(0x8d, 0x4f);
	I2C_WriteByte(0x8e, 0x00);
	I2C_WriteByte(0x8f, 0x00);
	I2C_WriteByte(0x90, 0x00);
	I2C_WriteByte(0x91, 0x00);
	I2C_WriteByte(0x92, 0x00);
	I2C_WriteByte(0x96, 0x00);
	I2C_WriteByte(0x9a, 0x80);
	I2C_WriteByte(0xb0, 0x84);
	I2C_WriteByte(0xb1, 0x0c);
	I2C_WriteByte(0xb2, 0x0e);
	I2C_WriteByte(0xb3, 0x82);
	I2C_WriteByte(0xb8, 0x0a); 
	I2C_WriteByte(0x43, 0x14);
	I2C_WriteByte(0x44, 0xf0);
	I2C_WriteByte(0x45, 0x34);
	I2C_WriteByte(0x46, 0x58);
	I2C_WriteByte(0x47, 0x28);
	I2C_WriteByte(0x48, 0x3a);
	I2C_WriteByte(0x59, 0x88);
	I2C_WriteByte(0x5a, 0x88);
	I2C_WriteByte(0x5b, 0x44);
	I2C_WriteByte(0x5c, 0x67);
	I2C_WriteByte(0x5d, 0x49);
	I2C_WriteByte(0x5e, 0x0e);
	I2C_WriteByte(0x64, 0x04);
	I2C_WriteByte(0x65, 0x20);
	I2C_WriteByte(0x66, 0x05);
	I2C_WriteByte(0x94, 0x04);
	I2C_WriteByte(0x95, 0x08);
	I2C_WriteByte(0x6c, 0x0a);
	I2C_WriteByte(0x6d, 0x55);
	I2C_WriteByte(0x6e, 0x11);
	I2C_WriteByte(0x6f, 0x9f);
	I2C_WriteByte(0x6a, 0x40);
	I2C_WriteByte(0x01, 0x40);
	I2C_WriteByte(0x02, 0x40);
	I2C_WriteByte(0x13, 0xe7);
	I2C_WriteByte(0x15, 0x00);  	
	I2C_WriteByte(0x4f, 0x80);
	I2C_WriteByte(0x50, 0x80);
	I2C_WriteByte(0x51, 0x00);
	I2C_WriteByte(0x52, 0x22);
	I2C_WriteByte(0x53, 0x5e);
	I2C_WriteByte(0x54, 0x80);
	I2C_WriteByte(0x58, 0x9e); 	
	I2C_WriteByte(0x41, 0x08);
	I2C_WriteByte(0x3f, 0x00);
	I2C_WriteByte(0x75, 0x05);
	I2C_WriteByte(0x76, 0xe1);
	I2C_WriteByte(0x4c, 0x00);
	I2C_WriteByte(0x77, 0x01);
	I2C_WriteByte(0x3d, 0xc2);	
	I2C_WriteByte(0x4b, 0x09);
	I2C_WriteByte(0xc9, 0x60);
	I2C_WriteByte(0x41, 0x38);
	I2C_WriteByte(0x56, 0x40);	
	I2C_WriteByte(0x34, 0x11);
	I2C_WriteByte(0x3b, 0x02); 								
	I2C_WriteByte(0xa4, 0x89);
	I2C_WriteByte(0x96, 0x00);
	I2C_WriteByte(0x97, 0x30);
	I2C_WriteByte(0x98, 0x20);
	I2C_WriteByte(0x99, 0x30);
	I2C_WriteByte(0x9a, 0x84);
	I2C_WriteByte(0x9b, 0x29);
	I2C_WriteByte(0x9c, 0x03);
	I2C_WriteByte(0x9d, 0x4c);
	I2C_WriteByte(0x9e, 0x3f);
	I2C_WriteByte(0x78, 0x04);	
	I2C_WriteByte(0x79, 0x01);
	I2C_WriteByte(0xc8, 0xf0);
	I2C_WriteByte(0x79, 0x0f);
	I2C_WriteByte(0xc8, 0x00);
	I2C_WriteByte(0x79, 0x10);
	I2C_WriteByte(0xc8, 0x7e);
	I2C_WriteByte(0x79, 0x0a);
	I2C_WriteByte(0xc8, 0x80);
	I2C_WriteByte(0x79, 0x0b);
	I2C_WriteByte(0xc8, 0x01);
	I2C_WriteByte(0x79, 0x0c);
	I2C_WriteByte(0xc8, 0x0f);
	I2C_WriteByte(0x79, 0x0d);
	I2C_WriteByte(0xc8, 0x20);
	I2C_WriteByte(0x79, 0x09);
	I2C_WriteByte(0xc8, 0x80);
	I2C_WriteByte(0x79, 0x02);
	I2C_WriteByte(0xc8, 0xc0);
	I2C_WriteByte(0x79, 0x03);
	I2C_WriteByte(0xc8, 0x40);
	I2C_WriteByte(0x79, 0x05);
	I2C_WriteByte(0xc8, 0x30);
	I2C_WriteByte(0x79, 0x26); 
	I2C_WriteByte(0x09, 0x00);	



	/*
	

//        {0x3a, 0x04},//RGB
        {0x12, 0x10},//QVGA  YUV
        {0x3a, 0x14},//ʹ�ù̶�UV���
        {0x3d, 0x80},//ʹ�ù̶�UV���
                                                        //0         0       :      Y U Y V ��3a:14 3d:80��
                                                        //0         1       :      Y V Y U  (3a:14 3d:81)
                                                        //1         0       :      U Y V Y (3a:18 3d:80)
                                                        //1         1       :      V Y U Y (3a:18 3d:81)
        {0x67, 0x11},//�̶�Uֵ��0x11���������
        {0x68, 0xFF},//�̶�Vֵ��0xFF���������
//        {0x40, 0x10},//RGB565
        {0x40, 0xC0},//��ʼ���ã�YUVģʽ������Ĵ����������ã����򲻺�ʹ
//        {0x12, 0x14},//QVGA  RGB��


	   I2C_WriteByte(0x12, 0x10);
	   I2C_WriteByte (0x3a, 0x14);
	   I2C_WriteByte (0x3d, 0x80);
	   I2C_WriteByte(0x67, 0x11);
	   I2C_WriteByte(0x68, 0xFF);
	   I2C_WriteByte(0x40, 0xC0);
	 

	*/
	   I2C_WriteByte(0x12, 0x10);
	   I2C_WriteByte(0x3a, 0x14);
	   I2C_WriteByte(0x3d, 0x80);
	   I2C_WriteByte(0x67, 0x11);
	   I2C_WriteByte(0x68, 0xFF);
	   I2C_WriteByte(0x40, 0xC0);

	   LED_1(ON);
	   LED_2(ON);
}

void Read_Pic(void)
{
	u16 i,j;
	u16 t1,t2;
	//���´�����ɣ�ͼ��Ĳɼ�������ʾ��tft��				 							
	for(i = 0; i < 240; i ++)	 
	{	
		 if(i%6==1)
		{
			for(j = 0; j < 320; j ++)
			{
				if (j<160)
				{
				    //���һ��Y�����Ĺ���										
					FIFO_RCK_L;					
					FIFO_RCK_H;		 				
					t1=(0x00ff&GPIOC->IDR);	 				
					FIFO_RCK_L;						
					FIFO_RCK_H;						
					t2=(0x00ff&GPIOC->IDR);
	
					//����һ���㣬����ȡ��
				    FIFO_RCK_L;					
					FIFO_RCK_H;		 				
					t2=(0x00ff&GPIOC->IDR);	 				
					FIFO_RCK_L;						
					FIFO_RCK_H;						
					t2=(0x00ff&GPIOC->IDR);
					PIC[TrueLine][j]=t1; 
					}
	    	}
		  TrueLine++;
		}
		else
		{	  //����������Щ���ص�
	      	for(j = 0; j < 320; j ++)
			{
				FIFO_RCK_L;					
				FIFO_RCK_H;		 				
				t2=(0x00ff&GPIOC->IDR);	 				
				FIFO_RCK_L;						
				FIFO_RCK_H;						
				t2=(0x00ff&GPIOC->IDR);
			}
		}
	}
	FIFO_OE_H;		 	  //��ֹFIFO���
}
/*************************************************************
��������void Send_Pic(void)
���ܣ�ͨ�����ڷ���ͼƬ
���룺��
�������
*************************************************************/
void Send_Pic(void)
{
	u8 i;
	u8 j;
	USART_ClearFlag(USART1, USART_FLAG_TC); 
	USART_SendData(USART1,0x00);
  	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1,0xff);
  	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1,0x01);
  	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1,0x00);
  	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	
	for(i=0;i<40;i++)
	{
		for(j=0;j<160;j++)
		{
			USART_ClearFlag(USART1, USART_FLAG_TC); 
		  	USART_SendData(USART1,PIC[i][j]);
			//USART_SendData(USART1,PIC_Smooth[i][j]);
			//USART_SendData(USART1,PIC_Outline[i][j]);
  			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		}
	}
}

/*********************************************************************
*ͼ������ 
*�ӵ�40�����ϴ�����Ϊʶ��˫�ߣ��м������߲��Һڰ��������ҵ���
*�������ؽ������ߵļ�¼����ʱ�������������1�������߶��ҵ���2��
*ֻ������ߡ�3��ֻ���ұ��ߡ�4�������߶�û�С�
*********************************************************************/
/*void V_control(uint8_t (*p)[160])
{
	int hang,lie;
  	for(hang=39;hang>0;hang--)
  	{
    	if(hang == 39)
		{
    		for(lie=0;lie<160;lie++)
			{
	  			if(*(*(p+hang)+lie)<100)
	  			{
	    			PIC_erzhihua[lie]=0x01; 
	  			}
	  			else
	  			{	
	    			PIC_erzhihua[lie]=0x00;
	  			}
			} 
    		for(lie=0;lie<159;lie++)
			{
				if((PIC_erzhihua[lie]>PIC_erzhihua[lie-1])&&(PIC_erzhihua[lie]>PIC_erzhihua[lie-2])
	     			&&(PIC_erzhihua[lie+1]>PIC_erzhihua[lie-1])&&(PIC_erzhihua[lie+1]>PIC_erzhihua[lie-2]))
				{
					left = lie;
		   			left_flag = 1;
		   			break;		    
		 		}
	    		left_flag = 0; 
			}
			for(lie=159;lie>0;lie--)
			{
	  	 		if((PIC_erzhihua[lie]>PIC_erzhihua[lie+1])&&(PIC_erzhihua[lie]>PIC_erzhihua[lie+2])
	     			&&(PIC_erzhihua[lie-1]>PIC_erzhihua[lie+1])&&(PIC_erzhihua[lie-1]>PIC_erzhihua[lie+2]))
		 		{
		   			right = lie;
		   			right_flag = 1;
		  			break;		    
		 		}
	    		right_flag = 0; 
			}
			//������ȫ���ҵ� ���������ߵ��м�ֵ
			if(( right_flag == 1 )&&( left_flag == 1 ))
			{
	 			data = (left+right)/2;
	  			data_last =  data;
	  			right_flag = 0;
	  			left_flag = 0;
			}
			//
			else 
			{
	 			data = data_last; 
			}
			left = 0;
			right = 0;
    		for(lie=0;lie<160;lie++)
			{
	  			PIC_erzhihua[lie]=0x00;
			} 
			USART_SendData(USART1,data);
  			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 
		}
	}    
}*/

/****************************************************************************
��������PIC_Process()
���ܣ���ֵ�����˲�
��ע��2015��4��26���Զ������
****************************************************************************/

void PIC_Process()
{
	int end = R-5;
	int start = end-7;
	TwoValue_Process();
	Get_MidLine();
	SmoothMid();
   	for (;start>=4;start -= 6, end -= 6)
	{
		Least_Squares(start, end);
	}
	Curve_Value = Curve();
	Display_MidLine();
}

/****************************************************************************
��������void TwoValue_Process(void)
���ܣ���ֵ�����˲�
��ע��2015��4��26�����
****************************************************************************/
void TwoValue_Process(void)
{
	int row=0;
	int col=0;
	Threshold = Get_GrayValue(); //��ü����ĻҶ���ֵ
	for (row = R-1; row>=0; row--)//�����row��
	{
		for (col = 0; col<C; col++)//��ֵ��
		{
			if(PIC[row][col] >= Threshold)
			{
				PIC[row][col] = white;
			}
			else 
			{
				PIC[row][col] = black;
			}
		}
		for(col=1; col<C-1; col++)//�˲�
		{
			if( PIC[row][col-1]==white && PIC[row][col]==black && PIC[row][col+1]==white )
			{
				PIC[row][col]=white;
			}
			else if( PIC[row][col-1]==black && PIC[row][col]==white && PIC[row][col+1]==black )
			{
				PIC[row][col]=black;
			}
		}
	}
}

/*********************************************************************
����������ʾ���ߺ���
���ܣ���PICͼ������ʾ���ߺ���
���룺��
�������
��ע��2015��4��28���Զ������
*********************************************************************/
void Display_MidLine()
{
	int row;
	for (row=R-1;row>=0;row--)
	{
		PIC[row][Mid_Line[row]]=black;
	}
}



/*********************************************************************
��������void Get_MidLine()
���ܣ���ȡͼ��������
�������
�������
��ע��2015��4��28���Զ������
*********************************************************************/
void Get_MidLine()
{
	int row = 0;
	int col = 0;
	int Left_Border = 0;
	int Right_Border = 0;
	short int Left_Find = 0;
	short int Right_Find = 0;

	for(row=R-1;row>=0;row--)
	{
		if(row>=30)//ǰ10�д���
		{
			for(col=39;col>=0;col--)
			{
				if((PIC[row][col]==black) && !Left_Find)
				{
					Left_Border = col;
					if(PIC[row][col-1]==black)
					{
						Left_Find = 1;
					}
				}
				if(PIC[row][C-1-col]==black && !Right_Find)
				{
					Right_Border = (C-1-col);
					if(PIC[row][C-2-col]==black)
					{
							Right_Find = 1;
					}
				}
				if(Left_Find && Right_Find)
					break;
			}
			if(Left_Find == 0)
				Left_Border = 0;
			if(Right_Find == 0)
				Right_Border = 160;
		}
		else	//��30�д���
		{
			for(col=79;col>=0;col--)
			{
				if((PIC[row][col]==black) && !Left_Find)
				{
					Left_Border = col;
					if(PIC[row][col-1]==black)
					{
						Left_Find = 1;
					}
				}
				if(PIC[row][C-1-col]==black && !Right_Find)
				{
					Right_Border = (C-1-col);
					if(PIC[row][C-1-col-1]==black)
					{
						 Right_Find = 1;
					}
				}
				if(Left_Find && Right_Find)
					break;
			}
			if(Left_Find == 0)
				Left_Border = 0;
			if(Right_Find == 0)
				Right_Border = 160;
		}
		Mid_Line[row]=(Left_Border+Right_Border)/2;
		Left_Find = 0;
		Right_Find = 0;
	}
}



/**********************************************************************
��������void PIC_Process_2(void)
���룺��
�������
���ܣ�����ͼ�񣬰���ͼ��ƽ�����񻯣���ֵ��
��ע��2015��4��27���Զ������
**********************************************************************/
/*void PIC_Process_2(void)
{
	  int i = 0;
	  int j = 0;
	  int row = 0;
	  int col = 0;
	  long int sum = 0;
	  
	  int templates[3][3]={
	  						{1,1,1},
	  						{1,1,1},
	  						{1,1,1}
	  };

	  for(row=1;row<=R-2;row++)
	  {
	  	for(col=1;col<=C-2;col++)
		{
			for(i=-1;i<2;i++)
			{
				for (j=-1;j<2;j++)
				{
					 //ͼ��ƽ��
					 sum += PIC[row+i][col+j]*templates[i+1][j+1];
				}
			}
			sum = sum/9;
			if(sum>255)
				sum = 255;
			if(sum<0)
				sum = 0;
			PIC_Smooth[row][col] = sum;//��ȡƽ�����ͼ��
			//PIC[row][col] = sum;
			
			sum = 0;
		}
	  }
	  //ͼ�����㷨
	  for(row=0;row<R;row++)
	  {
	  	for(col=0;col<C;col++)
		{
			//PIC_Outline[row][col] = PIC[row][col] + (PIC[row][col]-PIC_Smooth[row][col]); //��ͼ=ԭͼ +��ԭͼ-ƽ��ͼ��
			PIC[row][col] = PIC[row][col] + (PIC[row][col]-PIC_Smooth[row][col]);
		}
	  }
	  for (row=0;row<R;row++)
	  {
	  	for(col=0;col<C;col++)
		{
			if(PIC[row][col]<=Threshold)
			{
				PIC[row][col]=black;
			}
			else
			{
				PIC[row][col]=white;
			}
		}
	  }
}  */


/***************************************************
��������int8_t Abs8(int8_t a)
���ܣ���8λ���ݵľ���ֵ
���룺a
�����temp
��ע��2015��5��3�����
***************************************************/
int8_t Abs8(int8_t a)
{
	int8_t temp;
	temp=a;
	if (temp<=0)
	{
		temp = -temp;
	}
  	return temp;
}


/**********************************************************************
��������SmoothMid(void)
���ܣ�ƽ������
���룺��
�������
��ע��2015��5��3�����
**********************************************************************/
void SmoothMid(void)
{
	uint8_t  i;
	uint8_t temp=0;
	uint8_t temp1=0;
 	for (i=R-1;i>3;i--) 
	{
		if(Mid_Line[i] != Mid_Line[i-1])
		{
			temp = Abs8(Mid_Line[i-1]-Mid_Line[i]);
		}
		

		if(Mid_Line[i] != Mid_Line[i+1])
		{
		 	temp1 = Abs8(Mid_Line[i+1]-Mid_Line[i]);
		}
		
		if (((temp>5)&&(temp1>5)) || ((Mid_Line[i-1]<Mid_Line[i])&&(Mid_Line[i+1]<Mid_Line[i])) || ((Mid_Line[i-1]>Mid_Line[i])&&(Mid_Line[i+1]>Mid_Line[i])))	 
		{
			Mid_Line[i]=(Mid_Line[i-1]+Mid_Line[i+1])*0.5;
		}
	}
}


/****************************************************************************
��������Least_Squares(int start, int end)
���ܣ�ͨ����С���˷�ƽ��������
���룺start,end
�������
��ע��2015��5��3�����
*****************************************************************************/
void Least_Squares(int start, int end)
{
	uint8_t i=0;
	double A=0, B=0, D=0, F=0, Delta;
	
	double a,b;
	int t;

	for (i=end;i>=start;i--)
	{
		//BlackLineData[i]=y[i];
		y[i]=Mid_Line[i];
	}

	
	for (i=start;i<=end;i++)
	{
		A += i*i; //pow(i,2)
		B += i;
 		D += i*y[i];
		F += y[i];
	}
	
	t = end-start+1;
	Delta = A*t-B*B;
	a = (D*t-B*F)/Delta;
	b = (A*F-D*B)/Delta;
	
	
	for (i=start;i<=end;i++)
	{
		Mid_Line[i]=(a*i+b);
	}
	
	A=0;
	B=0;
	D=0;
	F=0;
	
	//y[]={0};
}
/*****************************************************************
��������Curve(void)
���ܣ��������ʰ뾶
���룺��
�������
��ע��s8 Curve(void)
*****************************************************************/
uint8_t Curve(void)
{
	int8_t i;
	uint8_t sum_MidLine=0;
	uint8_t average_MidLine=0;
	uint8_t curve_MidLine=0;
	uint8_t differ_MidLine=0;
	for(i=25, sum_MidLine=0; i>=10; i--)
	{
		sum_MidLine+=Mid_Line[i];
	}
	average_MidLine = sum_MidLine / 16;
	for(i=25, curve_MidLine=0;i>=10;i--)
	{
		differ_MidLine = Abs8(Mid_Line[i]-average_MidLine);
		curve_MidLine += differ_MidLine;
	}
	return curve_MidLine;
}

/*******************************************************
��������int Get_GrayValue()
���룺�ޣ�
������Ҷ���ֵ
˵����������ֵ�Ҷȷָ��㷨
��ע��2015��4��27���Զ������
*******************************************************/
int Get_GrayValue()
{
	 int row,col;
	 int Obj_Count=0;//Ŀ����ֵ��������
	 int Bla_Count=0;//������ֵ��������
	 uint32_t Obj_GrayValue=0;//Ŀ��Ҷ��ܺ� 
	 uint32_t Bla_GrayValue=0;//�����Ҷ��ܺ�
	 int Min_GrayValue = 127;//��С��ֵ
	 int Max_GrayValue = 127;//�����ֵ
	 int Init_GrayValue = 0;//����Ҷ���ֵ
	 int Init_GrayValue_Last = 0;//��¼��һ�μ��������Ҷ���ֵ
	 int Mean_GrayValue_1 = 0;//Ŀ��Ҷ�ƽ����ֵ
	 int Mean_GrayValue_2 = 0;//�����Ҷ�ƽ����ֵ
	 
	 /*ɨ������ͼƬ����¼��С�Ҷ���ֵ�����Ҷ���ֵ*/
	 for(row=0;row<R;row++)
	 {
	 	for(col=0;col<C;col++)
		{
			if(Min_GrayValue>PIC[row][col])
				Min_GrayValue = PIC[row][col];//������С�Ҷ���ֵ
			if(Max_GrayValue<PIC[row][col])
				Max_GrayValue = PIC[row][col];//�������Ҷ���ֵ
		}
	 }
	 Init_GrayValue = (Min_GrayValue+Max_GrayValue)/2;//�õ�����ĻҶ���ֵ
	 
	 /*����Ҷ���ֵ��ֱ��ѭ��������ĻҶ���ֵ����һ�μ���ĻҶ���ֵ���*/
	 do
	 {
	 	Init_GrayValue_Last = Init_GrayValue;
		Obj_Count=0;
		Bla_Count=0;
		Obj_GrayValue=0;
		Bla_GrayValue=0;
	 	for(row=0;row<R;row++)
	 	{
	 		for(col=0;col<C;col++)
			{
				if((Min_GrayValue<PIC[row][col])&&(PIC[row][col]<Init_GrayValue))//����С�Ҷ���ֵ������Ҷ���ֵ
				{
					Obj_GrayValue += PIC[row][col];//�˷�Χ�ڻҶ����
					Obj_Count++;//����	
				}
				if((Init_GrayValue<PIC[row][col])&&(PIC[row][col]<Max_GrayValue))//������Ҷ���ֵ�����Ҷ���ֵ
				{
					Bla_GrayValue += PIC[row][col];//�˷�Χ�ڻҶ����
					Bla_Count++;//����
				}	
			}	
	 	}
	 	Mean_GrayValue_1 = Obj_GrayValue/Obj_Count;//����Ŀ��Ҷ�ƽ����ֵ
	 	Mean_GrayValue_2 = Bla_GrayValue/Bla_Count;//���������Ҷ�ƽ����ֵ
	 	Init_GrayValue = (Mean_GrayValue_1+Mean_GrayValue_2)/2;//����ƽ���Ҷ���ֵ
	 }while(Init_GrayValue_Last!=Init_GrayValue);//ֱ����һ��ƽ���Ҷ���ֵ�����¼��������ƽ���Ҷ���ֵ���Ϊֹ
	 
	 /*���ػҶ���ֵ*/
	 return Init_GrayValue;
}
