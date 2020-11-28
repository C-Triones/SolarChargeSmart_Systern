#ifndef __adxl345_H
#define	__adxl345_H

#include "stm32f10x.h"
#include "./delay/bsp_delay.h"

#define	SlaveAddress   0xA6	  //����������IIC�����еĴӵ�ַ

#define ADLX345_SDA_RCC			RCC_APB2Periph_GPIOA
#define ADLX345_SDA_GPIO		GPIOA
#define ADLX345_SDA_GPIO_PIN	GPIO_Pin_4

#define ADLX345_SCL_RCC			RCC_APB2Periph_GPIOA
#define ADLX345_SCL_GPIO		GPIOA
#define ADLX345_SCL_GPIO_PIN	GPIO_Pin_5

#define ADLX345_SCL_OUT() ADLX345_SCL_Set_Output() //��λscl
#define ADLX345_SET_SCL() GPIO_SetBits(ADLX345_SCL_GPIO, ADLX345_SCL_GPIO_PIN) //��λscl
#define ADLX345_CLE_SCL() GPIO_ResetBits(ADLX345_SCL_GPIO, ADLX345_SCL_GPIO_PIN)//��scl
                    
#define ADLX345_SDA_OUT() ADLX345_SDA_Set_Output()
#define ADLX345_SDA_INT() ADLX345_SDA_Set_Input()
#define ADLX345_SET_SDA() GPIO_SetBits(ADLX345_SDA_GPIO, ADLX345_SDA_GPIO_PIN)//��λsda
#define ADLX345_CLE_SDA() GPIO_ResetBits(ADLX345_SDA_GPIO, ADLX345_SDA_GPIO_PIN)//���sda
#define ADLX345_SDA_VAL() GPIO_ReadInputDataBit(ADLX345_SDA_GPIO, ADLX345_SDA_GPIO_PIN)

typedef struct{
 float temp_X;
 float temp_Y;
 float temp_Z;
}Adxl345Struct_t;

/**************************�ⲿ��������****************************/

extern Adxl345Struct_t Adxl345;

/**************************��������*******************************/

void     ADLX345_Init(void);                             
void     ADLX345_ReadData(void);
uint16_t ADLX345_Single_Read(uint16_t REG_Address);

#endif
