#include "./adxl345/bsp_adxl345.h"

static uint8_t BUF[8];    //接收数据缓存区      	
static uint8_t err;       //应答信号                    
Adxl345Struct_t Adxl345;
/**************************************
功能    ：SCL设置成输ru模式
**************************************/
void ADLX345_SCL_Set_Output(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ADLX345_SCL_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(ADLX345_SCL_GPIO, &GPIO_InitStructure);					 					
}	
/**************************************
功能    ：SDA设置成输出模式
**************************************/
void ADLX345_SDA_Set_Output(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ADLX345_SDA_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(ADLX345_SDA_GPIO, &GPIO_InitStructure);					 					
}	
/**************************************
功能    ：SDA设置成输入模式
**************************************/
static void ADLX345_SDA_Set_Input(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ADLX345_SDA_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(ADLX345_SDA_GPIO, &GPIO_InitStructure);					 
}

/**************************************
功能    ：起始信号
**************************************/
static void ADXL345_Start(void){
    ADLX345_SCL_OUT();
    ADLX345_SDA_OUT();
    ADLX345_SET_SDA();//SDA = 1;                    //拉高数据线
    ADLX345_SET_SCL();//SCL = 1;                    //拉高时钟线
    delay_us(2);               //延时
    ADLX345_CLE_SDA();//SDA = 0;                    //产生下降沿
    delay_us(2);//Delay5us();                 //延时
    ADLX345_CLE_SCL();//SCL = 0;                    //拉低时钟线
}

/**************************************
功能    ：停止信号
**************************************/
static void ADXL345_Stop(void){
    ADLX345_SCL_OUT();
    ADLX345_SDA_OUT();
    ADLX345_CLE_SDA();//SDA = 0;                    //拉低数据线
    ADLX345_SET_SCL();//SCL = 1;                    //拉高时钟线
    delay_us(2);//Delay5us();                 //延时
    ADLX345_SET_SDA();//SDA = 1;                    //产生上升沿
    delay_us(2);//Delay5us();                 //延时
    ADLX345_CLE_SCL();
}

/**************************************
功能    ：发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
static void ADXL345_SendACK(uint16_t ack){   
    ADLX345_SDA_INT();
    ADLX345_SDA_OUT();
    if(ack==0)//SDA = ack;                  //写应答信号
    {
      ADLX345_CLE_SDA();
    }
    else
    {
      ADLX345_SET_SDA();
    }
    ADLX345_SET_SCL();//SCL = 1;                    //拉高时钟线
    delay_us(2);//Delay5us();                 //延时
    ADLX345_CLE_SCL();//SCL = 0;                    //拉低时钟线
    delay_us(5);//Delay5us();                 //延时
}

/**************************************
功能    ：接收应答信号
**************************************/
static uint8_t ADXL345_RecvACK(void){
    ADLX345_CLE_SDA();
    ADLX345_SCL_OUT();
    ADLX345_SET_SCL();              //拉高时钟线
    delay_us(2);
    ADLX345_SET_SCL();
    if(ADLX345_SDA_VAL()== Bit_SET) //读应答信号
    {
      err = 1;
    }
    else
    {
      err = 0;
    }
 
    ADLX345_CLE_SCL() ;           //拉低时钟线
    delay_us(5); 
    ADLX345_SDA_OUT();
    return err;
}

/**************************************
向IIC总线发送一个字节数据
入口参数：dat (需要发送的一字节数据)
**************************************/
static void ADXL345_SendByte(uint8_t dat){
    uint8_t i;
    ADLX345_SCL_OUT();
    ADLX345_SDA_OUT();
    for (i=0; i<8; i++)     
    {
        delay_us(5);            
        if(dat&0x80)            //送数据口
        {ADLX345_SET_SDA();}
        else
        {ADLX345_CLE_SDA();}       
        delay_us(5);             
        ADLX345_SET_SCL();      //拉高时钟线
        delay_us(5);             
        ADLX345_CLE_SCL();      //拉低时钟线
        dat <<= 1;              //移出数据的最高位
    }
    ADXL345_RecvACK();
}

/**************************************
功能    ：从IIC总线接收一个字节数据
**************************************/
static uint8_t ADXL345_RecvByte(void){
    uint8_t i;
    uint8_t Mid;
    uint8_t dat = 0;
    ADLX345_SDA_INT();
    ADLX345_SCL_OUT();

    for (i=0; i<8; i++)         
    {
        dat <<= 1;
        delay_us(5);          
        ADLX345_SET_SCL();

			if(ADLX345_SDA_VAL()== Bit_SET) //读应答信号
			{
				Mid = 1;
			}
			else
			{
				Mid = 0;
			}
        if(Mid) dat++;
        delay_us(5);     
        ADLX345_CLE_SCL();          //拉低时钟线
    }
    return dat;
}

/********************************************************
功能    ：单字节写入
入口参数：REG_Address (需要写入的地址) REG_data（需要写入的数据）
********************************************************/
static void Single_Write_ADXL345(uint16_t REG_Address,uint8_t REG_data){
    ADXL345_Start();                   //起始信号
    ADXL345_SendByte(SlaveAddress);    //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);     //内部寄存器地址，请参考中文pdf22页 
    ADXL345_SendByte(REG_data);        //内部寄存器数据，请参考中文pdf22页 
    ADXL345_Stop();                    //发送停止信号
}
/********************************************************
功能    ：单字节读取
入口参数：REG_Address (需要读取的地址)
********************************************************/
uint16_t ADLX345_Single_Read(uint16_t REG_Address){ 
		uint16_t REG_data;
		ADXL345_Start();                   //起始信号
		ADXL345_SendByte(SlaveAddress);    //发送设备地址+写信号
		ADXL345_SendByte(REG_Address);     //发送存储单元地址，从0开始	
		ADXL345_Start();                   //起始信号
		ADXL345_SendByte(SlaveAddress+1);  //发送设备地址+读信号
		REG_data=ADXL345_RecvByte();       //读出寄存器数据
		ADXL345_SendACK(1);   
		ADXL345_Stop();                    //停止信号
    return REG_data; 
}
/********************************************************
功能    ：连续读出ADXL345内部加速度数据，地址范围0x32~0x37
********************************************************/
static void Multiple_Read_ADXL345(void){ 
  uint16_t i;
    ADXL345_Start();                   //起始信号
    ADXL345_SendByte(SlaveAddress);    //发送设备地址+写信号
    ADXL345_SendByte(0x32);            //发送存储单元地址，从0x32开始	
    ADXL345_Start();                   //起始信号
    ADXL345_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	 for (i=0; i<6; i++)                 //连续读取6个地址数据，存储中BUF
    {
        BUF[i] = ADXL345_RecvByte();   //BUF[0]存储0x32地址中的数据
        if (i == 5)
        {
           ADXL345_SendACK(1);         //最后一个数据需要回NOACK
        }
        else
        {
          ADXL345_SendACK(0);          //回应ACK
       }
   }
    ADXL345_Stop();                    //停止信号
    delay_ms(5);
}
/********************************************************
***********************外部调用**************************
功能    ：初始化ADXL345，根据需要请参考pdf进行修改
********************************************************/
void ADLX345_Init(void){
   Single_Write_ADXL345(0x31,0x0B);   //测量范围,正负16g，13位模式
   Single_Write_ADXL345(0x2C,0x08);   //速率设定为12.5 参考pdf13页
   Single_Write_ADXL345(0x2D,0x08);   //选择电源模式   参考pdf24页
   Single_Write_ADXL345(0x2E,0x80);   //使能 DATA_READY 中断
   Single_Write_ADXL345(0x1E,0x00);   //X 偏移量 根据测试传感器的状态写入pdf29页
   Single_Write_ADXL345(0x1F,0x00);   //Y 偏移量 根据测试传感器的状态写入pdf29页
   Single_Write_ADXL345(0x20,0x05);   //Z 偏移量 根据测试传感器的状态写入pdf29页
}
/********************************************************
***********************外部调用**************************
功能    ：读取ADXL345x、y、z轴数据
********************************************************/
void ADLX345_ReadData(void){   
  int  dis_data;                       
  Multiple_Read_ADXL345();      //连续读出数据，存储在BUF中
	
  dis_data=(BUF[1]<<8)+BUF[0];  //合成数据   
  Adxl345.temp_X=(float)dis_data*3.9;   //ADXL345快速入门第4页
	
  dis_data=(BUF[3]<<8)+BUF[2];  //合成数据   
  Adxl345.temp_Y=(float)dis_data*3.9;  
	
  dis_data=(BUF[5]<<8)+BUF[4];  //合成数据   
  Adxl345.temp_Z=(float)dis_data*3.9;   
}

