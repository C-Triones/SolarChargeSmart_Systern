#include "./move_state/move_state.h"

//定时读取adxl345数据

uint16_t Massage_Sent[4];
MovedataStruct_t Movedata;

void Get_ridestate(void){  	 
		
    if(Movedata.ReadAdxl345_Flag==Ture){ //定时读取adxl345数据
      Movedata.ReadAdxl345_Flag= Flase;
      ADLX345_ReadData();//三轴检测函数
    //  Movedata.Check_Num++;
      if((Adxl345.temp_X>600)||(  Adxl345.temp_X<-550)){    //查看正常次数 
       Movedata.Normal_num++;   //正常次数++
      }
      else{
        Movedata.Error_num++;    //倾斜次数
      }
      if((Movedata.Error_num!=Flase)&&(Movedata.Normal_num!=Flase)){//检测到步数
				Movedata.Step_num++;           //步数脉冲量++
        Movedata.Error_num=Flase;     //清除一个周期检测
        Movedata.Normal_num=Flase;
      }
    }
		/*修改系数就可以了
		*/
			Movedata.Ride_distance = Movedata.Step_num*5.45;//显示距离	
}






