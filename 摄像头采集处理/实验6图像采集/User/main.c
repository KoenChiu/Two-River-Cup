#include "includes.h"    
/*
------------------------------------------------------  
  函数名称：int main(void)
  函数功能：应用函数
  输入形参：无  	
  返回值：无	
------------------------------------------------------
*/
int main(void)
{
	/*硬件初始化*/
	Board_Init();
	/*应用函数*/
	Application();
	/*正常情况下不会执行到这一步*/
	return 0;
}
