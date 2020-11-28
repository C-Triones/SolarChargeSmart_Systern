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
uint16_t Normal_num;               //姿态正常次数
uint16_t Error_num;                //姿态倾斜次数
	
uint32_t  Ride_time;	             //骑行时常
float  min_dot;
float min_int;
float  h_dot;
float h_int;
uint8_t   RideCheck_Flage;         //检测是否在骑行
uint16_t  RideCheck_time;	         //判断是否在骑行的时间最大间隔
float     Average_speed;           //平均速度
float     Instantaneous_speed;     //瞬时速度
uint32_t  Step_num;                //有效的动作个数
uint32_t  Laster_Step_num;         //上一次的有效动作个数
uint32_t  Ride_distance;            //骑行距离
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

/************************外部变量声明******************************/

extern uint16_t Message_Recv[5];
extern MovedataStruct_t Movedata;
/**************************函数声明********************************/

void Get_ridestate(void);
void Calculate_ridestate(void);

#endif
