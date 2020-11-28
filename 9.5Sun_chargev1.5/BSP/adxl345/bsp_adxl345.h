#ifndef __adxl345_H
#define	__adxl345_H

#include "stm32f10x.h"
#include "./delay/bsp_delay.h"

#define	SlaveAddress   0xA6	  //定义器件在IIC总线中的从地址

#define ADLX345_SDA_RCC			RCC_APB2Periph_GPIOA
#define ADLX345_SDA_GPIO		GPIOA
#define ADLX345_SDA_GPIO_PIN	GPIO_Pin_4

#define ADLX345_SCL_RCC			RCC_APB2Periph_GPIOA
#define ADLX345_SCL_GPIO		GPIOA
#define ADLX345_SCL_GPIO_PIN	GPIO_Pin_5

#define ADLX345_SCL_OUT() ADLX345_SCL_Set_Output() //置位scl
#define ADLX345_SET_SCL() GPIO_SetBits(ADLX345_SCL_GPIO, ADLX345_SCL_GPIO_PIN) //置位scl
#define ADLX345_CLE_SCL() GPIO_ResetBits(ADLX345_SCL_GPIO, ADLX345_SCL_GPIO_PIN)//清scl
                    
#define ADLX345_SDA_OUT() ADLX345_SDA_Set_Output()
#define ADLX345_SDA_INT() ADLX345_SDA_Set_Input()
#define ADLX345_SET_SDA() GPIO_SetBits(ADLX345_SDA_GPIO, ADLX345_SDA_GPIO_PIN)//置位sda
#define ADLX345_CLE_SDA() GPIO_ResetBits(ADLX345_SDA_GPIO, ADLX345_SDA_GPIO_PIN)//清楚sda
#define ADLX345_SDA_VAL() GPIO_ReadInputDataBit(ADLX345_SDA_GPIO, ADLX345_SDA_GPIO_PIN)

typedef struct{
 float temp_X;
 float temp_Y;
 float temp_Z;
}Adxl345Struct_t;

/**************************外部变量声明****************************/

extern Adxl345Struct_t Adxl345;

/**************************函数声明*******************************/

void     ADLX345_Init(void);                             
void     ADLX345_ReadData(void);
uint16_t ADLX345_Single_Read(uint16_t REG_Address);

#endif
