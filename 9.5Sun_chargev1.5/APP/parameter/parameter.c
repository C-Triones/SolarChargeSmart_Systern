#include "./parameter/parameter.h"
#include "./timer/bsp_timer.h"
#include "./move_state/move_state.h"
/**ȫ�ֱ�����ʼ��*/
void Parameter_Init(void){	
	Movedata.CalculateInstantaneous_speed_Time=0;//ÿ��·�̱仯��ʱ��
	Movedata.RideCheck_time=0;
	Movedata.RideCheck_Flage=Flase;
	Movedata.Ride_time=0;

	Movedata.Instantaneous_speed=0;
	Movedata.Average_speed=0;
	Movedata.Laster_Step_num=0;
	Movedata.Step_num =0;	
	Movedata.Ride_distance =0;	
	Movedata.CalculateAverage_speed_Flage=Flase;
	Movedata.CalculateAverage_speed_Time=0;
	Movedata.CalculateInstantaneous_speed_Time=0.5;//���ж�ʱ��һ��
	Movedata.ReadAdxl345_Flag=Flase;	
//	Movedata.DisplayAdxl345Data_Flage=Flase;
	CheckTime.time_10ms  =1;
	CheckTime.time_50ms  =5;
	CheckTime.time_100ms =10;
	CheckTime.time_200ms =20;
	CheckTime.time_500ms =50;
	CheckTime.time_1s  =100;
	CheckTime.time_2s  =200;
	CheckTime.time_5s  =500;
	CheckTime.time_10s =10000;
	
}
