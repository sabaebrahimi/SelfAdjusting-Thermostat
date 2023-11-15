/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.h
** Descriptions:		None
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2011-3-29
** Version:				2.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __GLCD_H 
#define __GLCD_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f1xx_hal.h>
#include <stdlib.h>
#include <stdio.h>
//#include "delay.h"
#include "AsciiLib.h"
#include "ArialFont.h"
#include "ArialFont22pt.h"
#include "ArialFont30pt.h"
//#include "VinGlobal.h"

/* Private define ------------------------------------------------------------*/

//LCD Register defines
//Driver: SSD1289
#define GLCD_X_CURSOR_REG				0X004E
#define GLCD_Y_CURSOR_REG				0X004F
#define GLCD_DATA_REG						0X0022
/*******END*******/

#define DISP_ORIENTATION  0  /* angle 0 90 */ 

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif

#define LCD_Delay								10
#define ON											1
#define OFF											0
//#define LCD_DIR_REG							DIR
#define LCD_DIRL_REG						CRL
#define LCD_DIRH_REG						CRH
//#define LCD_DATA_REG						PIN
#define LCD_INDATA_REG						IDR
#define LCD_OUTDATA_REG						ODR
#define LCD_DATA_BIT_OFFSET			0
#define LCD_DATA_PORT						GPIOE
#define LCD_CS_PORT							GPIOB
#define LCD_RS_PORT							GPIOB
#define LCD_RD_PORT							GPIOC
#define LCD_WR_PORT							GPIOC
#define LCD_RST_PORT						GPIOA
#define LCD_BL_PORT							GPIOD
#define LCD_CS_PIN							1
#define LCD_RS_PIN							0
#define LCD_RD_PIN							4
#define LCD_WR_PIN							5
#define LCD_RST_PIN							7
#define LCD_BL_PIN							2

//#define RCC_LCD_DATA_PORT				RCC_APB2Periph_GPIOE
//#define RCC_LCD_CS_PORT					RCC_APB2Periph_GPIOB
//#define RCC_LCD_RS_PORT					RCC_APB2Periph_GPIOB
//#define RCC_LCD_RD_PORT					RCC_APB2Periph_GPIOC
//#define RCC_LCD_WR_PORT					RCC_APB2Periph_GPIOC
//#define RCC_LCD_RST_PORT				RCC_APB2Periph_GPIOA
//#define RCC_LCD_BL_PORT					RCC_APB2Periph_GPIOD
/*
#define Set_Cs        GPIO_SetBits(GPIOC , GPIO_Pin_9);
#define Clr_Cs        GPIO_ResetBits(GPIOC , GPIO_Pin_9);

#define Set_Rs        GPIO_SetBits(GPIOC , GPIO_Pin_8);
#define Clr_Rs        GPIO_ResetBits(GPIOC , GPIO_Pin_8);

#define Set_nWr       GPIO_SetBits(GPIOC , GPIO_Pin_7);
#define Clr_nWr       GPIO_ResetBits(GPIOC , GPIO_Pin_7);

#define Set_nRd       GPIO_SetBits(GPIOC , GPIO_Pin_6);
#define Clr_nRd       GPIO_ResetBits(GPIOC , GPIO_Pin_6);
*/

#define Set_Cs        LCD_CS_PORT->LCD_OUTDATA_REG  |= ( 1<<LCD_CS_PIN );
#define Clr_Cs        LCD_CS_PORT->LCD_OUTDATA_REG  &=~( 1<<LCD_CS_PIN );

#define Set_Rs        LCD_RS_PORT->LCD_OUTDATA_REG  |= ( 1<<LCD_RS_PIN );
#define Clr_Rs        LCD_RS_PORT->LCD_OUTDATA_REG  &=~( 1<<LCD_RS_PIN );

#define Set_nWr       LCD_WR_PORT->LCD_OUTDATA_REG  |= ( 1<<LCD_WR_PIN );
#define Clr_nWr       LCD_WR_PORT->LCD_OUTDATA_REG  &=~( 1<<LCD_WR_PIN );

#define Set_nRd       LCD_RD_PORT->LCD_OUTDATA_REG  |= ( 1<<LCD_RD_PIN );
#define Clr_nRd       LCD_RD_PORT->LCD_OUTDATA_REG  &=~( 1<<LCD_RD_PIN );

#define Set_Rst       LCD_RST_PORT->LCD_OUTDATA_REG  |= ( 1<<LCD_RST_PIN );
#define Clr_Rst       LCD_RST_PORT->LCD_OUTDATA_REG  &=~( 1<<LCD_RST_PIN );

#define LCD_BL(x)			((x)? (LCD_BL_PORT->LCD_OUTDATA_REG&=~(1<<LCD_BL_PIN)) : (LCD_BL_PORT->LCD_OUTDATA_REG|=(1<<LCD_BL_PIN)));
#define LCD_BL_STATUS	(((LCD_BL_PORT->LCD_OUTDATA_REG)&(1<<LCD_BL_PIN))>>LCD_BL_PIN)

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#define Blue3          0x2e7f
#define NO_BG          0x0001

/******************************************************************************
* Function Name  : RGB565CONVERT
* Description    : 24bit Convert to 16bit
* Input          : - red: R
*                  - green: G 
*				   - blue: B
* Output         : None
* Return         : RGB color
* Attention		 : None
*******************************************************************************/
#define RGB565CONVERT(red, green, blue)\
(unsigned short)   ( ( ( red   >> 3 ) << 11 ) | \
                   ( ( green >> 2 ) << 5  ) | \
                   ( blue  >> 3 ))

/* Private function prototypes -----------------------------------------------*/
void LCD_Initializtion(void);
void LCD_Clear(uint16_t Color);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
uint8_t PutCharArial_18pt( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void GUI_TextArial_18pt(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
uint8_t PutCharArial_22pt( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void GUI_TextArial_22pt(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
uint8_t PutCharArial_30pt( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void GUI_TextArial_30pt(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void Draw_BMP(unsigned int x_pos, unsigned int y_pos, signed int W, signed int H, const unsigned short *bitmap);
uint8_t Draw_BMP_Slice(unsigned int x_pos, unsigned int y_pos, signed int W, signed int H, const unsigned short *bitmap);
uint8_t Draw_BMP_Fast(unsigned int x_pos, unsigned int y_pos, const unsigned short *bitmap);
uint8_t Draw_BMP_Fast_NO_BackGround(unsigned int x_pos, unsigned int y_pos, const unsigned short *bitmap);
void Draw_Circle(uint16_t x0,uint16_t y0,uint16_t radius,uint16_t color );
void Draw_FillCircle(uint16_t x0,uint16_t y0,uint16_t radius,uint16_t color );

extern const unsigned short MainBG[0x12C00+2];
extern const unsigned short arrowup[900];
extern const unsigned short arrowdown[900];

#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
