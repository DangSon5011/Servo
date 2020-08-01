#ifndef		__LCDTXT_H
#define		__LCDTXT_H

#include "stm32f7xx.h"
#include "main.h"

/*------------- Define LCD Use -----------------*/

/*Note: Comment which not use */

#define LCD16xN //For lcd16x2 or lcd16x4
//#define LCD20xN //For lcd20x4

/*------------- Define For Connection -----------------*/

#define RS_PORT		GPIOD
#define RS_PIN		GPIO_PIN_2

#define EN_PORT		GPIOG
#define EN_PIN		GPIO_PIN_3

#define D7_PORT		GPIOD
#define D7_PIN		GPIO_PIN_6

#define D6_PORT		GPIOD
#define D6_PIN		GPIO_PIN_5

#define D5_PORT		GPIOD
#define D5_PIN		GPIO_PIN_4

#define D4_PORT		GPIOD
#define D4_PIN		GPIO_PIN_3

#define Pos 	1
#define Vel		2


#define Runn			1
#define Stopp			0
#define Initt			2
/*------------ Declaring Private Macro -----------------*/

#define PIN_LOW(PORT,PIN)		HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_RESET);
#define PIN_HIGH(PORT,PIN)	HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_SET);

/*------------ Declaring Function Prototype -------------*/
void lcd_init(void);
void lcd_write(uint8_t type,uint8_t data);
void lcd_puts(uint8_t x, uint8_t y, int8_t *string);
void lcd_clear(void);
void lcd_cursor_on(void);
void lcd_cursor_off(void);
void lcd_display_on(void);
void lcd_display_off(void);
void lcd_go_to(uint8_t x, uint8_t y);

void Update_Main(void);
void PC_Screen(void);
void Main_Screen(void);
void Setting_Screen(void);

void PID_Screen(void);
#endif

