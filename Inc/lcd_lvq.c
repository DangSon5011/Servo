#include "lcd_txt.h"
#include <string.h>
#include "main.h"
uint8_t pos_Addr;
uint8_t pos_Addr_temp;
char line[6]="";
extern int screen;
extern int cursor;
extern int ok_btn, up_btn, down_btn, back_btn;
int8_t IsPC = 0;
int8_t Mode;
int8_t Status;
int8_t PV;
int8_t SP;
int8_t Reset = 0;
uint16_t Kp1_a, Kd1_a, Ki1_a;
uint8_t Kp1_b, Kd1_b, Ki1_b;
float Kp1, Ki1, Kd1, Kp2, Ki2, Kd2, Kp_temp, Ki_temp, Kd_temp;
int sign_Kp1,sign_Ki1,sign_Kd1;
int PID_Choose = 1;
/*--------------- Initialize LCD ------------------*/
void lcd_init(void)
{
	
	HAL_Delay(30);
	
	PIN_LOW(D4_PORT,D4_PIN);
	PIN_HIGH(D5_PORT,D5_PIN);
	PIN_LOW(D6_PORT,D6_PIN);
	PIN_LOW(D7_PORT,D7_PIN);
	PIN_LOW(RS_PORT,RS_PIN);
	
	PIN_HIGH(EN_PORT,EN_PIN);
	PIN_LOW(EN_PORT,EN_PIN);
	
	lcd_write(0,0x28);
	lcd_write(0,0x0c);
	lcd_write(0,0x06);
	lcd_write(0,0x01);
}

/*--------------- Write To LCD ---------------*/
void lcd_write(uint8_t type,uint8_t data)
{
	HAL_Delay(1);
	if(type)
	{
		PIN_HIGH(RS_PORT,RS_PIN);
	}else
	{
		PIN_LOW(RS_PORT,RS_PIN);
	}
	
	//Send High Nibble
	if(data&0x80)
	{
		PIN_HIGH(D7_PORT,D7_PIN);
	}else
	{
		PIN_LOW(D7_PORT,D7_PIN);
	}
	
	if(data&0x40)
	{
		PIN_HIGH(D6_PORT,D6_PIN);
	}else
	{
		PIN_LOW(D6_PORT,D6_PIN);
	}
	
	if(data&0x20)
	{
		PIN_HIGH(D5_PORT,D5_PIN);
	}else
	{
		PIN_LOW(D5_PORT,D5_PIN);
	}
	
	if(data&0x10)
	{
		PIN_HIGH(D4_PORT,D4_PIN);
	}else
	{
		PIN_LOW(D4_PORT,D4_PIN);
	}
	PIN_HIGH(EN_PORT,EN_PIN);
	PIN_LOW(EN_PORT,EN_PIN);
	

	//Send Low Nibble
	if(data&0x08)
	{
		PIN_HIGH(D7_PORT,D7_PIN);
	}else
	{
		PIN_LOW(D7_PORT,D7_PIN);
	}
	
	if(data&0x04)
	{
		PIN_HIGH(D6_PORT,D6_PIN);
	}else
	{
		PIN_LOW(D6_PORT,D6_PIN);
	}
	
	if(data&0x02)
	{
		PIN_HIGH(D5_PORT,D5_PIN);
	}else
	{
		PIN_LOW(D5_PORT,D5_PIN);
	}
	
	if(data&0x01)
	{
		PIN_HIGH(D4_PORT,D4_PIN);
	}else
	{
		PIN_LOW(D4_PORT,D4_PIN);
	}
	PIN_HIGH(EN_PORT,EN_PIN);
	PIN_LOW(EN_PORT,EN_PIN);
}

void lcd_puts(uint8_t x, uint8_t y, int8_t *string)
{
	//Set Cursor Position
	#ifdef LCD16xN	//For LCD16x2 or LCD16x4
	switch(x)
	{
		case 0: //Row 0
			lcd_write(0,0x80+0x00+y);		
			break;
		case 1: //Row 1
			lcd_write(0,0x80+0x40+y);
			break;
		case 2: //Row 2
			lcd_write(0,0x80+0x10+y);
			break;
		case 3: //Row 3
			lcd_write(0,0x80+0x50+y);
			break;
	}
	#endif
	
	#ifdef LCD20xN	//For LCD20x4
	switch(x)
	{
		case 0: //Row 0
			lcd_write(0,0x80+0x00+y);
			pos_Addr = 0x80+0x00+y;
			break;
		case 1: //Row 1
			lcd_write(0,0x80+0x40+y);
			pos_Addr = 0x80+0x40+y;
			break;
		case 2: //Row 2
			lcd_write(0,0x80+0x14+y);
			pos_Addr = 0x80+0x14+y;
			break;
		case 3: //Row 3
			lcd_write(0,0x80+0x54+y);
			pos_Addr = 0x80+0x54+y;
			break;
	}
	#endif
	
	while(*string)
	{
		lcd_write(1,*string);
		string++;
	}
}

void lcd_go_to(uint8_t x, uint8_t y)
{
		switch(x)
	{
		case 0: //Row 0
			lcd_write(0,0x80+0x00+y);
			pos_Addr = 0x80+0x00+y;
			break;
		case 1: //Row 1
			lcd_write(0,0x80+0x40+y);
			pos_Addr = 0x80+0x40+y;
			break;
		case 2: //Row 2
			lcd_write(0,0x80+0x10+y);
			pos_Addr = 0x80+0x10+y;
			break;
		case 3: //Row 3
			lcd_write(0,0x80+0x50+y);
			pos_Addr = 0x80+0x50+y;
			break;
	}
}

void lcd_clear(void)
{
	lcd_write(0,0x01);
}

void lcd_cursor_on(void){
	lcd_write(0,0x0D);
	cursor = 1;
}	

void lcd_cursor_off(void){
	lcd_write(0,0x0C);
	cursor = 0;
}

void lcd_display_off(void){
	lcd_write(0,0x08);
}	

void lcd_display_on(void){
	lcd_write(0,0x0C);
}

void Update_Main(void){
	pos_Addr_temp = pos_Addr;
	if(Mode == Position) lcd_puts(0,0,(int8_t*)"Mode: Position"); else lcd_puts(0,0,(int8_t*)"Mode: Velocity");
	if(Status == Run) lcd_puts(1,0,(int8_t*)"Status: Running"); else lcd_puts(3,0,(int8_t*)"Status: Stop");
	sprintf(line,"%3d",SP);
	lcd_puts(2,4,(int8_t*)line);
	sprintf(line,"%3d",PV);
	lcd_puts(3,4,(int8_t*)line);
	lcd_write(0,pos_Addr_temp);
}

void PC_Screen(void){
	lcd_clear();
	lcd_puts(1,10,(int8_t*)"PC_Mode");
	if(IsPC==0)
	{
		lcd_clear();
		if(Mode == Position) lcd_puts(0,0,(int8_t*)"Mode: Position"); else lcd_puts(0,0,(int8_t*)"Mode: Velocity");
		if(Status == Run) lcd_puts(1,0,(int8_t*)"Status: Running"); else lcd_puts(1,0,(int8_t*)"Status: Stop");
		lcd_puts(2,0,(int8_t*)"SP: ");
		lcd_puts(3,0,(int8_t*)"PV: ");
		lcd_cursor_on();
		lcd_go_to(1,0);
		screen = 2;
	}
	if(ok_btn){
		HAL_Delay(100);
		ok_btn = 0;
	}
	if(up_btn){
		HAL_Delay(100);
		up_btn = 0;
	}
	if(down_btn){
		HAL_Delay(100);
		down_btn = 0;
	}
	if(back_btn){
		HAL_Delay(100);
		back_btn = 0;
	}
}
void Main_Screen(void){
	if(ok_btn){
		HAL_Delay(100);
		ok_btn = 0;
		//
		switch(pos_Addr){
			case 0x80+0x40:
			if(Mode == Stop){
				Mode = Run;
				lcd_puts(1,0,(int8_t*)"Status: Running");
				lcd_go_to(1,0);
			}
			else {
				Mode = Stop;
				lcd_puts(1,0,(int8_t*)"Status: Stop   ");
				lcd_go_to(1,0);
			}
			break;
			case 0x80+0x10:
				lcd_go_to(2,6);
			break;
			case 0x80+0x10+6:
				lcd_go_to(2,0);
			break;
		}
	}
	if(up_btn){
		HAL_Delay(100);
		up_btn = 0;
		//
		switch(pos_Addr){
			case 0x80+0x10:
				lcd_go_to(1,0);
			break;
			//Tang SP
			case 0x80+0x10+6:
				SP++;
				sprintf(line,"SP: %3d", SP);
				lcd_puts(2,0,(int8_t*)line);
				lcd_go_to(2,6);
			break;			
		}
	}
	if(down_btn){
		HAL_Delay(100);
		down_btn = 0;	
		//
		switch(pos_Addr){
			case 0x80+0x40:
				lcd_go_to(2,0);
			break;
			//Giam SP
			case 0x80+0x10+6:
				SP--;
				sprintf(line,"SP: %3d", SP);
				lcd_puts(2,0,(int8_t*)line);
				lcd_go_to(2,6);
			break;	
			
		}	
	}
	if(back_btn){
		HAL_Delay(100);
		back_btn = 0;
		if(pos_Addr == 0x80+0x10+6) lcd_go_to(2,0);
		else
		{
		lcd_clear();
		if(Mode == Velocity) lcd_puts(0,0,(int8_t*)" Mode: Velocity");
		else lcd_puts(0,0,(int8_t*)" Mode: Position");
		lcd_puts(1,0,(int8_t*)" Set PID1: ");
		lcd_puts(2,0,(int8_t*)" Set PID2: ");
		lcd_puts(3,0,(int8_t*)" Reset: ");
		lcd_go_to(0,0);
		lcd_cursor_on();
		screen = 3;
		}
}

void Setting_Screen(void){
	if(ok_btn){
		HAL_Delay(100);
		ok_btn = 0;
		//
		switch(pos_Addr){
			case 0x80+0x00:
				if(Mode == Velocity) {
					lcd_puts(0,0,(int8_t*)" Mode: Position");
					Mode = Position;
					lcd_go_to(0,0);
				}
				else {
					lcd_puts(0,0,(int8_t*)" Mode: Velocity");
					Mode = Velocity;
					lcd_go_to(0,0);
				}
			break;
			case 0x80+0x40:
				PID_Choose = 1;
				lcd_clear();
				if(Kp1<0) sign_Kp1 = -1; else sign_Kp1 = 1;
				if(Ki1<0) sign_Ki1 = -1; else sign_Ki1 = 1;
				if(Kd1<0) sign_Kd1 = -1; else sign_Kd1 = 1;
				Kp1_a = (int)sign_Kp1*Kp1;
				Kp1_b = sign_Kp1*Kp1*100-Kp1_a*100;
				Ki1_a = (int)sign_Ki1*Ki1;
				Ki1_b = sign_Ki1*Ki1*100-Ki1_a*100;
				Kd1_a = (int)sign_Kd1*Kd1;
				Kd1_b = sign_Kd1*Kd1*100-Kd1_a*100;
				lcd_puts(0,0,(int8_t*)"  PID Position  ");
				if(sign_Kp1 == -1) lcd_puts(1,0,(int8_t*)" Kp1: -"); else lcd_puts(1,0,(int8_t*)" Kp1: +");
				sprintf(line,"%3d,%2d", Kp1_a, Kp1_b);
				lcd_puts(1,7,(int8_t*)line);
				if(sign_Ki1 == -1) lcd_puts(2,0,(int8_t*)" Ki1: -"); else lcd_puts(2,0,(int8_t*)" Ki1: +");
				sprintf(line,"%3d,%2d", Ki1_a, Ki1_b);
				lcd_puts(2,7,(int8_t*)line);
				if(sign_Kd1 == -1) lcd_puts(3,0,(int8_t*)" Kd1: -"); else lcd_puts(3,0,(int8_t*)" Kd1: +");
				sprintf(line,"%3d,%2d", Kd1_a, Kd1_b);
				lcd_puts(3,7,(int8_t*)line);
				lcd_go_to(1,0);
				lcd_cursor_on();
				screen = 4;
			break;
			case 0x80+0x10:
				PID_Choose = 2;
				lcd_clear();
				Kp_temp = Kp1;
				Ki_temp = Ki1;
				Kd_temp = Kd1;
				Kp1 = Kp2;
				Ki1 = Ki2;
				Kd1 = Kd2;
				if(Kp1<0) sign_Kp1 = -1; else sign_Kp1 = 1;
				if(Ki1<0) sign_Ki1 = -1; else sign_Ki1 = 1;
				if(Kd1<0) sign_Kd1 = -1; else sign_Kd1 = 1;
				Kp1_a = (int)sign_Kp1*Kp1;
				Kp1_b = sign_Kp1*Kp1*100-Kp1_a*100;
				Ki1_a = (int)sign_Ki1*Ki1;
				Ki1_b = sign_Ki1*Ki1*100-Ki1_a*100;
				Kd1_a = (int)sign_Kd1*Kd1;
				Kd1_b = sign_Kd1*Kd1*100-Kd1_a*100;
				lcd_puts(0,0,(int8_t*)"  PID Velocity  ");
				if(sign_Kp1 == -1) lcd_puts(1,0,(int8_t*)" Kp1: -"); else lcd_puts(1,0,(int8_t*)" Kp1: +");
				sprintf(line,"%3d,%2d", Kp1_a, Kp1_b);
				lcd_puts(1,7,(int8_t*)line);
				if(sign_Ki1 == -1) lcd_puts(2,0,(int8_t*)" Ki1: -"); else lcd_puts(2,0,(int8_t*)" Ki1: +");
				sprintf(line,"%3d,%2d", Ki1_a, Ki1_b);
				lcd_puts(2,7,(int8_t*)line);
				if(sign_Kd1 == -1) lcd_puts(3,0,(int8_t*)" Kd1: -"); else lcd_puts(3,0,(int8_t*)" Kd1: +");
				sprintf(line,"%3d,%2d", Kd1_a, Kd1_b);
				lcd_puts(3,7,(int8_t*)line);
				lcd_go_to(1,0);
				lcd_cursor_on();
				screen = 4;
			break;
			case 0x80+0x50:
				lcd_puts(3,0,(int8_t*)" Reset: Done");
				Reset = 1;
				lcd_go_to(3,0);
			break;
		}
	}
	if(up_btn){
		HAL_Delay(100);
		up_btn = 0;
		//
		switch(pos_Addr){
			case 0x80+0x00:
				lcd_go_to(3,0);
			break;
			case 0x80+0x40:
				lcd_go_to(0,0);
			break;
			case 0x80+0x10:
				lcd_go_to(1,0);
			break;
			case 0x80+0x50:
				lcd_go_to(2,0);
			break;
		}
	}
	if(down_btn){
		HAL_Delay(100);
		down_btn = 0;
		//
		switch(pos_Addr){
			case 0x80+0x00:
				lcd_go_to(1,0);
			break;
			case 0x80+0x40:
				lcd_go_to(2,0);
			break;
			case 0x80+0x10:
				lcd_go_to(3,0);
			break;
			case 0x80+0x50:
				lcd_go_to(0,0);
			break;
		}
	}
	if(back_btn){
		HAL_Delay(100);
		back_btn = 0;
		//
		lcd_clear();
		if(Mode == Position) lcd_puts(0,0,(int8_t*)"Mode: Position"); else lcd_puts(0,0,(int8_t*)"Mode: Velocity");
		if(Status == Run) lcd_puts(1,0,(int8_t*)"Status: Running"); else lcd_puts(1,0,(int8_t*)"Status: Stop");
		lcd_puts(2,0,(int8_t*)"SP: ");
		lcd_puts(3,0,(int8_t*)"PV: ");
		lcd_cursor_on();
		lcd_go_to(0,0);
		screen = 2;
	}
}

void PID_Screen(void){
	if(ok_btn){
		HAL_Delay(100);
		ok_btn = 0;
		switch(pos_Addr){
		//Nhay con tro Kp
		case 0x80+0x40:
			lcd_go_to(1,6);
		break;
		case 0x80+0x40+6:
			lcd_go_to(1,7);
		break;
		case 0x80+0x40+7:
			lcd_go_to(1,8);
		break;
		case 0x80+0x40+8:
			lcd_go_to(1,9);
		break;
		case 0x80+0x40+9:
			lcd_go_to(1,11);
		break;
		case 0x80+0x40+11:
			lcd_go_to(1,12);
		break;
		//Nhay con tro Ki
		case 0x80+0x10:
			lcd_go_to(2,6);
		break;
		case 0x80+0x10+6:
			lcd_go_to(2,7);
		break;
		case 0x80+0x10+7:
			lcd_go_to(2,8);
		break;
		case 0x80+0x10+8:
			lcd_go_to(2,9);
		break;
		case 0x80+0x10+9:
			lcd_go_to(2,11);
		break;
		case 0x80+0x10+11:
			lcd_go_to(2,12);
		break;
		//Nhay con tro Kd
		case 0x80+0x50:
			lcd_go_to(3,6);
		break;
		case 0x80+0x50+6:
			lcd_go_to(3,7);
		break;
		case 0x80+0x50+7:
			lcd_go_to(3,8);
		break;
		case 0x80+0x50+8:
			lcd_go_to(3,9);
		break;
		case 0x80+0x50+9:
			lcd_go_to(3,11);
		break;
		case 0x80+0x50+11:
			lcd_go_to(3,12);
		break;
		}
	}
	if(up_btn){
		HAL_Delay(100);
		up_btn = 0;
		//
		switch(pos_Addr){
			//Nhay con tro
			case 0x80+0x40:
				lcd_go_to(3,0);
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10:
				lcd_go_to(1,0);
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50:
				lcd_go_to(2,0);
				pos_Addr_temp=pos_Addr;
			break;
			//Dau Kp
			case 0x80+0x40+6:
				lcd_puts(1,6,((int8_t*)"+"));
				sign_Kp1=1;
				lcd_go_to(1,6);
				pos_Addr_temp=pos_Addr;
			break;
			//Tham so Kp
			case 0x80+0x40+7:
				Kp1_a = Kp1_a + 100;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x40+8:
				Kp1_a = Kp1_a + 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x40+9:
				Kp1_a = Kp1_a + 1;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x40+11:
				Kp1_b = Kp1_b + 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x40+12:
				Kp1_b = Kp1_b + 1;
				pos_Addr_temp=pos_Addr;
			break;
			//Dau Ki
			case 0x80+0x10+6:
				lcd_puts(2,6,((int8_t*)"+"));
				sign_Ki1=1;
				lcd_go_to(2,6);
				pos_Addr_temp=pos_Addr;
			break;
			//Tham so Ki
			case 0x80+0x10+7:
				Ki1_a = Ki1_a + 100;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10+8:
				Ki1_a = Ki1_a + 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10+9:
				Ki1_a = Ki1_a + 1;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10+11:
				Ki1_b = Ki1_b + 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10+12:
				Ki1_b = Ki1_b + 1;
				pos_Addr_temp=pos_Addr;
			break;
			//Dau Kd
			case 0x80+0x50+6:
				lcd_puts(2,6,((int8_t*)"+"));
				sign_Kd1=1;
				lcd_go_to(2,6);
				pos_Addr_temp=pos_Addr;
			break;
			//Tham so Kd
			case 0x80+0x50+7:
				Kd1_a = Kd1_a + 100;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50+8:
				Kd1_a = Kd1_a + 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50+9:
				Kd1_a = Kd1_a + 1;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50+11:
				Kd1_b = Kd1_b + 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50+12:
				Kd1_b = Kd1_b + 1;
				pos_Addr_temp=pos_Addr;
			break;
		}
		//Update man hinh
		if(Kp1_a>999) Kp1_a = 999;
		if(Kp1_b>99) Kp1_b = 99;
		if(Ki1_a>999) Ki1_a = 999;
		if(Ki1_b>99) Kp1_a = 99;
		if(Kd1_a>999) Kp1_a = 999;
		if(Kd1_b>99) Kp1_a = 99;
		sprintf(line,"%3d,%2d", Kp1_a, Kp1_b);
		lcd_puts(1,7,(int8_t*)line);
		sprintf(line,"%3d,%2d", Ki1_a, Ki1_b);
		lcd_puts(2,7,(int8_t*)line);
		sprintf(line,"%3d,%2d", Kd1_a, Kd1_b);
		lcd_puts(3,7,(int8_t*)line);
		lcd_write(0,pos_Addr_temp);
	}
	
	if(down_btn)
	{
		HAL_Delay(100);
		down_btn = 0;
		//
		switch(pos_Addr)
		{
			//Nhay con tro
			case 0x80+0x40:
				lcd_go_to(2,0);
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10:
				lcd_go_to(3,0);
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50:
				lcd_go_to(1,0);
				pos_Addr_temp=pos_Addr;
			break;
			//Dau Kp
			case 0x80+0x40+6:
				lcd_puts(1,6,((int8_t*)"-"));
				sign_Kp1=-1;
				lcd_go_to(1,6);
				pos_Addr_temp=pos_Addr;
			break;
			//Tham so Kp
			case 0x80+0x40+7:
				Kp1_a = Kp1_a - 100;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x40+8:
				Kp1_a = Kp1_a - 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x40+9:
				Kp1_a = Kp1_a - 1;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x40+11:
				Kp1_b = Kp1_b - 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x40+12:
				Kp1_b = Kp1_b - 1;
				pos_Addr_temp=pos_Addr;
			break;
			//Dau Ki
			case 0x80+0x10+6:
				lcd_puts(2,6,((int8_t*)"-"));
				sign_Ki1=-1;
				lcd_go_to(2,6);
				pos_Addr_temp=pos_Addr;
			break;
			//Tham so Ki
			case 0x80+0x10+7:
				Ki1_a = Ki1_a - 100;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10+8:
				Ki1_a = Ki1_a - 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10+9:
				Ki1_a = Ki1_a - 1;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10+11:
				Ki1_b = Ki1_b - 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x10+12:
				Ki1_b = Ki1_b - 1;
				pos_Addr_temp=pos_Addr;
			break;
			//Dau Kd
			case 0x80+0x50+6:
				lcd_puts(3,6,((int8_t*)"-"));
				sign_Kd1=-1;
				lcd_go_to(3,6);
				pos_Addr_temp=pos_Addr;
			break;
			//Tham so Kd
			case 0x80+0x50+7:
				Kd1_a = Kd1_a - 100;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50+8:
				Kd1_a = Kd1_a - 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50+9:
				Kd1_a = Kd1_a - 1;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50+11:
				Kd1_b = Kd1_b - 10;
				pos_Addr_temp=pos_Addr;
			break;
			case 0x80+0x50+12:
				Kd1_b = Kd1_b - 1;
				pos_Addr_temp=pos_Addr;
			break;
		}
		//Update man hinh
		if(Kp1_a>999) Kp1_a = 999;
		if(Kp1_b>99) Kp1_b = 99;
		if(Ki1_a>999) Ki1_a = 999;
		if(Ki1_b>99) Kp1_a = 99;
		if(Kd1_a>999) Kp1_a = 999;
		if(Kd1_b>99) Kp1_a = 99;
		sprintf(line,"%3d,%2d", Kp1_a, Kp1_b);
		lcd_puts(1,7,(int8_t*)line);
		sprintf(line,"%3d,%2d", Ki1_a, Ki1_b);
		lcd_puts(2,7,(int8_t*)line);
		sprintf(line,"%3d,%2d", Kd1_a, Kd1_b);
		lcd_puts(3,7,(int8_t*)line);
		lcd_write(0,pos_Addr_temp);
	}
	
	if(back_btn)
	{
		HAL_Delay(100);
		back_btn = 0;
		//
		if(0x80+0x40 < pos_Addr && pos_Addr < 0x80+0x50){
			lcd_go_to(1,0);
		}
		else if(0x80+0x10 < pos_Addr && pos_Addr < 0x80+0x20){
			lcd_go_to(2,0);
		}	
		else if(0x80+0x50 < pos_Addr && pos_Addr < 0x80+0x60){
			lcd_go_to(3,0);
		}	
		else 
		{
			lcd_clear();
			if(Mode == Position) lcd_puts(0,0,(int8_t*)" Mode: Position"); else lcd_puts(0,0,(int8_t*)" Mode: Velocity");
			lcd_puts(1,0,(int8_t*)" Set PID1: ");
			lcd_puts(2,0,(int8_t*)" Set PID2: ");
			lcd_puts(3,0,(int8_t*)" Reset: ");
			lcd_cursor_on();
			screen = 3;
			if(PID_Choose ==1){
			Kp1=sign_Kp1*(Kp1_a+(float)Kp1_b/100);
			Ki1=sign_Ki1*(Ki1_a+(float)Ki1_b/100);
			Kd1=sign_Kd1*(Kd1_a+(float)Kd1_b/100);
			lcd_go_to(1,0);
			}
			else if(PID_Choose ==2){
			Kp2=sign_Kp1*(Kp1_a+(float)Kp1_b/100);
			Ki2=sign_Ki1*(Ki1_a+(float)Ki1_b/100);
			Kd2=sign_Kd1*(Kd1_a+(float)Kd1_b/100);
			Kp1 = Kp_temp;
			Ki1 = Ki_temp;
			Kd1 = Kd_temp;
			lcd_go_to(2,0);
			}
		}
	}
}