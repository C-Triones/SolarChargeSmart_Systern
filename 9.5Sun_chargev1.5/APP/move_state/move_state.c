#include "./move_state/move_state.h"

//��ʱ��ȡadxl345����

uint16_t Massage_Sent[4];
MovedataStruct_t Movedata;

void Get_ridestate(void){  	 
		
    if(Movedata.ReadAdxl345_Flag==Ture){ //��ʱ��ȡadxl345����
      Movedata.ReadAdxl345_Flag= Flase;
      ADLX345_ReadData();//�����⺯��
    //  Movedata.Check_Num++;
      if((Adxl345.temp_X>600)||(  Adxl345.temp_X<-550)){    //�鿴�������� 
       Movedata.Normal_num++;   //��������++
      }
      else{
        Movedata.Error_num++;    //��б����
      }
      if((Movedata.Error_num!=Flase)&&(Movedata.Normal_num!=Flase)){//��⵽����
				Movedata.Step_num++;           //����������++
        Movedata.Error_num=Flase;     //���һ�����ڼ��
        Movedata.Normal_num=Flase;
      }
    }
		/*�޸�ϵ���Ϳ�����
		*/
			Movedata.Ride_distance = Movedata.Step_num*5.45;//��ʾ����	
}






