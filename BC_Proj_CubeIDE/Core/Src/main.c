/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum states {
	IN = 0, OUT, SLEEP
} states;

typedef enum modes {
	COOLING = 0, HEATING, SYSTEM_OFF
} modes;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INITIAL_SLEEP_SET_POINT   21;
#define INITIAL_IN_SET_POINT      24;
#define GET_BACKGROUND_COLOR(cursor, index)   cursor == index ? Black : White
#define GET_TEXT_COLOR(cursor, index)   cursor == index ? White : Black
#define GET_STATE_STRING(state)    state == IN ? ("IN   ") : (state == OUT ? ("OUT  ") : ("SLEEP"))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern const unsigned short MainBG[0x12C00 + 2];
extern const unsigned short arrowup[900];
extern const unsigned short arrowdown[900];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
modes mode = COOLING;
char *temp_modes[3] = { "COOLING", "HEATING", "OFF" };
char *week_days[7] = { "S`stqc`x", "Stmc`x", "Mnmc`x", "Ttdrc`x", "Wdcmdrc`x",
		"Tgtqrc`x", "Fqhc`x" };
uint8_t days_temp[7][9];
states days_state[7][9];
uint8_t current_temperature = 18;

uint8_t real_hour = 7;
uint8_t real_weekday = 0;

uint8_t current_day_cursor = 1;
states current_hour_state = 0;
uint8_t current_week_day = 0;
uint8_t state_editor_mode = 0;
uint8_t current_setpoint = 0;

uint8_t is_button8_pressed = 0;
uint8_t is_button7_pressed = 0;
uint8_t is_button5_pressed = 0;
uint8_t is_button4_pressed = 0;
uint8_t is_button3_pressed = 0;
uint8_t is_button2_pressed = 0;
uint8_t is_button1_pressed = 0;

uint8_t in_comfort_temp;
uint8_t sleep_comfort_temp;
uint8_t temp_initialized = 0;
int counter = 0;
uint8_t Temp_byte1, Temp_byte2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void ThermoAdjucment(void);
void SyncStateAndSetPoints(void);
void ReadKey(void);
void LcdDailySchedule(uint8_t day);
void ShowTemperature(uint8_t temperature);
void LcdPageOne(void);
uint16_t slen(const char*);
void add_txt(char*, char*);
char* my_utoa(unsigned, char*);
char* my_ftoa(float, char*);
uint8_t GetHourIndex(uint8_t hour);
void LcdGiveFeedback(void);
void ShowMode(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*Temp Begin*/
void delayus(uint32_t us) {
//	volatile uint32_t counter = 8 * us;
//	while (counter--)
//		;
	uint32_t counterval = 0;
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim3))<2*us);
}
void LH_signal(uint32_t L_time, uint32_t H_time) {
	HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);
	delayus(L_time); //From pullup_HIGH to GND_LOW:---___
	HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
	delayus(H_time); //From GND_LOW to pullup_HIGH:___---
}

void write_bit(uint8_t bit) {
	if (bit == 0)
		LH_signal(60, 5);
	else
		LH_signal(5, 60);
}

void write_byte(uint8_t data) {
	for (uint8_t i = 0; i < 8; i++)
		write_bit(data >> i & 1);
}

static void A9_as_INPUT(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void A9_as_OUTPUT(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}

uint8_t read_bit(void) {
	uint8_t bit = 0;
	LH_signal(1, 10);
	A9_as_INPUT();
	bit = (HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin) ? 1 : 0);
	delayus(40);
	A9_as_OUTPUT();
	return bit;
}

uint8_t read_byte(void) {
	uint8_t data = 0;
	for (uint8_t i = 0; i < 8; i++)
		data += read_bit() << i;
	return data;
}

void A9_wait_for_1(uint32_t time) {
	A9_as_INPUT();
	delayus(time);
	while (HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin) == 0)
		;
	A9_as_OUTPUT();
}

void reset(void) {
	LH_signal(500, 500);
}
uint8_t get_presence(void) {
	uint8_t flag = 1;
	LH_signal(500, 0);
	A9_as_INPUT();
	delayus(60); //look for GND_LOW = DS18B20 exists
	flag = (HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin) ? 0 : 1); //not ?1:0
	A9_as_OUTPUT();
	delayus(400);
	if (flag)
		return 1;
	else
		return 0;
}

float get_temperature(void) {
	uint8_t pad_data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //9 Byte
	reset();
	write_byte(0xCC); //Skip ROM [CCh]
	write_byte(0x44); //Convert Temperature [44h]
	A9_wait_for_1(20);
	reset();
	write_byte(0xCC); //Skip ROM [CCh]
	write_byte(0xBE); //Read Scratchpad [BEh]
	for (uint8_t i = 0; i < 9; i++)
		pad_data[i] = read_byte(); //factor out 1/16 and remember 1/16 != 1/16.0
	uint16_t x = (pad_data[1] << 8) + pad_data[0];
	if ((pad_data[1] >> 7) == 1) {
		x -= 1;
		x = ~x;
		return x / -16.0;
	} else
		return x / 16.0;
}

void send_ID(void) {
	uint8_t id_data[] = { 40, 25, 0, 0, 120, 2, 0, 136 }; //8 Byte ID
	reset();
	write_byte(0x55); //Match ROM [55h]
	for (uint8_t i = 0; i < 8; i++)
		write_byte(id_data[i]); //id_data[0] = 40 = 0x28
}

float get_temperature_with_ID(void) {
	uint8_t pad_data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //9 Byte
	reset();
	send_ID();
	write_byte(0x44); //Convert Temperature [44h]
	A9_wait_for_1(20);
	reset();
	send_ID();
	write_byte(0xBE); //Read Scratchpad [BEh]
	for (uint8_t i = 0; i < 9; i++)
		pad_data[i] = read_byte(); //factor out 1/16 and remember 1/16 != 1/16.0
	return ((pad_data[1] << 8) + pad_data[0]) / 16.0; //DS18B20.pdf: val of bit0: 2^(-4) = 1/16
}

uint16_t slen(const char *s) {
	uint16_t i;
	for (i = 0; s[i] != 0; i++)
		;
	return i; //s[0] not 0 then i=1;
}
void add_txt(char *out, char *in) {
	while (*out != 0)
		out++;
	while (*in != 0) {
		*out++ = *in++;
	}
	*out = 0;
}
char* my_utoa(unsigned val, char *str) {
	//static char buffer[10];
	char *cp = str;
	unsigned v;
	char c;
	v = val;
	do {
		v /= 10;
		cp++;
	} while (v != 0);
	*cp-- = 0;
	do {
		c = val % 10;
		val /= 10;
		c += '0';
		*cp-- = c;
	} while (val != 0);
	return cp;
}
char* my_ftoa(float val, char *str) {
	char *cp;
	cp = str;
	int v, v0, rest, rest0;
	char c;
	if (val < 0) { // cp=0
		val = -val; //cp[0 ][1 ][2][3][4][5][6][7][8][9]
		*cp++ = '-'; // [0: -] cp=1
	}
	v0 = (int) val;
	v = v0;
	//rest0=(int)((val-(int)val)*100000000); rest = rest0;
	rest0 = (int) ((val - (int) val) * 10000);
	rest = rest0;
	do {
		v /= 10;
		cp++;
	} while (v != 0);
	do {
		rest /= 10;
		cp++;
	} while (rest != 0);
	cp++; //wegen ','
	*cp-- = 0;
	do {
		c = rest0 % 10;
		rest0 /= 10;
		c += '0';
		*cp-- = c;
	} while (rest0 != 0);
	*cp-- = ',';
	do {
		c = v0 % 10;
		v0 /= 10;
		c += '0';
		*cp-- = c;
	} while (v0 != 0);
	return cp;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		counter++;
		float f = get_temperature();
		current_temperature = (uint16_t) f;
		if (counter > 10) {
			if (is_button2_pressed == 0 && is_button4_pressed == 0) {
				ShowMode();
				ShowTemperature(current_temperature);
			}
			counter = 0;
		}
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	in_comfort_temp = INITIAL_IN_SET_POINT
	;
	sleep_comfort_temp = INITIAL_SLEEP_SET_POINT
	;

	for (uint8_t day = 0; day < 7; day++) {
		days_state[day][0] = SLEEP;
		days_state[day][1] = days_state[day][6] = days_state[day][7] =
				days_state[day][8] = IN;
		days_state[day][2] = days_state[day][3] = days_state[day][4] =
				days_state[day][5] = OUT;
	}

	SyncStateAndSetPoints();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	if (HAL_TIM_Base_Start(&htim3) == HAL_ERROR) {
		while(1);
	}
	HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  Draw_BMP_Fast(0, 0, MainBG);
	HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_Port, LCD_BACKLIGHT_Pin, 1);

	LCD_Initializtion();
	LCD_Clear(Black);
	char str[50];
	sprintf(str, "%d", 65464);
	ThermoAdjucment();
	LcdPageOne();

	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		ThermoAdjucment();
		ReadKey();
		if (is_button8_pressed == 1) {
			current_week_day = (current_week_day + 1) % 7;
			LCD_Clear(White);
			is_button8_pressed = 0;
		}

		if (is_button2_pressed == 1) {
			LcdDailySchedule(current_week_day);
			is_button4_pressed = 0;
		}

		if (is_button4_pressed == 1) {
			LcdGiveFeedback();
			is_button2_pressed = 0;
		}

		if (is_button7_pressed == 1) {
			is_button2_pressed = 0;
			is_button7_pressed = 0;
			is_button4_pressed = 0;
			LcdPageOne();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 36000-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 36-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65534;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED8_Pin|LED7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DS18B20_Pin|LED5_Pin|LED1_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TOUCHPWR_Pin|LEDENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, BUZZER_Pin|LCD_BACKLIGHT_Pin|LED2_Pin|LED4_Pin
                          |LED6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED8_Pin LED7_Pin */
  GPIO_InitStruct.Pin = LED8_Pin|LED7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DS18B20_Pin LED5_Pin LED3_Pin */
  GPIO_InitStruct.Pin = DS18B20_Pin|LED5_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TOUCHPWR_Pin */
  GPIO_InitStruct.Pin = TOUCHPWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TOUCHPWR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TOUCH8_Pin TOUCH6_Pin TOUCH4_Pin TOUCH2_Pin
                           TOUCH1_Pin TOUCH7_Pin TOUCH5_Pin TOUCH3_Pin */
  GPIO_InitStruct.Pin = TOUCH8_Pin|TOUCH6_Pin|TOUCH4_Pin|TOUCH2_Pin
                          |TOUCH1_Pin|TOUCH7_Pin|TOUCH5_Pin|TOUCH3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUZZER_Pin LED2_Pin LED4_Pin LED6_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin|LED2_Pin|LED4_Pin|LED6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_BACKLIGHT_Pin */
  GPIO_InitStruct.Pin = LCD_BACKLIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_BACKLIGHT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LEDENABLE_Pin */
  GPIO_InitStruct.Pin = LEDENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LEDENABLE_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void SetTemperatureBasedOnInput(uint8_t ideal_temperature) {
	if (current_temperature > ideal_temperature) {
		mode = COOLING;
	} else if (current_temperature < ideal_temperature) {
		mode = HEATING;
	}
}

uint8_t GetHourIndex(uint8_t hour) {
	uint8_t hour_index = 0;
	if (hour >= 6 && hour < 8)
		hour_index = 1;
	else if (hour >= 8 && hour < 10)
		hour_index = 2;
	else if (hour >= 10 && hour < 12)
		hour_index = 3;
	else if (hour >= 12 && hour < 14)
		hour_index = 4;
	else if (hour >= 14 && hour < 16)
		hour_index = 5;
	else if (hour >= 16 && hour < 18)
		hour_index = 6;
	else if (hour >= 18 && hour < 20)
		hour_index = 7;
	else if (hour >= 20 && hour < 22)
		hour_index = 8;
	else
		hour_index = 0;

	return hour_index;
}

void ThermoAdjucment(void) {
	uint8_t hour_index = GetHourIndex(real_hour);
	if (days_state[real_weekday][hour_index] == OUT) {
		mode = SYSTEM_OFF;
	} else if (days_state[real_weekday][hour_index] == IN) {
		SetTemperatureBasedOnInput(in_comfort_temp);
	} else {
		SetTemperatureBasedOnInput(sleep_comfort_temp);
	}
}

void SyncStateAndSetPoints(void) {
	for (uint8_t day = 0; day < 7; day++) {
		for (uint8_t hour = 0; hour < 9; hour++) {
			switch (days_state[day][hour]) {
			case IN:
				days_temp[day][hour] = in_comfort_temp;
				break;
			case SLEEP:
				days_temp[day][hour] = sleep_comfort_temp;
				break;
			case OUT:
				days_temp[day][hour] = 0;
				break;
			}
		}
	}
}

void ReadKey(void) {
	/********Read Key1********/
	if (HAL_GPIO_ReadPin(TOUCH1_GPIO_Port, TOUCH1_Pin) == 0) {
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(TOUCH1_GPIO_Port, TOUCH1_Pin) == 0) {
			is_button1_pressed = 1;
		}
	}
	/********Read Key2********/
	if (HAL_GPIO_ReadPin(TOUCH2_GPIO_Port, TOUCH2_Pin) == 0) {
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(TOUCH2_GPIO_Port, TOUCH2_Pin) == 0) {
			LCD_Clear(White);
			is_button2_pressed = 1;
		}
	}
	/********Read Key3********/
	if (HAL_GPIO_ReadPin(TOUCH3_GPIO_Port, TOUCH3_Pin) == 0) {
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(TOUCH3_GPIO_Port, TOUCH3_Pin) == 0) {
			is_button3_pressed = 1;
		}
	}
	/********Read Key4********/
	if (HAL_GPIO_ReadPin(TOUCH4_GPIO_Port, TOUCH4_Pin) == 0) {
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(TOUCH4_GPIO_Port, TOUCH4_Pin) == 0) {
			LCD_Clear(White);
			is_button4_pressed = 1;
			current_setpoint = days_temp[real_weekday][GetHourIndex(real_hour)];
		}
	}
	/********Read Key5********/
	if (HAL_GPIO_ReadPin(TOUCH5_GPIO_Port, TOUCH5_Pin) == 0) {
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(TOUCH5_GPIO_Port, TOUCH5_Pin) == 0) {
			is_button5_pressed = 1;
		}
	}
	/********Read Key6********/
	if (HAL_GPIO_ReadPin(TOUCH6_GPIO_Port, TOUCH6_Pin) == 0) {
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(TOUCH6_GPIO_Port, TOUCH6_Pin) == 0) {
			LCD_Clear(Green);
			GUI_TextArial_30pt(86, 20, "EYDGC", White, NO_BG);
		}
	}
	/********Read Key7********/
	if (HAL_GPIO_ReadPin(TOUCH7_GPIO_Port, TOUCH7_Pin) == 0) {
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(TOUCH7_GPIO_Port, TOUCH7_Pin) == 0) {
			is_button7_pressed = 1;
		}
	}
	/********Read Key8********/
	if (HAL_GPIO_ReadPin(TOUCH8_GPIO_Port, TOUCH8_Pin) == 0) {
		HAL_Delay(50);
		if (HAL_GPIO_ReadPin(TOUCH8_GPIO_Port, TOUCH8_Pin) == 0) {
			is_button8_pressed = 1;
		}
	}
}

void ShowTemperature(uint8_t temperature) {
	char str[50];
	sprintf(str, "%d", temperature);
	Draw_BMP_Slice(86, 17, 42, 34, MainBG);
	GUI_TextArial_30pt(86, 20, str, White, NO_BG);
	PutCharArial_22pt(128, 20, '~' + 1, White, NO_BG); //'~'+1 -> degree
	PutCharArial_30pt(138, 20, 'c', White, NO_BG);
}

void ShowDateAndTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
		uint8_t minute) {
	char str[70];
	sprintf(str, "%d/%d/%d  %d:%d", year, month, day, hour, minute);
	GUI_TextArial_18pt(35, 250, str, White, NO_BG);
}

void ShowMode(void) {
//	Draw_BMP_Slice(70, 85, 30, 103, MainBG);
	GUI_TextArial_18pt(70, 90, temp_modes[(uint8_t) mode], White, NO_BG);
}

void LcdPageOne(void) {
	LCD_Clear(Black);
	Draw_BMP_Fast(0, 0, MainBG);
	ShowTemperature(current_temperature);
	GUI_TextArial_18pt(70, 90, temp_modes[(uint8_t) mode], White, NO_BG);
	ShowDateAndTime(2023, 10, 23, 13, 7);
}

void table_item(uint8_t day, uint8_t clk, uint16_t y, uint16_t l_y,
		uint8_t is_am, uint8_t has_line, uint8_t index) {
	char str[20];
	if (is_am == 1)
		sprintf(str, "%dAM", clk);
	else
		sprintf(str, "%dPM", clk);
	GUI_TextArial_18pt(10, y, str, GET_TEXT_COLOR(current_day_cursor, index),
			GET_BACKGROUND_COLOR(current_day_cursor, index));
	sprintf(str, "%d", days_temp[day][index]);
	GUI_TextArial_18pt(100, y, str, Black, NO_BG);
	if (state_editor_mode == 1 && current_day_cursor == index) {
		GUI_TextArial_18pt(170, y, GET_STATE_STRING(current_hour_state),
				GET_TEXT_COLOR(current_day_cursor, index),
				GET_BACKGROUND_COLOR(current_day_cursor, index));
	} else {
		GUI_TextArial_18pt(170, y, GET_STATE_STRING(days_state[day][index]),
		Black,
		White);
	}
	if (has_line == 1)
		LCD_DrawLine(0, l_y, 300, l_y, Black);
}

void LcdDailySchedule(uint8_t day) {
	GUI_TextArial_18pt(50, 5, week_days[day], Black, NO_BG);
	char str[20];
	if (is_button1_pressed == 1) {
		if (state_editor_mode == 0)
			current_day_cursor = (current_day_cursor - 1) % 9;
		else
			current_hour_state = (current_hour_state - 1) % 3;
		is_button1_pressed = 0;
	}

	if (is_button3_pressed == 1) {
		if (state_editor_mode == 0)
			current_day_cursor = (current_day_cursor + 1) % 9;
		else
			current_hour_state = (current_hour_state + 1) % 3;
		is_button3_pressed = 0;
	}

	if (is_button5_pressed == 1) {
		if (state_editor_mode == 0) {
			current_hour_state = days_state[day][current_day_cursor];
			state_editor_mode = 1;
		} else {
			days_state[day][current_day_cursor] = current_hour_state;
			SyncStateAndSetPoints();
			state_editor_mode = 0;
		}
		is_button5_pressed = 0;
	}
	table_item(day, 6, 30, 55, 1, 1, 1);

	table_item(day, 8, 60, 85, 1, 1, 2);

	table_item(day, 10, 90, 115, 1, 1, 3);

	table_item(day, 12, 120, 145, 0, 1, 4);

	table_item(day, 2, 150, 175, 0, 1, 5);

	table_item(day, 4, 180, 205, 0, 1, 6);

	table_item(day, 6, 210, 235, 0, 1, 7);

	table_item(day, 8, 240, 265, 0, 1, 8);

	table_item(day, 10, 270, 295, 0, 0, 0);
}

void LcdGiveFeedback(void) {
	if (is_button1_pressed == 1) {
		current_setpoint++;
		is_button1_pressed = 0;
	}

	if (is_button3_pressed == 1) {
		current_setpoint--;
		is_button3_pressed = 0;
	}

	if (is_button5_pressed == 1) {
		states current_state = days_state[real_weekday][GetHourIndex(real_hour)];
		if (current_state == IN) {
			in_comfort_temp = current_setpoint;
		} else if (current_state == SLEEP) {
			sleep_comfort_temp = current_setpoint;
		}
		SyncStateAndSetPoints();
		is_button5_pressed = 0;
	}
	char str[20];
	sprintf(str, "%d", current_setpoint);
	GUI_TextArial_30pt(100, 150, str, Black, White);
	Draw_BMP_Fast(105, 100, arrowup);
	Draw_BMP_Fast(105, 200, arrowdown);
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
	__disable_irq();
	while (1) {
	}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
