/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.c
** Descriptions:		This example describes how to use glcd
**						
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2011-3-29
** Version:			2.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h" 

/* Program is based on the STM32 MCU */
/* If you are not use STM32, you must transplant program */
/*******************************************************************************
* Function Name  : LCD_Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention	 : None
*******************************************************************************/
static void delay_ms( unsigned short ms )    
{ 
	unsigned short i, j;

	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1141; j++ );
	}
} 

/*******************************************************************************
* Function Name  : wait_delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void wait_delay(int count)
{
	while(count--);
}

/*******************************************************************************
* Function Name  : Lcd_Configuration
* Description    : setting the IO to control LCD
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void LCD_Configuration(void)
{
/* If you are not use STM32, you must change it */
	          
/* 
   D0~D15 -> GPIO3.16~GPIO3.31
	 D0~D15 -> GPIOE.0~GPIOE.16
*/ 

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

//	RCC_APB2PeriphClockCmd(RCC_LCD_DATA_PORT, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_LCD_CS_PORT, 	ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_LCD_RS_PORT, 	ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_LCD_RD_PORT, 	ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_LCD_WR_PORT, 	ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_LCD_RST_PORT, 	ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_LCD_BL_PORT, 	ENABLE);

	/* setting the IO to output mode */

	LCD_DATA_PORT->LCD_DIRL_REG= (0x33333333 << LCD_DATA_BIT_OFFSET);
	LCD_DATA_PORT->LCD_DIRH_REG= (0x33333333 << LCD_DATA_BIT_OFFSET);
	
	/* (RS), (WR), (RD) ,(CS) */

  /* setting the IO to output mode */
	LCD_RS_PORT->LCD_DIRL_REG&=~(0xF<<0);
	LCD_RS_PORT->LCD_DIRL_REG|= (0x3<<0);
	LCD_RD_PORT->LCD_DIRL_REG&=~(0xF<<16);
	LCD_RD_PORT->LCD_DIRL_REG|= (0x3<<16);
	LCD_WR_PORT->LCD_DIRL_REG&=~(0xF<<20);
	LCD_WR_PORT->LCD_DIRL_REG|= (0x3<<20);
	LCD_CS_PORT->LCD_DIRL_REG&=~(0xF<<4);
	LCD_CS_PORT->LCD_DIRL_REG|= (0x3<<4);
	LCD_RST_PORT->LCD_DIRL_REG&=~(0xF<<28);
	LCD_RST_PORT->LCD_DIRL_REG|= (0x3<<28);
	LCD_BL_PORT->LCD_DIRL_REG&=~(0xF<<24);
	LCD_BL_PORT->LCD_DIRL_REG|= (0x3<<24);
	LCD_BL(ON);
}


/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Description    : LCD write register address ( on include data only address )
* Input          : - index: register address
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_WriteIndex(unsigned short index)
{
        /* If you are not use STM32, you must change it */
	Clr_Rs;   /* RS low */
	Set_nRd;  /* RD high */
	//Delay_us(LCD_Delay);
        /* write data */
//	GPIOE->ODR = index;	 /* GPIO_Write(GPIOE,index); */
//	LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
//	LCD_DATA_PORT->LCD_OUTDATA_REG |= (index<<LCD_DATA_BIT_OFFSET);
	LCD_DATA_PORT->LCD_OUTDATA_REG = (index<<LCD_DATA_BIT_OFFSET); // Because STM32F103 Port is 16Bit
	//wait_delay(25);	
	Clr_nWr;  /* Wr low */
	//wait_delay(1);
	Set_nWr;  /* Wr high */
}

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : LCD write register data
* Input          : - index: register data
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteData(unsigned short data)
{
	Set_Rs;  /* RS high */
	//Delay_us(LCD_Delay);
        /* write data */
//	GPIOE->ODR = data;	 /* GPIO_Write(GPIOE,data); */
//	LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
//	LCD_DATA_PORT->LCD_OUTDATA_REG |= (data<<LCD_DATA_BIT_OFFSET);
		LCD_DATA_PORT->LCD_OUTDATA_REG = (data<<LCD_DATA_BIT_OFFSET);
	Clr_nWr;  /* Wr low */
	//wait_delay(1);
	Set_nWr;  /* Wr high */
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : LCD read data
* Input          : None
* Output         : None
* Return         : return data
* Attention	 : None
*******************************************************************************/
unsigned short LCD_ReadData(void)
{ 
        /* If you are not use STM32, you must change it */

	unsigned short value;
	Set_Rs;  /* RS high */ 
	Set_nWr; /* Wr high */
	Clr_nRd; /* Rd low */ 

/* 
    PE.00(D0), PE.01(D1), PE.02(D2), PE.03(D3), PE.04(D4), PE.05(D5), PE.06(D6), PE.07(D7), PE.08(D8)
    PE.09(D9), PE.10(D10), PE.11(D11), PE.12(D12), PE.13(D13), PE.14(D14), PE.15(D15)   */

    /* setting the IO to input mode */

    
	LCD_DATA_PORT->LCD_DIRL_REG= (0x88888888 << LCD_DATA_BIT_OFFSET);
	LCD_DATA_PORT->LCD_DIRH_REG= (0x88888888 << LCD_DATA_BIT_OFFSET);
	
	
    /* read data */
//    value = GPIO_ReadInputData(GPIOE);
//    value = GPIO_ReadInputData(GPIOE);
    value = (LCD_DATA_PORT->LCD_INDATA_REG)>>LCD_DATA_BIT_OFFSET;
		value = (LCD_DATA_PORT->LCD_INDATA_REG)>>LCD_DATA_BIT_OFFSET;
    /* Read twice to ensure correct data */

    /* read data OK */

/*  
    PE.00(D0), PE.01(D1), PE.02(D2), PE.03(D3), PE.04(D4), PE.05(D5), PE.06(D6), PE.07(D7), PE.08(D8)
    PE.09(D9), PE.10(D10), PE.11(D11), PE.12(D12), PE.13(D13), PE.14(D14), PE.15(D15)   
*/
    
    /* setting the IO to output mode */

    LCD_DATA_PORT->LCD_DIRL_REG= (0x33333333 << LCD_DATA_BIT_OFFSET);
		LCD_DATA_PORT->LCD_DIRH_REG= (0x33333333 << LCD_DATA_BIT_OFFSET);

    Set_nRd;   /* Rd high */
    return value;  /* return data */
}


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention	 : None
*******************************************************************************/
void LCD_WriteReg( unsigned short LCD_Reg, unsigned short LCD_RegValue )
{ 
	/* Write 16-bit Index, then Write Reg */

	Clr_Cs;   /* Cs low */ 

        /* selected LCD register */ 
	LCD_WriteIndex(LCD_Reg);
      
	/* Write register data */
	LCD_WriteData(LCD_RegValue);
  
	Set_Cs;   /* Cs high */
}

/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD register data.
* Input          : - LCD_Reg: address of the selected register.
* Output         : None
* Return         : return Register data.
* Attention	 : None
*******************************************************************************/
unsigned short LCD_ReadReg(unsigned short LCD_Reg)
{
	unsigned short LCD_RAM;
	
	Clr_Cs;  /* Cs low */ 
 
        /* selected LCD register */ 
	LCD_WriteIndex(LCD_Reg);

	/* Write register data */
	LCD_RAM = LCD_ReadData(); 
     	
	Set_Cs;  /* Cs high */

        /* return read data */
	return LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetCursor( unsigned short Xpos, unsigned short Ypos )
{
    LCD_WriteReg(0x004e, Xpos );     
    LCD_WriteReg(0x004f, Ypos );     
}


static void LCD_reset_sequence(void)
{
    Set_Rst;
    delay_ms(50);
    Clr_Rst;
    delay_ms(1000);
    Set_Rst;
    delay_ms(150);
}


/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : Initialize TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention	 : None
*******************************************************************************/
void LCD_Initializtion(void)
{
	unsigned short LCD_ID;
	char str[50];
	
	LCD_Configuration();
	LCD_reset_sequence();
	LCD_ID = LCD_ReadReg(0x0000);		/* read the LCD ID, if Controller is ILI9320, The ID value is 0x9320 */
	
	sprintf(str,"LCD ID:%x\r\n",LCD_ID);
	//UARTPuts(USART1,str);
	if( LCD_ID == 0x9320 || LCD_ID == 0x9300 )
	{
		//UARTPuts(USART1,"ILI Driver\r\n");
	    LCD_WriteReg(0x00,0x0000);
	    LCD_WriteReg(0x01,0x0100);	/* Driver Output Contral */
	    LCD_WriteReg(0x02,0x0700);	/* LCD Driver Waveform Contral */
	    LCD_WriteReg(0x03,0x1018);	/* Entry Mode Set */
		
	    LCD_WriteReg(0x04,0x0000);	/* Scalling Contral */
	    LCD_WriteReg(0x08,0x0202);	/* Display Contral */
	    LCD_WriteReg(0x09,0x0000);	/* Display Contral 3.(0x0000) */
	    LCD_WriteReg(0x0a,0x0000);	/* Frame Cycle Contal.(0x0000) */
	    LCD_WriteReg(0x0c,(1<<0));	/* Extern Display Interface Contral */
	    LCD_WriteReg(0x0d,0x0000);	/* Frame Maker Position */
	    LCD_WriteReg(0x0f,0x0000);	/* Extern Display Interface Contral 2. */
		
	    delay_ms(100);  /* delay 100 ms */		
	    LCD_WriteReg(0x07,0x0101);	/* Display Contral */
	    delay_ms(100);  /* delay 100 ms */		
	
	    LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	/* Power Control 1.(0x16b0)	*/
	    LCD_WriteReg(0x11,0x0007);								/* Power Control 2 */
	    LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				/* Power Control 3.(0x0138)	*/
	    LCD_WriteReg(0x13,0x0b00);								/* Power Control 4 */
	    LCD_WriteReg(0x29,0x0000);								/* Power Control 7 */
		
	    LCD_WriteReg(0x2b,(1<<14)|(1<<4));
			
	    LCD_WriteReg(0x50,0);       /* Set X Start */
	    LCD_WriteReg(0x51,239);	    /* Set X End */
	    LCD_WriteReg(0x52,0);	    /* Set Y Start */
	    LCD_WriteReg(0x53,319);	    /* Set Y End */
		
	    LCD_WriteReg(0x60,0x2700);	/* Driver Output Control */
	    LCD_WriteReg(0x61,0x0001);	/* Driver Output Control */
	    LCD_WriteReg(0x6a,0x0000);	/* Vertical Srcoll Control */
		
	    LCD_WriteReg(0x80,0x0000);	/* Display Position? Partial Display 1 */
	    LCD_WriteReg(0x81,0x0000);	/* RAM Address Start? Partial Display 1 */
	    LCD_WriteReg(0x82,0x0000);	/* RAM Address End-Partial Display 1 */
	    LCD_WriteReg(0x83,0x0000);	/* Displsy Position? Partial Display 2 */
	    LCD_WriteReg(0x84,0x0000);	/* RAM Address Start? Partial Display 2 */
	    LCD_WriteReg(0x85,0x0000);	/* RAM Address End? Partial Display 2 */
		
	    LCD_WriteReg(0x90,(0<<7)|(16<<0));	/* Frame Cycle Contral.(0x0013)	*/
	    LCD_WriteReg(0x92,0x0000);	/* Panel Interface Contral 2.(0x0000) */
	    LCD_WriteReg(0x93,0x0001);	/* Panel Interface Contral 3. */
	    LCD_WriteReg(0x95,0x0110);	/* Frame Cycle Contral.(0x0110)	*/
	    LCD_WriteReg(0x97,(0<<8));	
	    LCD_WriteReg(0x98,0x0000);	/* Frame Cycle Contral */
	    LCD_WriteReg(0x07,0x0173);
	}
	else if( LCD_ID == 0x8989 )		 //LCD_Code = SSD1289;
	{
			LCD_WriteReg(0x0000,0x0001);    delay_ms(50);   /* ´ٍ؟ھ¾§صٌ */
	    LCD_WriteReg(0x0003,0xA8A4);    delay_ms(50);	
	    LCD_WriteReg(0x000C,0x0000);    delay_ms(50);   
	    LCD_WriteReg(0x000D,0x080C);    delay_ms(50);   
	    LCD_WriteReg(0x000E,0x2B00);    delay_ms(50);   
	    LCD_WriteReg(0x001E,0x00B0);    delay_ms(50);   
	    LCD_WriteReg(0x0001,0x2B3F);    delay_ms(50);   /* ا‎¶¯تن³ِ؟طضئ320*240 0x2B3F */
	    LCD_WriteReg(0x0002,0x0600);    delay_ms(50);
	    LCD_WriteReg(0x0010,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0011,0x6070);    delay_ms(50);   /* ¶¨زهت‎¾ف¸ٌت½ 16خ»ة« ؛لئء 0x6070 */
	    LCD_WriteReg(0x0005,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0006,0x0000);    delay_ms(50);
	    //LCD_WriteReg(0x0016,0xEF1C);    delay_ms(50);
	    //LCD_WriteReg(0x0017,0x0003);    delay_ms(50);
	    LCD_WriteReg(0x0007,0x0133);    delay_ms(50);         
	    LCD_WriteReg(0x000B,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x000F,0x0000);    delay_ms(50);   /* ة¨أè؟ھت¼µطض· */
	    LCD_WriteReg(0x0041,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0042,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0048,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0049,0x013F);    delay_ms(50);
	    LCD_WriteReg(0x004A,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x004B,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0044,0xEF00);    delay_ms(50);
	    LCD_WriteReg(0x0045,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0046,0x013F);    delay_ms(50);
	    LCD_WriteReg(0x0030,0x0707);    delay_ms(50);
	    LCD_WriteReg(0x0031,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0032,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0033,0x0502);    delay_ms(50);
	    LCD_WriteReg(0x0034,0x0507);    delay_ms(50);
	    LCD_WriteReg(0x0035,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0036,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0037,0x0502);    delay_ms(50);
	    LCD_WriteReg(0x003A,0x0302);    delay_ms(50);
	    LCD_WriteReg(0x003B,0x0302);    delay_ms(50);
	    LCD_WriteReg(0x0023,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0024,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0025,0x8000);    delay_ms(50);
	    LCD_WriteReg(0x004f,0);        /* ذذت×ض·0 */
	    LCD_WriteReg(0x004e,0);        /* ءذت×ض·0 */
	}

	else if(LCD_ID==0x8999)	   //הӦքȽ֯ICΪSSD1289
	{
		//UARTPuts(USART1,"SSD1298 Driver\r\n");
		//************* Start Initial Sequence **********//
		LCD_WriteReg(0x00, 0x0001); // Start internal OSC.
		LCD_WriteReg(0x01, 0x3B3F); // Driver output control, RL=0;REV=1;GD=1;BGR=0;SM=0;TB=1
		LCD_WriteReg(0x02, 0x0600); // set 1 line inversion
		//************* Power control setup ************/
		LCD_WriteReg(0x0C, 0x0007); // Adjust VCIX2 output voltage
		LCD_WriteReg(0x0D, 0x0006); // Set amplitude magnification of VLCD63
		LCD_WriteReg(0x0E, 0x3200); // Set alternating amplitude of VCOM
		LCD_WriteReg(0x1E, 0x00BB); // Set VcomH voltage
		LCD_WriteReg(0x03, 0x6A64); // Step-up factor/cycle setting  7b30
		//************ RAM position control **********/
		LCD_WriteReg(0x0F, 0x0000); // Gate scan position start at G0.
		LCD_WriteReg(0x44, 0xEF00); // Horizontal RAM address position
		LCD_WriteReg(0x45, 0x0000); // Vertical RAM address start position
		LCD_WriteReg(0x46, 0x013F); // Vertical RAM address end position
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x30, 0x0000);
		LCD_WriteReg(0x31, 0x0706);
		LCD_WriteReg(0x32, 0x0206);
		LCD_WriteReg(0x33, 0x0300);
		LCD_WriteReg(0x34, 0x0002);
		LCD_WriteReg(0x35, 0x0000);
		LCD_WriteReg(0x36, 0x0707);
		LCD_WriteReg(0x37, 0x0200);
		LCD_WriteReg(0x3A, 0x0908);
		LCD_WriteReg(0x3B, 0x0F0D);
		//************* Special command **************/
		LCD_WriteReg(0x28, 0x0006); // Enable test command
		LCD_WriteReg(0x2F, 0x12EB); // RAM speed tuning
		LCD_WriteReg(0x26, 0x7000); // Internal Bandgap strength
		LCD_WriteReg(0x20, 0xB0E3); // Internal Vcom strength
		LCD_WriteReg(0x27, 0x0044); // Internal Vcomh/VcomL timing
		LCD_WriteReg(0x2E, 0x7E45); // VCOM charge sharing time  
		//************* Turn On display ******************/
		LCD_WriteReg(0x10, 0x0000); // Sleep mode off.
		delay_ms(8); // Wait 30mS
		LCD_WriteReg(0x11, 0x6878); // Entry mode setup. 262K type B, take care on the data bus with 16it only 
		LCD_WriteReg(0x07, 0x0033); // Display ON	*/
	}
	else
	{
		//UARTPuts(USART1,"else Driver\r\n");
				/* read LCD ID fail, testing terminated */
				/* fatal error */
				while(1);
	}					
    delay_ms(50);   /* delay 50 ms */		
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : clear LCD screen
* Input          : - Color: LCD screen color
* Output         : None
* Return         : None
* Attention	 : None
*******************************************************************************/
void LCD_Clear( unsigned short color )
{
	unsigned int index=0;
	
	LCD_SetCursor(0,0); 
	
	Clr_Cs;  /* Cs low */

        /* selected LCD register */ 
	LCD_WriteIndex(0x0022);

	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
                /* Write data */
		LCD_WriteData( color );
	}

	Set_Cs;  /* Cs high */
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB change BBBBBGGGGGGRRRRR
* Input          : - color: BRG color value
* Output         : None
* Return         : RGB color value
* Attention	 : None
*******************************************************************************/
unsigned short LCD_BGR2RGB( unsigned short color )
{
	unsigned short  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : Get the color value of the specified coordinates
* Input          : - Xpos: Row Coordinate
*                  - Xpos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention	 : None
*******************************************************************************/
unsigned short LCD_GetPoint( unsigned short Xpos, unsigned short Ypos )
{
	unsigned short dummy;
	
	LCD_SetCursor(Xpos,Ypos);

	Clr_Cs;   /* Cs low */

        /* selected LCD register */ 
	LCD_WriteIndex(0x0022);  
	

        dummy = LCD_ReadData();   /* NOP read necessary */
        dummy = LCD_ReadData();   /* read data */

        Set_Cs;   /* Cs high */
        
        /* RRRRRGGGGGGBBBBB change BBBBBGGGGGGRRRRR and return data */
        return  LCD_BGR2RGB( dummy );

}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : Draw point at the specified coordinates
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(MAX_X-1-Xpos,MAX_Y-1-Ypos); //Inverted position
//	LCD_SetCursor(Xpos,Ypos); //Non Inverted position
	LCD_WriteReg(0x0022,point);
}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: A point line coordinates
*                  - y1: A point column coordinates 
*		   - x2: B point line coordinates
*		   - y2: B point column coordinates 
*		   - color: Line color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;      /* The definition of the X Y axis increase the value of the variable */
    short temp;       

    if( x0 > x1 )     
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;   
    }
    if( y0 > y1 )     
    {
		temp = y1;
		y1 = y0;
		y0 = temp;   
    }
  
	dx = x1-x0;       
	dy = y1-y0;       

    if( dx == 0 )     
    {
        do
        { 
            LCD_SetPoint(x0, y0, color);   
            y0++;
        }
        while( y1 >= y0 ); 
		return; 
    }
    if( dy == 0 )     
    {
        do
        {
            LCD_SetPoint(x0, y0, color);
            x0++;
        }
        while( x1 >= x0 ); 
		return;
    }
	
    if( dx > dy )                        
    {
	    temp = 2 * dy - dx;                
        while( x0 != x1 )
        {
	        LCD_SetPoint(x0,y0,color);  
	        x0++;                        
	        if( temp > 0 )               
	        {
	            y0++;                    
	            temp += 2 * dy - 2 * dx; 
	 	    }
            else         
            {
			    temp += 2 * dy;          
			}       
        }
        LCD_SetPoint(x0,y0,color);
    }  
    else
    {
	    temp = 2 * dx - dy;                     
        while( y0 != y1 )
        {
	 	    LCD_SetPoint(x0,y0,color);     
            y0++;                 
            if( temp > 0 )           
            {
                x0++;               
                temp+=2*dy-2*dx; 
            }
            else
			{
                temp += 2 * dy;
			}
        } 
        LCD_SetPoint(x0,y0,color);
	}
} 

/******************************************************************************
* Function Name  : PutChar
* Description    : Lcd screen displays a character
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - ASCI: Displayed character
*				   - charColor: Character color  
*				   - bkColor: Background color 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
	uint8_t buffer[16], tmp_char;
	GetASCIICode(buffer,ASCI);  /* get font data */
	for( i=0; i<16; i++ )
	{
			tmp_char = buffer[i];
			for( j=0; j<8; j++ )
			{
					if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
					{
							LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
					}
					else
					{
						if(bkColor==NO_BG){}
						else
							LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* Background color */
					}
			}
	}
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : Displays the string
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - str: Displayed string
*				   - charColor: Character color   
*				   - bkColor: Background color  
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}

#ifdef	ARIAL_18PT
/******************************************************************************
* Function Name  : PutCharArial
* Description    : Lcd screen displays a Standard character
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - ASCI: Displayed character
*				   - charColor: Character color  
*				   - bkColor: Background color 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint8_t PutCharArial_18pt( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	char str[50];
	uint16_t i, j,k,OffsetByte=0;
	uint16_t FontInfo[2];	//FontInfo[0] -> Char width in bits ,FontInfo[1] -> Offset in bytes
	uint8_t Width_Data;
	uint8_t  WidthByte_Index,WidthByte_Index_Temp;
	
	if(ASCI==' ') //Space
	{
		FontInfo[0]=CHAR_SPACE_18PT;
		WidthByte_Index=1;
	}
	else
	{
		GetFontInfo_18pt(FontInfo,ASCI);  /* get font data */
		WidthByte_Index=FontInfo[0]/8;
		if(FontInfo[0]%8)	WidthByte_Index++;
	}
	for( i=0; i<ARIALFONT_HIGHT_18PT; i++ )
	{
		j=0;
		WidthByte_Index_Temp=WidthByte_Index;
		while(WidthByte_Index_Temp)
		{
			if(ASCI==' ') //Space
				Width_Data=0;
			else
				Width_Data=arial_18pt[FontInfo[1]+OffsetByte];
			for(k=0;k<8;k++)
			{
				if((Width_Data & (1<<(7-k))) !=0 )
				{
						LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
				}
				else
				{
					if(bkColor==NO_BG){}
					else
					{
						LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* Background color */
					}
				}
				j++;
				if(j>FontInfo[0]+CHAR_SPACE_18PT)	break;
			}
			OffsetByte++;
			WidthByte_Index_Temp--;
		}
	}
	return FontInfo[0];
}
/******************************************************************************
* Function Name  : GUI_TextArial
* Description    : Displays the string
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - str: Displayed string
*				   - charColor: Character color   
*				   - bkColor: Background color  
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_TextArial_18pt(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
	uint8_t CharWidth;
    do
    {
        TempChar = *str++;  
        CharWidth=PutCharArial_18pt( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += CharWidth+CHAR_SPACE_18PT;
        } 
        else if ( Ypos < MAX_Y - ARIALFONT_HIGHT_18PT )
        {
            Xpos = 0;
            Ypos += ARIALFONT_HIGHT_18PT;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}
#endif

#ifdef	ARIAL_22PT
/******************************************************************************
* Function Name  : PutCharArial_22pt
* Description    : Lcd screen displays a Standard character
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - ASCI: Displayed character
*				   - charColor: Character color  
*				   - bkColor: Background color 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint8_t PutCharArial_22pt( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	char str[50];
	uint16_t i, j,k,OffsetByte=0;
	uint16_t FontInfo[2];	//FontInfo[0] -> Char width in bits ,FontInfo[1] -> Offset in bytes
	uint8_t Width_Data;
	uint8_t  WidthByte_Index,WidthByte_Index_Temp;
	
	if(ASCI==' ') //Space
	{
		FontInfo[0]=CHAR_SPACE_22PT;
		WidthByte_Index=1;
	}
	else
	{
		GetFontInfo_22pt(FontInfo,ASCI);  /* get font data */
		WidthByte_Index=FontInfo[0]/8;
		if(FontInfo[0]%8)	WidthByte_Index++;
	}
	for( i=0; i<ARIALFONT_HIGHT_22PT; i++ )
	{
		j=0;
		WidthByte_Index_Temp=WidthByte_Index;
		while(WidthByte_Index_Temp)
		{
			if(ASCI==' ') //Space
				Width_Data=0;
			else
				Width_Data=arial_22pt[FontInfo[1]+OffsetByte];
			for(k=0;k<8;k++)
			{
				if((Width_Data & (1<<(7-k))) !=0 )
				{
						LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
				}
				else
				{
					if(bkColor==NO_BG){}
					else
					{
						LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* Background color */
					}
				}
				j++;
				if(j>FontInfo[0]+CHAR_SPACE_22PT)	break;
			}
			OffsetByte++;
			WidthByte_Index_Temp--;
		}
	}
	return FontInfo[0];
}
/******************************************************************************
* Function Name  : GUI_TextArial_22pt
* Description    : Displays the string
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - str: Displayed string
*				   - charColor: Character color   
*				   - bkColor: Background color  
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_TextArial_22pt(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
	uint8_t CharWidth;
    do
    {
        TempChar = *str++;  
        CharWidth=PutCharArial_22pt( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += CharWidth+CHAR_SPACE_22PT;
        } 
        else if ( Ypos < MAX_Y - ARIALFONT_HIGHT_22PT )
        {
            Xpos = 0;
            Ypos += ARIALFONT_HIGHT_22PT;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}
#endif

#ifdef	ARIAL_30PT
/******************************************************************************
* Function Name  : PutCharArial_22pt
* Description    : Lcd screen displays a Standard character
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - ASCI: Displayed character
*				   - charColor: Character color  
*				   - bkColor: Background color 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint8_t PutCharArial_30pt( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	char str[50];
	uint16_t i, j,k,OffsetByte=0;
	uint16_t FontInfo[2];	//FontInfo[0] -> Char width in bits ,FontInfo[1] -> Offset in bytes
	uint8_t Width_Data;
	uint8_t  WidthByte_Index,WidthByte_Index_Temp;
	
	if(ASCI==' ') //Space
	{
		FontInfo[0]=CHAR_SPACE_30PT;
		WidthByte_Index=1;
	}
	else
	{
		GetFontInfo_30pt(FontInfo,ASCI);  /* get font data */
		WidthByte_Index=FontInfo[0]/8;
		if(FontInfo[0]%8)	WidthByte_Index++;
	}
	for( i=0; i<ARIALFONT_HIGHT_30PT; i++ )
	{
		j=0;
		WidthByte_Index_Temp=WidthByte_Index;
		while(WidthByte_Index_Temp)
		{
			if(ASCI==' ') //Space
				Width_Data=0;
			else
				Width_Data=arial_30pt[FontInfo[1]+OffsetByte];
			for(k=0;k<8;k++)
			{
				if((Width_Data & (1<<(7-k))) !=0 )
				{
						LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
				}
				else
				{
					if(bkColor==NO_BG){}
					else
					{
						LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* Background color */
					}
				}
				j++;
				if(j>FontInfo[0]+CHAR_SPACE_30PT)	break;
			}
			OffsetByte++;
			WidthByte_Index_Temp--;
		}
	}
	return FontInfo[0];
}
/******************************************************************************
* Function Name  : GUI_TextArial_22pt
* Description    : Displays the string
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - str: Displayed string
*				   - charColor: Character color   
*				   - bkColor: Background color  
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_TextArial_30pt(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
	uint8_t CharWidth;
    do
    {
        TempChar = *str++;  
        CharWidth=PutCharArial_30pt( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += CharWidth+CHAR_SPACE_30PT;
        } 
        else if ( Ypos < MAX_Y - ARIALFONT_HIGHT_30PT )
        {
            Xpos = 0;
            Ypos += ARIALFONT_HIGHT_30PT;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}
#endif

void Draw_BMP(unsigned int x_pos, unsigned int y_pos, signed int W, signed int H, const unsigned short *bitmap)
{
     signed int xp = 0;
     signed int yp = (y_pos );
     unsigned long index = 2;


     while(yp < H)
     {
         for(xp = x_pos; xp < (x_pos + W); xp++)
         {
					 LCD_SetPoint(xp,yp,bitmap[index++]);
         }
         yp++;
     }
}

uint8_t Draw_BMP_Slice(unsigned int x_pos, unsigned int y_pos, signed int W, signed int H, const unsigned short *bitmap)
{
//     signed int xp = 0;
//     signed int yp = (y_pos );
//     unsigned long index = 0;

//			index= (bitmap[0]*y_pos)+x_pos+2;
//     while(yp < H+y_pos)
//     {
//         for(xp = x_pos; xp < (x_pos + W); xp++)
//         {
//					 LCD_SetPoint(xp,yp,bitmap[index++]);
//         }
//				 index= index+bitmap[0]-W;
//         yp++;
//     }
	signed int xp = 0;
	signed int yp = (y_pos );
	unsigned long index = 0;
	signed int BMPW=bitmap[index++];
	signed int BMPH=bitmap[index++];
	
	index= (bitmap[0]*y_pos)+x_pos+2;
	if( BMPW > MAX_X || BMPH > MAX_Y )
	{
		return 1; //Pic is bigger than LCD Size
	}
	Clr_Cs;   /* Cs low */ 
	Set_nRd;  /* RD high */
	while(yp < H+y_pos)
	{
		for(xp = x_pos; xp < (x_pos + W); xp++)
		{
			//LCD_WriteIndex(GLCD_X_CURSOR_REG);	/* selected LCD register */ 
			Clr_Rs;   /* RS low */		
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= (GLCD_X_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_X_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			//LCD_WriteData(MAX_X-1-xp);	/* Write register data */
			Set_Rs;  /* RS high */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((MAX_X-1-xp)<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = ((MAX_X-1-xp)<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			//LCD_WriteIndex(GLCD_Y_CURSOR_REG);	/* selected LCD register */ 
			Clr_Rs;   /* RS low */		
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= (GLCD_Y_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_Y_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
//			LCD_WriteData(MAX_Y-1-yp);	/* Write register data */
			Set_Rs;  /* RS high */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((MAX_Y-1-yp)<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = ((MAX_Y-1-yp)<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
//			LCD_WriteIndex(GLCD_DATA_REG);	/* selected LCD register */ 
			Clr_Rs;   /* RS low */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= (GLCD_DATA_REG<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_DATA_REG<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			//LCD_WriteData(bitmap[index++]);	/* Write register data */
			Set_Rs;  /* RS high */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((bitmap[index++])<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = ((bitmap[index++])<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
		}
		index= index+bitmap[0]-W;
		yp++;
	}
	Set_Cs;
	return 0;
}

uint8_t Draw_BMP_Fast(unsigned int x_pos, unsigned int y_pos, const unsigned short *bitmap)
{
	signed int xp = 0;
	signed int yp = (y_pos );
	unsigned long index = 0;
	signed int BMPW=bitmap[index++];
	signed int BMPH=bitmap[index++];
	
	if( BMPW > MAX_X || BMPH > MAX_Y )
	{
		return 1; //Pic is bigger than LCD Size
	}
	Clr_Cs;   /* Cs low */ 
	Set_nRd;  /* RD high */
	while(yp < (BMPH+y_pos))
	{
		for(xp = x_pos; xp < (x_pos + BMPW); xp++)
		{
			//LCD_WriteIndex(GLCD_X_CURSOR_REG);	/* selected LCD register */ 
			Clr_Rs;   /* RS low */		
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= (GLCD_X_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_X_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			//LCD_WriteData(MAX_X-1-xp);	/* Write register data */
			Set_Rs;  /* RS high */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((MAX_X-1-xp)<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = ((MAX_X-1-xp)<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			//LCD_WriteIndex(GLCD_Y_CURSOR_REG);	/* selected LCD register */ 
			Clr_Rs;   /* RS low */		
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= (GLCD_Y_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_Y_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
//			LCD_WriteData(MAX_Y-1-yp);	/* Write register data */
			Set_Rs;  /* RS high */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((MAX_Y-1-yp)<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = ((MAX_Y-1-yp)<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
//			LCD_WriteIndex(GLCD_DATA_REG);	/* selected LCD register */ 
			Clr_Rs;   /* RS low */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//
			LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_DATA_REG<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			//LCD_WriteData(bitmap[index++]);	/* Write register data */
			Set_Rs;  /* RS high */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((bitmap[index++])<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = ((bitmap[index++])<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
		}
		 yp++;
	}
	Set_Cs;
	return 0;
}

uint8_t Draw_BMP_Fast_NO_BackGround(unsigned int x_pos, unsigned int y_pos, const unsigned short *bitmap)
{
	signed int xp = 0;
	signed int yp = (y_pos );
	unsigned long index = 0;
	signed int BMPW=bitmap[index++];
	signed int BMPH=bitmap[index++];
	
	if( BMPW > MAX_X || BMPH > MAX_Y )
	{
		return 1; //Pic is bigger than LCD Size
	}
	Clr_Cs;   /* Cs low */ 
	Set_nRd;  /* RD high */
	while(yp < (BMPH+y_pos))
	{
		for(xp = x_pos; xp < (x_pos + BMPW); xp++)
		{
			//LCD_WriteIndex(GLCD_X_CURSOR_REG);	/* selected LCD register */ 
			Clr_Rs;   /* RS low */		
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= (GLCD_X_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_X_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			//LCD_WriteData(MAX_X-1-xp);	/* Write register data */
			Set_Rs;  /* RS high */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((MAX_X-1-xp)<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = ((MAX_X-1-xp)<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			//LCD_WriteIndex(GLCD_Y_CURSOR_REG);	/* selected LCD register */ 
			Clr_Rs;   /* RS low */		
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= (GLCD_Y_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_Y_CURSOR_REG<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
//			LCD_WriteData(MAX_Y-1-yp);	/* Write register data */
			Set_Rs;  /* RS high */
			//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
			//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((MAX_Y-1-yp)<<LCD_DATA_BIT_OFFSET);
			LCD_DATA_PORT->LCD_OUTDATA_REG = ((MAX_Y-1-yp)<<LCD_DATA_BIT_OFFSET);
			Clr_nWr;  /* Wr low */
			Set_nWr;  /* Wr high */
			if(bitmap[index]!=0)
			{
	//			LCD_WriteIndex(GLCD_DATA_REG);	/* selected LCD register */ 
				Clr_Rs;   /* RS low */
				//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
				//LCD_DATA_PORT->LCD_OUTDATA_REG |= (GLCD_DATA_REG<<LCD_DATA_BIT_OFFSET);
				LCD_DATA_PORT->LCD_OUTDATA_REG = (GLCD_DATA_REG<<LCD_DATA_BIT_OFFSET);
				Clr_nWr;  /* Wr low */
				Set_nWr;  /* Wr high */
				//LCD_WriteData(bitmap[index++]);	/* Write register data */
				Set_Rs;  /* RS high */
				//LCD_DATA_PORT->LCD_OUTDATA_REG &=~ (0xFFFF<<LCD_DATA_BIT_OFFSET);
				//LCD_DATA_PORT->LCD_OUTDATA_REG |= ((bitmap[index++])<<LCD_DATA_BIT_OFFSET);
				LCD_DATA_PORT->LCD_OUTDATA_REG = ((bitmap[index++])<<LCD_DATA_BIT_OFFSET);
				Clr_nWr;  /* Wr low */
				Set_nWr;  /* Wr high */
			}
			else	index++;
		}
		 yp++;
	}
	Set_Cs;
	return 0;
}

// http://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void Draw_Circle(uint16_t x0,uint16_t y0,uint16_t radius,uint16_t color )
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
 
    LCD_SetPoint(x0, y0 + radius, color); 
    LCD_SetPoint(x0, y0 - radius, color);
    LCD_SetPoint(x0 + radius, y0, color);
    LCD_SetPoint(x0 - radius, y0, color);
 
    while(x < y) 
    {
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;    
        LCD_SetPoint(x0 + x, y0 + y, color);
        LCD_SetPoint(x0 - x, y0 + y, color);
        LCD_SetPoint(x0 + x, y0 - y, color);
        LCD_SetPoint(x0 - x, y0 - y, color);
        LCD_SetPoint(x0 + y, y0 + x, color);
        LCD_SetPoint(x0 - y, y0 + x, color);
        LCD_SetPoint(x0 + y, y0 - x, color);
        LCD_SetPoint(x0 - y, y0 - x, color);
    }
}

void Draw_FillCircle(uint16_t x0,uint16_t y0,uint16_t radius,uint16_t color )
{
	uint16_t r;
	for(r=0;r<radius;r++) Draw_Circle( x0, y0, radius-r, color );
}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

