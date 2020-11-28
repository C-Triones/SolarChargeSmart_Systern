#include "./adxl345/bsp_adxl345.h"
#include "./usart/bsp_usart.h"
#include "./parameter/parameter.h"
#include "./timer/bsp_timer.h"
int state=0;
int main(void)//72MHz
{
	delay_init();	    	 //延时函数初始化	
  USART_Config(); 	//串口初始化为9600
	ADLX345_Init();
	GENERAL_TIM_Init();
	Parameter_Init();

	if(ADLX345_Single_Read(0X00)==0xe5){
       delay_ms(5);
   }
  else{
       delay_ms(3);
  } 

  while(Ture){

	 Get_ridestate();
	
   }
}


