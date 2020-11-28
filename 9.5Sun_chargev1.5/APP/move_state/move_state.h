#ifndef __MOVE_H
#define	__MOVE_H

#include "stm32f10x.h"
#include "./delay/bsp_delay.h"
#include "./adxl345/bsp_adxl345.h"
#include "./timer/bsp_timer.h"
#include "./usart/bsp_usart.h"


#define Ture  1
#define Flase 0 

typedef struct {
//uint8_t  Check_Num;      
uint16_t Normal_num;               //��̬��������
uint16_t Error_num;                //��̬��б����
	
uint32_t  Ride_time;	             //����ʱ��
float  min_dot;
float min_int;
float  h_dot;
float h_int;
uint8_t   RideCheck_Flage;         //����Ƿ�������
uint16_t  RideCheck_time;	         //�ж��Ƿ������е�ʱ�������
float     Average_speed;           //ƽ���ٶ�
float     Instantaneous_speed;     //˲ʱ�ٶ�
uint32_t  Step_num;                //��Ч�Ķ�������
uint32_t  Laster_Step_num;         //��һ�ε���Ч��������
uint32_t  Ride_distance;            //���о���
uint8_t	  CalculateAverage_speed_Flage;
uint16_t	CalculateAverage_speed_Time;
uint8_t	  CalculateInstantaneous_speed_Flage;
float	CalculateInstantaneous_speed_Time;
uint16_t  Laster_distance;
uint16_t	Distance_err;
	
uint8_t  MoveFlage;	
uint8_t  ReadAdxl345_Flag;
uint8_t  DisplayAdxl345Data_Flage;
}MovedataStruct_t;

/************************�ⲿ��������******************************/

extern uint16_t Message_Recv[5];
extern MovedataStruct_t Movedata;
/**************************��������********************************/

void Get_ridestate(void);
void Calculate_ridestate(void);

#endif
