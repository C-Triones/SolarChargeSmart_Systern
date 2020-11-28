#include "./adxl345/bsp_adxl345.h"

static uint8_t BUF[8];    //�������ݻ�����      	
static uint8_t err;       //Ӧ���ź�                    
Adxl345Struct_t Adxl345;
/**************************************
����    ��SCL���ó���ruģʽ
**************************************/
void ADLX345_SCL_Set_Output(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ADLX345_SCL_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(ADLX345_SCL_GPIO, &GPIO_InitStructure);					 					
}	
/**************************************
����    ��SDA���ó����ģʽ
**************************************/
void ADLX345_SDA_Set_Output(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ADLX345_SDA_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(ADLX345_SDA_GPIO, &GPIO_InitStructure);					 					
}	
/**************************************
����    ��SDA���ó�����ģʽ
**************************************/
static void ADLX345_SDA_Set_Input(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ADLX345_SDA_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(ADLX345_SDA_GPIO, &GPIO_InitStructure);					 
}

/**************************************
����    ����ʼ�ź�
**************************************/
static void ADXL345_Start(void){
    ADLX345_SCL_OUT();
    ADLX345_SDA_OUT();
    ADLX345_SET_SDA();//SDA = 1;                    //����������
    ADLX345_SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);               //��ʱ
    ADLX345_CLE_SDA();//SDA = 0;                    //�����½���
    delay_us(2);//Delay5us();                 //��ʱ
    ADLX345_CLE_SCL();//SCL = 0;                    //����ʱ����
}

/**************************************
����    ��ֹͣ�ź�
**************************************/
static void ADXL345_Stop(void){
    ADLX345_SCL_OUT();
    ADLX345_SDA_OUT();
    ADLX345_CLE_SDA();//SDA = 0;                    //����������
    ADLX345_SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);//Delay5us();                 //��ʱ
    ADLX345_SET_SDA();//SDA = 1;                    //����������
    delay_us(2);//Delay5us();                 //��ʱ
    ADLX345_CLE_SCL();
}

/**************************************
����    ������Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
static void ADXL345_SendACK(uint16_t ack){   
    ADLX345_SDA_INT();
    ADLX345_SDA_OUT();
    if(ack==0)//SDA = ack;                  //дӦ���ź�
    {
      ADLX345_CLE_SDA();
    }
    else
    {
      ADLX345_SET_SDA();
    }
    ADLX345_SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);//Delay5us();                 //��ʱ
    ADLX345_CLE_SCL();//SCL = 0;                    //����ʱ����
    delay_us(5);//Delay5us();                 //��ʱ
}

/**************************************
����    ������Ӧ���ź�
**************************************/
static uint8_t ADXL345_RecvACK(void){
    ADLX345_CLE_SDA();
    ADLX345_SCL_OUT();
    ADLX345_SET_SCL();              //����ʱ����
    delay_us(2);
    ADLX345_SET_SCL();
    if(ADLX345_SDA_VAL()== Bit_SET) //��Ӧ���ź�
    {
      err = 1;
    }
    else
    {
      err = 0;
    }
 
    ADLX345_CLE_SCL() ;           //����ʱ����
    delay_us(5); 
    ADLX345_SDA_OUT();
    return err;
}

/**************************************
��IIC���߷���һ���ֽ�����
��ڲ�����dat (��Ҫ���͵�һ�ֽ�����)
**************************************/
static void ADXL345_SendByte(uint8_t dat){
    uint8_t i;
    ADLX345_SCL_OUT();
    ADLX345_SDA_OUT();
    for (i=0; i<8; i++)     
    {
        delay_us(5);            
        if(dat&0x80)            //�����ݿ�
        {ADLX345_SET_SDA();}
        else
        {ADLX345_CLE_SDA();}       
        delay_us(5);             
        ADLX345_SET_SCL();      //����ʱ����
        delay_us(5);             
        ADLX345_CLE_SCL();      //����ʱ����
        dat <<= 1;              //�Ƴ����ݵ����λ
    }
    ADXL345_RecvACK();
}

/**************************************
����    ����IIC���߽���һ���ֽ�����
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

			if(ADLX345_SDA_VAL()== Bit_SET) //��Ӧ���ź�
			{
				Mid = 1;
			}
			else
			{
				Mid = 0;
			}
        if(Mid) dat++;
        delay_us(5);     
        ADLX345_CLE_SCL();          //����ʱ����
    }
    return dat;
}

/********************************************************
����    �����ֽ�д��
��ڲ�����REG_Address (��Ҫд��ĵ�ַ) REG_data����Ҫд������ݣ�
********************************************************/
static void Single_Write_ADXL345(uint16_t REG_Address,uint8_t REG_data){
    ADXL345_Start();                   //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);     //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    ADXL345_SendByte(REG_data);        //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
    ADXL345_Stop();                    //����ֹͣ�ź�
}
/********************************************************
����    �����ֽڶ�ȡ
��ڲ�����REG_Address (��Ҫ��ȡ�ĵ�ַ)
********************************************************/
uint16_t ADLX345_Single_Read(uint16_t REG_Address){ 
		uint16_t REG_data;
		ADXL345_Start();                   //��ʼ�ź�
		ADXL345_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
		ADXL345_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
		ADXL345_Start();                   //��ʼ�ź�
		ADXL345_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
		REG_data=ADXL345_RecvByte();       //�����Ĵ�������
		ADXL345_SendACK(1);   
		ADXL345_Stop();                    //ֹͣ�ź�
    return REG_data; 
}
/********************************************************
����    ����������ADXL345�ڲ����ٶ����ݣ���ַ��Χ0x32~0x37
********************************************************/
static void Multiple_Read_ADXL345(void){ 
  uint16_t i;
    ADXL345_Start();                   //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
    ADXL345_SendByte(0x32);            //���ʹ洢��Ԫ��ַ����0x32��ʼ	
    ADXL345_Start();                   //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	 for (i=0; i<6; i++)                 //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = ADXL345_RecvByte();   //BUF[0]�洢0x32��ַ�е�����
        if (i == 5)
        {
           ADXL345_SendACK(1);         //���һ��������Ҫ��NOACK
        }
        else
        {
          ADXL345_SendACK(0);          //��ӦACK
       }
   }
    ADXL345_Stop();                    //ֹͣ�ź�
    delay_ms(5);
}
/********************************************************
***********************�ⲿ����**************************
����    ����ʼ��ADXL345��������Ҫ��ο�pdf�����޸�
********************************************************/
void ADLX345_Init(void){
   Single_Write_ADXL345(0x31,0x0B);   //������Χ,����16g��13λģʽ
   Single_Write_ADXL345(0x2C,0x08);   //�����趨Ϊ12.5 �ο�pdf13ҳ
   Single_Write_ADXL345(0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
   Single_Write_ADXL345(0x2E,0x80);   //ʹ�� DATA_READY �ж�
   Single_Write_ADXL345(0x1E,0x00);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
   Single_Write_ADXL345(0x1F,0x00);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
   Single_Write_ADXL345(0x20,0x05);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
}
/********************************************************
***********************�ⲿ����**************************
����    ����ȡADXL345x��y��z������
********************************************************/
void ADLX345_ReadData(void){   
  int  dis_data;                       
  Multiple_Read_ADXL345();      //�����������ݣ��洢��BUF��
	
  dis_data=(BUF[1]<<8)+BUF[0];  //�ϳ�����   
  Adxl345.temp_X=(float)dis_data*3.9;   //ADXL345�������ŵ�4ҳ
	
  dis_data=(BUF[3]<<8)+BUF[2];  //�ϳ�����   
  Adxl345.temp_Y=(float)dis_data*3.9;  
	
  dis_data=(BUF[5]<<8)+BUF[4];  //�ϳ�����   
  Adxl345.temp_Z=(float)dis_data*3.9;   
}

