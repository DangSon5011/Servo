/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "lcd_txt.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	uint8_t ready;
	uint8_t brun;
	uint8_t mode;
	char Reset;
}Status;

typedef struct
{
	uint8_t Ts;
	double Kp;
	double Ki;
	double Kd;
}PID_Gain;

typedef struct
{
	double AmMax;
	double AmMin;
	double Period;
	double OffsetTime;
	double bMode;
}pulse;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RXBUFFERSIZE 60
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

double ConvertByte2Double(uint8_t *nbytes)
{
	double *dDouble = (double*)nbytes;
	return *dDouble;
}
int16_t ConverByte2Int16(uint8_t *nbyte)
{
	 int16_t *dInt16 = (int16_t*)nbyte;
	 return *dInt16;
}
int8_t ConverByte2Int8(uint8_t* nbyte)
{
	int8_t *dint8 = (int8_t*)nbyte;
	 return *dint8;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

DAC_HandleTypeDef hdac;

ETH_HandleTypeDef heth;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_uart5_tx;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DAC_Init(void);
static void MX_ETH_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_UART5_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
__IO ITStatus UartReady = RESET;
__IO uint8_t rxUsbBuffer[64]={0};
char txUsbBuffer[64]={0};
char txRs232Buffer[64]={0};
uint8_t len=0 ;
double SetSpeed,SetPosition,Kp1,Ki1,Kd1,Kp2,Ki2,Kd2,Speed,Position,Udk , Udk_P , distance , SetPointcu;
double p = 0, p_pre=0, dp = 0,dpcu=0,dpcu1=0,dpcu2=0,dpcu3=0;
int32_t pp = 0, pp_pre = 0, dpp =0;
double set;
double butpd=2.5,khoangcach=0,viphancu=0;
double error , sum_error = 0, pPart=0, iPart = 0, dPart, pre_error, Ts2, Ts1,OffSetTime;
double err_P, sum_err_P =0, pre_err_P, pPart_P, iPart_P = 0, dPart_P;
unsigned char Down=0, Up=0, Close=0, Enter=0, Alrm=0, Stop=1,Mode=0,Green,Red, COMRun, Run;
uint8_t CTHT1,CTHT2,HOME;
double Udac;
double k1=5,k2=1,J=0.05,alpha=2,theta=0,Speedcu=0,SetPointcu2=0;

int LCD_MODE = 0;
char Buffer[16];
int i=0, itruoc;

int cnttimes , send, ComReset=0,Reset,bReset;
int timeUp;
Status ComStatus;
PID_Gain PID1, PID2;
pulse PulseSpeed;
int LocalControl, ComControl, LCDmode;

int32_t status_f7=0;
uint8_t input_reset=0;
uint8_t input_ctht1=0;
uint8_t input_ctht2=0;
uint8_t input_run=0;
uint8_t input_stop=0;
uint8_t input_mode_speed=0;
uint8_t input_mode_position=0;
uint8_t input_alarm=0;

int screen;
int cursor;
int ok_btn, up_btn, down_btn, back_btn;
int mode_pc_local=0;
int mode_pc_local_temp=0;
int mode_pc_local_irq=0;
extern int8_t PV,SP;
extern int8_t Modee;
extern int8_t Statuss;
extern int8_t Mode_Send;
extern int8_t Status_Send;
extern int8_t Re_set;
extern float Kp_1, Ki_1, Kd_1, Kp_2, Ki_2, Kd_2, Kp_temp, Ki_temp, Kd_temp;
extern int8_t IsPC;
void Reset_servo(void)
{
	   
	    HAL_GPIO_WritePin(SPDLM_GPIO_Port,SPDLM_Pin,GPIO_PIN_SET);
	    HAL_GPIO_WritePin(PL_GPIO_Port,PL_Pin,GPIO_PIN_SET);
		  HAL_GPIO_WritePin(NL_GPIO_Port,NL_Pin,GPIO_PIN_RESET);  
			HAL_GPIO_WritePin(SON_GPIO_Port,SON_Pin,GPIO_PIN_SET);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)(300));
}
void Encoder(void)
{
	 p  =(double)TIM4->CNT;	 
	 TIM4->CNT = 10000;	
	 dp =  p - 10000;
	khoangcach=khoangcach+dp*0.000242456;
	 dp=dpcu*0.3+dp*0.5+dpcu1*0.2;
	 dpcu1=dpcu;
	 dpcu=dp;
	 Speed=dp*0.07;
}
void SetPointSpeed()
{
	if(mode_pc_local==0)
	{
		cnttimes +=10;
	 
	
		 if (PulseSpeed.bMode ==1 )
		 {
				SetSpeed = PulseSpeed.AmMax;
		 }		 
		 
		 if (PulseSpeed.bMode==2 )
		 {
			 if ( 0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*4.9/10)
			 {
				 SetSpeed = PulseSpeed.AmMax;
			 }
			 if ( 1000*PulseSpeed.Period*4.9/10 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*5/10)
			 {
				 SetSpeed =PulseSpeed.AmMax - ( cnttimes - 1000*PulseSpeed.Period*4.9/10)/ ( 1000*PulseSpeed.Period/100)*(PulseSpeed.AmMax-PulseSpeed.AmMin);
			 }
			 if  ( 1000*PulseSpeed.Period*5/10 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*9.9/10)
			 {
				 SetSpeed = PulseSpeed.AmMin;
			 }
			 if ( 1000*PulseSpeed.Period*9.9/10 <= cnttimes && cnttimes < 1000*PulseSpeed.Period)
			 {
				 SetSpeed = PulseSpeed.AmMin + ( cnttimes - 1000*PulseSpeed.Period*9.9/10)/ ( 1000*PulseSpeed.Period/100)*(PulseSpeed.AmMax-PulseSpeed.AmMin);
			 }
//			 SetSpeed = SetPointcu*0.5 + SetSpeed*0.3 + SetPointcu2*0.2;
//			 SetPointcu = SetSpeed;
//			 SetPointcu2 = SetPointcu;
			 if (cnttimes >= 1000*PulseSpeed.Period ) cnttimes =0;
		 }
		 
		 if (PulseSpeed.bMode == 3)
		 {
			 if ( 0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*3/8.0)
			 {
				 SetSpeed = PulseSpeed.AmMax;
			 }
			 if (1000*PulseSpeed.Period*3/8.0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*4/8.0)
			 {
				 //SetSpeed -= (PulseSpeed.AmMax - PulseSpeed.AmMin)/ (1000*PulseSpeed.Period/8)*0.01;
				 SetSpeed = PulseSpeed.AmMax - ( cnttimes - 1000*PulseSpeed.Period*3/8.0 )/ ( 1000*PulseSpeed.Period/8.0)*(PulseSpeed.AmMax-PulseSpeed.AmMin);
			 }
			 if (1000*PulseSpeed.Period*4/8.0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*7/8.0)
			 {
				 SetSpeed = PulseSpeed.AmMin;
			 }
			 if (1000*PulseSpeed.Period*7/8.0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*8/8.0)
			 {
				 
				 SetSpeed = PulseSpeed.AmMin + ( cnttimes - 1000*PulseSpeed.Period*7/8.0 )/ ( 1000*PulseSpeed.Period/8.0)*(PulseSpeed.AmMax-PulseSpeed.AmMin);
			 }
			 if (cnttimes >= 1000*PulseSpeed.Period) cnttimes=0;
			 
		 }
	}
}
void acb();
void ReadBuffer(void)
{
	    ComStatus.brun = rxUsbBuffer[0];
			ComStatus.mode = rxUsbBuffer[1];
				
			//PID1
			PID1.Ts= rxUsbBuffer[2];
			PID2.Ts=rxUsbBuffer[27];
	
			uint8_t nbytes[8] = {0,0,0,0,0,0,0,0};
			int i;
			
			for(i=0;i<8;i++) nbytes[i] = rxUsbBuffer[3+i];
			PID1.Kp = ConvertByte2Double(nbytes);
			
			for(i=0;i<8;i++) nbytes[i] = rxUsbBuffer[11+i];
			PID1.Ki = ConvertByte2Double(nbytes);
			
			for(i=0;i<8;i++) nbytes[i] = rxUsbBuffer[19+i];
			PID1.Kd = ConvertByte2Double(nbytes);
			//PID2
			for(i=0;i<8;i++) nbytes[i] = rxUsbBuffer[28+i];
			PID2.Kp = ConvertByte2Double(nbytes);
			
			for(i=0;i<8;i++) nbytes[i] = rxUsbBuffer[36+i];
			PID2.Ki = ConvertByte2Double(nbytes);
			
			for(i=0;i<8;i++) nbytes[i] = rxUsbBuffer[44+i];
			PID2.Kd = ConvertByte2Double(nbytes);
			
			
			for(i=0;i<8;i++) nbytes[i] = rxUsbBuffer[52+i];
		
			// SetSpeed = ConvertByte2Double(nbytes);
			PulseSpeed.AmMax = (double)ConverByte2Int16(nbytes);
			PulseSpeed.AmMin = (double)ConverByte2Int16(nbytes+2);
			PulseSpeed.Period= (double)ConverByte2Int16(nbytes +4);
			PulseSpeed.bMode = nbytes[6];
			PulseSpeed.OffsetTime = (double)nbytes[7];
			
			ComStatus.Reset = (char)nbytes[8];
		
			
			Kp1 = PID1.Kp;    Ki1 = PID1.Ki;   Kd1 = PID1.Kd;	
			Kp2 = PID2.Kp;    Ki2 = PID2.Ki;   Kd2 = PID2.Kd;


			Ts1=(double)PID1.Ts; Ts2=(double)PID2.Ts;
			OffSetTime = 1000*PulseSpeed.OffsetTime ;
			ComReset = ComStatus.Reset;
			//bReset = ComReset;
			
			if (ComStatus.brun == 1) 	
			{	
				COMRun = 1; 
//				LocalControl = 0;
//				ComControl =1;
				//Green = 0 ;
				
			}
			else   
			{ 
				COMRun = 0 ;
//				ComControl =0;
				//Red = 0;
			}
}

void thuan ()
	
{
			//Udac = Udk;
			HAL_GPIO_WritePin(SPDLM_GPIO_Port,SPDLM_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PL_GPIO_Port,PL_Pin,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(NL_GPIO_Port,NL_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SON_GPIO_Port,SON_Pin,GPIO_PIN_SET);
}
void nguoc()
{
	    HAL_GPIO_WritePin(SPDLM_GPIO_Port,SPDLM_Pin,GPIO_PIN_SET);
	    HAL_GPIO_WritePin(PL_GPIO_Port,PL_Pin,GPIO_PIN_SET);
		  HAL_GPIO_WritePin(NL_GPIO_Port,NL_Pin,GPIO_PIN_RESET);  
			HAL_GPIO_WritePin(SON_GPIO_Port,SON_Pin,GPIO_PIN_SET);
}

void MotorStop ()
{
	  HAL_GPIO_WritePin(SON_GPIO_Port,SON_Pin,GPIO_PIN_RESET);
		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)(0)); 
}
void UartTransmit(void)
{
	len = sprintf((char *)txUsbBuffer,"%ld,%ld,%ld,%i,%i,%i,%i,%i,%ld,%ld\r\n",(long)SetSpeed,(long)Speed,(long)Udk,status_f7,CTHT1,CTHT2,Green,Red,(long)SetPosition,(long)khoangcach);
	//len = sprintf((char *)txUsbBuffer,"%ld,%ld,%ld,%i,%i,%i,%i,%i,%ld,%ld\r\n",(long)SetSpeed,(long)(khoangcach/300.0*15.5),(long)Udk,Run,CTHT1,CTHT2,Green,Red,(long)SetPosition,(long)Position);
	//len = sprintf((char *)txUsbBuffer,"%f,%f,%f,%d,%d,%d,%d,%d\r\n",SetSpeed,Speed,Udk,Run,CTHT1,CTHT2,Green,Red);
	txRs232Buffer[0]='A';
	txRs232Buffer[1]= (char)',';
	for (int i=0;i<=len;i++)
	{
		txRs232Buffer[i+2]= txUsbBuffer[i];
	}
	
	HAL_UART_Transmit_DMA(&huart5,(uint8_t*)txRs232Buffer,len+2);
}

void Limit(void)
{
			CTHT2 = HAL_GPIO_ReadPin(CTHT2_GPIO_Port,CTHT2_Pin);
			CTHT1 = HAL_GPIO_ReadPin(CTHT1_GPIO_Port,CTHT1_Pin);
			
			if (CTHT1 == 1 && (ComReset == 1 || Reset == 1) )
			{
				//Position = 0;
				bReset = 0;
				Red = 0;
				HAL_GPIO_WritePin(SON_GPIO_Port,SON_Pin,GPIO_PIN_RESET);	
        SetSpeed=0;				
				Udac = 0;
				iPart=0;
				dPart=0;
				khoangcach =0;		
			}
			
			if (CTHT1 == 1 && SetSpeed < 0  )
			{
				Run = 0;
				HAL_GPIO_WritePin(SON_GPIO_Port,SON_Pin,GPIO_PIN_RESET);		
				Udac = 0;
				iPart=0;
				dPart=0;
				
			}
			
			else if (CTHT2 == 1 && SetSpeed > 0 )
			{
				Run = 0;
				HAL_GPIO_WritePin(SON_GPIO_Port,SON_Pin,GPIO_PIN_RESET);
				Udac=0;
				iPart=0;
				dPart=0;
			}
			
			else
			{
				Run =1;
			}

}
void SetPointPosition()
{
	if(mode_pc_local==0)
	{
		if (PulseSpeed.bMode ==1 )
	 {
			SetPosition= PulseSpeed.AmMax;
	 }		 
	 
	 if (PulseSpeed.bMode==2 )
	 {
		 if ( 0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period/2)
		 {
			 SetPosition = PulseSpeed.AmMax;
		 }
		 if ( 1000*PulseSpeed.Period / 2 <= cnttimes && cnttimes < 1000*PulseSpeed.Period)
		 {
			 SetPosition = PulseSpeed.AmMin;
		 }
		 cnttimes +=10;
		 if (cnttimes >= 1000*PulseSpeed.Period ) cnttimes =0;
	 }
	 
	 if (PulseSpeed.bMode == 3)
	 {
		 if ( 0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*3/8.0)
		 {
			 SetPosition = PulseSpeed.AmMax;
		 }
		 if (1000*PulseSpeed.Period*3/8.0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*4/8.0)
		 {
			 //SetSpeed -= (PulseSpeed.AmMax - PulseSpeed.AmMin)/ (1000*PulseSpeed.Period/8)*0.01;
			 SetPosition = PulseSpeed.AmMax - ( cnttimes - 1000*PulseSpeed.Period*3/8.0 )/ ( 1000*PulseSpeed.Period/8.0)*(PulseSpeed.AmMax-PulseSpeed.AmMin);
		 }
		 if (1000*PulseSpeed.Period*4/8.0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*7/8.0)
		 {
			 SetPosition = PulseSpeed.AmMin;
		 }
		 if (1000*PulseSpeed.Period*7/8.0 <= cnttimes && cnttimes < 1000*PulseSpeed.Period*8/8.0)
		 {
			 
			 SetPosition = PulseSpeed.AmMin + ( cnttimes - 1000*PulseSpeed.Period*7/8.0 )/ ( 1000*PulseSpeed.Period/8.0)*(PulseSpeed.AmMax-PulseSpeed.AmMin);
		 }
		 
		cnttimes +=10;
		 if (cnttimes >= 1000*PulseSpeed.Period) cnttimes=0;
	 }
	}
}
void PID_Position(void)
{
	
   error  = SetPosition - khoangcach;	 
	 pPart_P  = Kp1*error;	
	
	 iPart_P +=  Ki1*error*0.01;
	

	 dPart_P  = Kd1/0.01*(error - pre_error);
		
	 pre_error = error;	
	
	Udk    = pPart_P + iPart_P + dPart_P;
	 if (Udk > 4095.0) Udk = 4095.0;	
	 if (Udk < -4095.0) Udk = -4095.0;
	 if (Udk>=0) 
		{
			Udac = Udk;
			thuan();
		}			
		if (Udk <0) 
		{
			Udac = -Udk;
			nguoc();
		}
		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)(Udac));
}


void PID_Speed(void)
{
	
	 //Speed  = (double)dp*0.4861;  //   7* (pulse / (360*4))/0.01 cm/s
   error  = SetSpeed - Speed;	 
	
	 pPart  = Kp2*error;	
	
	 iPart +=  Ki2*error*0.01;
	
//	 if ( iPart > 4095.0)
//		{
//			iPart = 4095;
//		}
//	 if ( iPart < -4095.0)
//		{
//			iPart = -4095;
//		}
	 dPart  = Kd2/0.01*(error - pre_error);
		dPart=viphancu*0.3+dPart*0.7;
		viphancu=dPart;
	 pre_error = error;
	 Udk    = pPart + iPart + dPart;
	 if (Udk > 4095.0) Udk = 4095.0;	
	 if (Udk < -4095.0) Udk = -4095.0;
	 if (Udk>=0) 
		{
			Udac = Udk;
			thuan();
		}			
		if (Udk <0) 
		{
			Udac = -Udk;
			nguoc();
		}
		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)(Udac));
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_10)
	{
		if(up_btn==0 && ok_btn==0 && back_btn==0)
		{
			down_btn = 1;
		}
	}
	
	if(GPIO_Pin == GPIO_PIN_11)
	{
		if(down_btn==0 && ok_btn==0 && back_btn==0)
		{
			up_btn = 1;
		}
	}
		
	if (GPIO_Pin == GPIO_PIN_14)  // Swtich Enter
	{
		if(down_btn==0 && up_btn==0 && back_btn==0)
		{
			ok_btn = 1;
		}
	}
	if (GPIO_Pin == GPIO_PIN_15)  // Switch Close
	{
		if(down_btn==0 && ok_btn==0 && up_btn==0)
		{
			back_btn = 1;
		}
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	if (htim->Instance == htim6.Instance)
	{ 		  
		
		acb();
		
		if (send <5 ) send++;			
		if (send >=5)
		{
		UartTransmit();	
		send = 0;
		}
	} 
 
		
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Set transmission flag: transfer complete */
    UartReady = SET;
  //HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//	lcd_init();
//	lcd_puts(0,0,(int8_t*)"Servo: OFF");
//	lcd_puts(1,0,(int8_t*)"Mode: Speed");
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
//		lcd_init();
//		lcd_clear();
//		lcd_puts( 1, 0, (int8_t*)"AmMax: ");
//		lcd_puts( 2, 0, (int8_t*)"AmMin: ");
//		lcd_puts( 3, 0, (int8_t*)"Peroid: ");
  /* USER CODE END SysInit */
	ComStatus.brun=2;
	ComStatus.mode=0;
	ComStatus.Reset=0;
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DAC_Init();
  MX_ETH_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(SPD0_GPIO_Port,SPD0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPD1_GPIO_Port,SPD1_Pin,GPIO_PIN_RESET);
	HAL_UART_Receive_DMA(&huart5,(uint8_t *)rxUsbBuffer,RXBUFFERSIZE);
  HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	
	
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1|TIM_CHANNEL_2);	
	TIM3->CNT = 10000; TIM4->CNT = 10000;
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1|TIM_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim6);
	iPart = 0; dPart = 0;
	status_f7=1;
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
		lcd_init();
		lcd_clear();
		int lcd_count = 0;
		screen =1;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		mode_pc_local_temp = mode_pc_local;
		mode_pc_local=HAL_GPIO_ReadPin(STATUS_GPIO_Port,STATUS_Pin);
		if(ComStatus.ready==1)
		{
			ComStatus.ready=0;		
			ReadBuffer();		
		}
		if(lcd_count++>200){
			lcd_count =0;
			if ( Modee == 1 && screen == 2)
			{
				PV = khoangcach;
				Update_Main();
			}
			else if( Modee == 2 && screen == 2)
			{
				PV = Speed;
				Update_Main();
			}
		}
		if (UartReady == SET && mode_pc_local==0)
		{
			UartReady = RESET;
			ComStatus.ready = 1;
			HAL_UART_Receive_DMA(&huart5, (uint8_t *)rxUsbBuffer, RXBUFFERSIZE);
		}
		if(mode_pc_local == 1 && mode_pc_local_temp == 0)
		{
			Kp_1 = Kp1;
			Ki_1 = Ki1;
			Kd_1 = Kd1;
			Kp_2 = Kp2;
			Ki_2 = Ki2;
			Kd_2 = Kd2;
			IsPC=0;
		}
		
		switch(screen){
			case 1: PC_Screen();
			break;
			case 2: Main_Screen();
			break;
			case 3: Setting_Screen();
			break;
			case 4: PID_Screen();
			break;
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART5
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization 
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config 
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT2 config 
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   uint8_t MACAddr[6] ;

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  heth.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  heth.Init.PhyAddress = LAN8742A_PHY_ADDRESS;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.RxMode = ETH_RXPOLLING_MODE;
  heth.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  heth.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

  /* USER CODE BEGIN MACADDRESS */
    
  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xFFFF;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0xFFFF;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 47999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 19;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 9599;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 9999;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPD0_GPIO_Port, SPD0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPDLM_GPIO_Port, SPDLM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PL_GPIO_Port, PL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EMGS_Pin|SON_Pin|ARST_Pin|LD3_Pin 
                          |SPD1_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STATUS_GPIO_Port, STATUS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, NL_Pin|RS_Pin|D4_Pin|D5_Pin 
                          |D6_Pin|D7_Pin|D8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, RW_Pin|EN_Pin|USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ALRM_Pin */
  GPIO_InitStruct.Pin = ALRM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ALRM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BRKR_Pin TSPD_Pin SRDY_Pin ZSPD_Pin */
  GPIO_InitStruct.Pin = BRKR_Pin|TSPD_Pin|SRDY_Pin|ZSPD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : SPD0_Pin */
  GPIO_InitStruct.Pin = SPD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPD0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPDLM_Pin */
  GPIO_InitStruct.Pin = SPDLM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPDLM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PL_Pin */
  GPIO_InitStruct.Pin = PL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(PL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : EMGS_Pin SON_Pin ARST_Pin SPD1_Pin */
  GPIO_InitStruct.Pin = EMGS_Pin|SON_Pin|ARST_Pin|SPD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : CTHT1_Pin CTHT2_Pin */
  GPIO_InitStruct.Pin = CTHT1_Pin|CTHT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : STATUS_Pin */
  GPIO_InitStruct.Pin = STATUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(STATUS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ENTER_Pin CLOSE_Pin */
  GPIO_InitStruct.Pin = ENTER_Pin|CLOSE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : DOWN_Pin UP_Pin */
  GPIO_InitStruct.Pin = DOWN_Pin|UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : NL_Pin RS_Pin D4_Pin D5_Pin 
                           D6_Pin D7_Pin D8_Pin */
  GPIO_InitStruct.Pin = NL_Pin|RS_Pin|D4_Pin|D5_Pin 
                          |D6_Pin|D7_Pin|D8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : RW_Pin EN_Pin */
  GPIO_InitStruct.Pin = RW_Pin|EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : HOME_Pin */
  GPIO_InitStruct.Pin = HOME_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(HOME_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void acb()
{
	__disable_irq();
	//Update input
	if(mode_pc_local==0)
	{
		if(mode_pc_local_irq==1)
		{
			screen = 1;
			IsPC = 1;
			status_f7=1;
			input_stop=0;
			input_run=0;
			input_mode_speed=0;
			input_mode_position=0;
		}
		else
		{
			input_reset = ComStatus.Reset;
	CTHT1= HAL_GPIO_ReadPin(CTHT1_GPIO_Port,CTHT1_Pin);
	input_ctht1 = CTHT1;
	CTHT2= HAL_GPIO_ReadPin(CTHT2_GPIO_Port,CTHT2_Pin);
	input_ctht2 = CTHT2;
	if(ComStatus.brun==2)
	{
		input_run = 0;
		input_stop = 0;
	}
	if(ComStatus.brun==1)
	{
		input_run = 1;
	}
	if(ComStatus.brun==0)
	{
		input_stop = 1;
	}
	if (ComStatus.mode)
	{
		input_mode_speed = 1;
		input_mode_position=0;
	}
	else 
	{
		input_mode_speed = 0;
		input_mode_position=1;
	}
	 input_alarm = 0;
	ComStatus.Reset=0;
	ComStatus.brun=2;
		}
	mode_pc_local_irq=0;
	}
	if(mode_pc_local==1)
	{
		if(mode_pc_local_irq==0)
		{
			status_f7=1;
			input_stop=0;
			input_run=0;
			input_mode_speed=0;
			input_mode_position=0;
			Modee=1;
			Statuss=2;
		}
		else
		{
			input_reset=Re_set;
		CTHT1= HAL_GPIO_ReadPin(CTHT1_GPIO_Port,CTHT1_Pin);
		input_ctht1 = CTHT1;
		CTHT2= HAL_GPIO_ReadPin(CTHT2_GPIO_Port,CTHT2_Pin);
		input_ctht2 = CTHT2;
		if(Status_Send==20) 
		{
			input_stop=0;input_run=0;
		}
		if(Status_Send==0) input_stop=1;
		if(Status_Send==1) input_run=1;
		if(Modee==2) 
		{
			input_mode_position=0;
			input_mode_speed=1;
		}
		if(Modee==1) 
		{
			input_mode_position=1;
			input_mode_speed=0;
		}
		input_alarm = 0;
		Status_Send=20;
		Re_set=0;
		}
		mode_pc_local_irq=1;
	}
	__enable_irq();
	if(status_f7==1)  //Init
	{
		Statuss = 2;
		iPart=0;iPart_P=0;
		if(input_alarm==1) status_f7=6;
		if(input_alarm==0&&input_reset==1) status_f7=2;
	}
	else if(status_f7==2)  //RESET
	{
		//Dieu khien dong co RWD
		Statuss = Stopp;
		iPart=0;iPart_P=0;
		 nguoc();
		 HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)(700)); 			
		//input_reset=0;
		if(input_alarm==1) status_f7=6;
		if(input_alarm==0&&input_ctht1==1) 
		{
			for(int i=0;i<99;i++) for(int j=0;j<99999;j++);
			//Reset khoang cach ve 0
			khoangcach=0;
			TIM4->CNT = 10000;	
			status_f7=3;
		}
	}
	else if(status_f7==3) //IDLE
	{
		//Cap nhat gia tri khoang cach, toc do
		Encoder();
		//Stop
		MotorStop();
		iPart=0;iPart_P=0;
		Statuss = Stopp;
		//input_stop=0;
		if(input_alarm==1) status_f7=6;
		if(input_alarm==0&&input_reset==1) status_f7=2;
		if(input_alarm==0&&input_reset==0&&input_run==1&&input_stop==0) status_f7=4;
	}
	else if(status_f7==4) //Control
	{
		//Cap nhat gia tri khoang cach, toc do
		Encoder();
		Statuss = Runn;
		//input_run=0;
		if(input_alarm==1) status_f7=6;
		if(input_alarm==0&&input_reset==1) status_f7=2;
		if(input_alarm==0&&input_reset==0&&input_stop==1) status_f7=3;
		if(input_alarm==0&&input_reset==0&&input_stop==0&&input_mode_speed==1) 
		{
			status_f7=41;
			iPart=0;
		}
		if(input_alarm==0&&input_reset==0&&input_stop==0&&input_mode_position==1) 
		{
			status_f7=42;
			iPart_P=0;
		}
	}
	else if(status_f7==41) //Control Speed
	{
		//Cap nhat gia tri khoang cach, toc do
		Encoder();
		Statuss=Runn;
		//Dieu khien toc do
		SetPointSpeed();
		PID_Speed();
		//input_mode_speed=0;
		if(input_alarm==1) status_f7=6;
		if(input_alarm==0&&input_reset==1) status_f7=2;
		if(input_alarm==0&&input_reset==0&&input_stop==1) status_f7=3;
		if(input_alarm==0&&input_reset==0&&input_stop==0)
		{
			if(input_ctht1==1&&SetSpeed<0) status_f7=5;
			if(input_ctht2==1&&SetSpeed>0) status_f7=5;
		}
		if(input_alarm==0&&input_reset==0&&input_stop==0&&input_mode_position==1&&input_mode_speed==0) 
		{
			status_f7=42;
			iPart_P=0;
		}
	}
	else if(status_f7==42) //Control Position
	{
		//Cap nhat gia tri khoang cach, toc do
		Encoder();
		Statuss=Runn;
		//Dieu khien vi tri
		SetPointPosition();
		PID_Position();
		//input_mode_position=0;
		if(input_alarm==1) status_f7=6;
		if(input_alarm==0&&input_reset==1) status_f7=2;
		if(input_alarm==0&&input_reset==0&&input_stop==1) status_f7=3;
		if(input_alarm==0&&input_reset==0&&input_stop==0&&input_mode_speed==1&&input_mode_position==0) 
		{
			status_f7=41;
			iPart=0;
		}
	}
	else if(status_f7==5) //Pause, Limit
	{
		//Cap nhat gia tri khoang cach, toc do
		Encoder();
		//Stop
		Statuss=Stopp;
		MotorStop();
		SetPointSpeed();
		iPart=0;iPart_P=0;
		if(input_alarm==1) status_f7=6;
		if(input_alarm==0&&input_reset==1) status_f7=2;
		if(input_alarm==0&&input_reset==0&&input_stop==1) status_f7=3;
		if(input_alarm==0&&input_reset==0&&input_stop==0&&input_mode_position==1) status_f7=42;
		if(input_alarm==0&&input_reset==0&&input_stop==0&&input_mode_speed==1) 
		{
			if(input_ctht1 == 1 && SetSpeed < 0) status_f7=5;
			else if (input_ctht2 == 1 && SetSpeed > 0 ) status_f7=5;
			else status_f7=41;
		}
	}
	else if(status_f7==6)  //Drive Error
	{
		//Stop
		MotorStop();
		Statuss=Stopp;
		if(input_alarm==0) status_f7=1;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
