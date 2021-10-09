/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "LCD_STM32F4.h"
#include "GUI.h"
#include "DIALOG.h"
#include <stdio.h>
#include "ts.h"
#include <stdlib.h> //random
#include <stdbool.h>
#include "LCDConf.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern GUI_PID_STATE pstate;

GUI_CONST_STORAGE GUI_BITMAP bmlemmling_Cartoon_penguin_small;
GUI_CONST_STORAGE GUI_BITMAP bmledCircleOrange;
GUI_HSPRITE hSprite;

bool Right=true;
bool Down=true;

int xAccel=1;
int yAccel=1;

#ifdef ILI9341
#define TFTX 320
#define TFTY 240

#elif defined ILI9481
#define TFTX 480
#define TFTY 320

#elif defined SSD1963
#define TFTX 800
#define TFTY 480

#else
#define TFTX 320
#define TFTY 240

#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
static void MX_CRC_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	int count = 0;
	char countString[10];
	int xpos=0;
	int ypos=0;

	PROGBAR_Handle hProgbar;
	BUTTON_Handle hButton;

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_CRC_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();

  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);

  /* USER CODE BEGIN 2 */
	GUI_Init();

	GUI_SetFont(&GUI_Font8x16);
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();

	GUI_SetColor(GUI_CYAN);			//foreground or text color
	GUI_DispString("ECET260 Test Program\tStemwin ver: ");

	GUI_DispString(GUI_GetVersionString());
	GUI_DispString("\n\n\n");

	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetColor(GUI_WHITE);		//foreground or text color


#ifdef ILI9481
	GUI_DispStringHCenterAt("ECET260", 240, 50);

	GUI_DrawGradientH(15, 220, 465, 300, 0x0000FF, 0x00FFFF);

	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX); // Sets the default skin for new widgets

/* Create progress bar at location x = 10, y = 10, length = 219, height = 30 */
	hProgbar = PROGBAR_CreateEx(50, 235, 385, 50, 0, WM_CF_SHOW, 0, GUI_ID_PROGBAR0);

	// Create the button
	hButton = BUTTON_CreateEx(200, 120, 80, 60, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON0);

	//display ID code for TFT
	uint16_t regTemp[6];

	GUI_SetFont(&GUI_Font16B_ASCII);
	GUI_SetColor(GUI_YELLOW);

	LcdWriteReg(0xbf);
	HAL_SRAM_Read_16b(&hsram1, LCD_CMD, &regTemp, 6);
	GUI_DispHexAt(regTemp[3], 10, 20, 2);
	GUI_DispHex(regTemp[4], 2);

#endif

#ifdef ILI9341
	GUI_DispStringHCenterAt("ECET260", 160, 50);

	GUI_DrawGradientH(5, 150, 315, 235, 0x0000FF, 0x00FFFF);

	/* Create progress bar at location x = 10, y = 10, length = 219, height = 30 */
	hProgbar = PROGBAR_CreateEx(50, 180, 219, 30, 0, WM_CF_SHOW, 0, GUI_ID_PROGBAR0);


	// Create the button
	hButton = BUTTON_CreateEx(120, 100, 80, 60, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON0);


	//display ID code for TFT
	uint16_t regTemp[6];

	GUI_SetFont(&GUI_Font16B_ASCII);
	GUI_SetColor(GUI_YELLOW);

	LcdWriteReg(0xd3);
	HAL_SRAM_Read_16b(&hsram1, LCD_CMD, &regTemp, 4);
	GUI_DispHexAt(regTemp[2], 10, 20, 2);
	GUI_DispHex(regTemp[2], 2);

	GUI_Exec();

#endif

#ifdef	SSD1963
	GUI_SetFont(&GUI_FontD60x80);

	GUI_DrawGradientH(15, 300, 785, 450, 0x0000FF, 0x00FFFF);

	/* Create progress bar at location x = 50, y = 325, length = 700, height = 100 */
	hProgbar = PROGBAR_CreateEx(50, 325, 700, 100, 0, WM_CF_SHOW, 0, GUI_ID_PROGBAR0);


	// Create the button
	hButton = BUTTON_CreateEx(380, 200, 80, 60, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON0);



	//display ID code for SSD1963 TFT
	uint16_t regTemp[6];

	GUI_SetFont(&GUI_Font16B_ASCII);
	GUI_SetColor(GUI_YELLOW);

	LcdWriteReg(0xa1);
	HAL_SRAM_Read_16b(&hsram1, LCD_CMD, &regTemp, 5);
	GUI_DispHexAt(regTemp[0], 10, 20, 2);	//0x01
	GUI_DispHex(regTemp[1], 2);				//0x57
	GUI_DispHex(regTemp[2], 2);				//0x61
	GUI_DispHex(regTemp[3], 2);				//0x01
	GUI_DispHex(regTemp[4], 2);				//0xff

#endif

#ifdef SSD1289
	GUI_DispStringHCenterAt("ECET260", 160, 50);

	GUI_DrawGradientH(5, 150, 315, 235, 0x0000FF, 0x00FFFF);

	/* Create progress bar at location x = 10, y = 10, length = 219, height = 30 */
	hProgbar = PROGBAR_CreateEx(50, 180, 219, 30, 0, WM_CF_SHOW, 0, GUI_ID_PROGBAR0);


	// Create the button
	hButton = BUTTON_CreateEx(120, 100, 80, 60, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON0);


	//display ID code for TFT
	uint16_t regTemp[6];

	GUI_SetFont(&GUI_Font16B_ASCII);
	GUI_SetColor(GUI_YELLOW);

	LcdWriteReg(0x00);
	HAL_SRAM_Read_16b(&hsram1, LCD_CMD, regTemp, 1);
	GUI_DispHexAt(regTemp[0], 10, 20, 4);

#endif




	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX); // Sets the default skin for new widgets

	/* Set progress bar font */
	PROGBAR_SetFont(hProgbar, &GUI_Font8x16);

	PROGBAR_SetMinMax(hProgbar, 0, 99);

	/* Set progress bar text */
	PROGBAR_SetText(hProgbar, "...");



	BUTTON_SetFont(hButton, &GUI_Font8x16);
	// Set the button text
	BUTTON_SetText(hButton, "Test");

	GUI_Exec();

	hSprite = GUI_SPRITE_Create(&bmlemmling_Cartoon_penguin_small, xpos,ypos);
//	hSprite = GUI_SPRITE_Create(&bmledCircleOrange, 50,50);


	srand(uwTick); //init random number generator with systick
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  GUI_PID_GetCurrentState(&pstate);
//	  GUI_TOUCH_GetState(&pstate);  /* Get the touch position in pixel */
//	  GUI_TOUCH_Exec(); //call each time through to get new touchscreen input

	  GUI_SetFont(&GUI_Font8x16);

#ifdef ILI9481

		if (pstate.Pressed) {
			GUI_DispDecAt( pstate.x, 440,20,4);
			GUI_DispDecAt( pstate.y, 440,40,4);
			GUI_DispStringAt("-P-", 440,60);



		}
		else {
			GUI_DispDecAt( 0, 440,20,4);
			GUI_DispDecAt( 0, 440,40,4);
			GUI_DispStringAt("- -", 440,60);
		}

#elif SSD1963

		if (pstate.Pressed) {
			GUI_DispDecAt( pstate.x, 760,20,4);
			GUI_DispDecAt( pstate.y, 760,40,4);
			GUI_DispStringAt("-P-", 760,60);



		}
		else {
			GUI_DispDecAt( 0, 760,20,4);
			GUI_DispDecAt( 0, 760,40,4);
			GUI_DispStringAt("- -", 760,60);
		}

#else


	 		if (pstate.Pressed) {
	 			GUI_DispDecAt( pstate.x, 280,20,4);
	 			GUI_DispDecAt( pstate.y, 280,40,4);
	 			GUI_DispStringAt("-P-", 280,60);



	 		}
	 		else {
	 			GUI_DispDecAt( 0, 280,20,4);
	 			GUI_DispDecAt( 0, 280,40,4);
	 			GUI_DispStringAt("- -", 280,60);
	 		}
#endif
	 		if(BUTTON_IsPressed(hButton))
	 		{
	 			count++;
	 			count%=100;
	 			sprintf(countString, "%d", count);

	 			PROGBAR_SetValue(hProgbar, count);			//use dec value for bar position
	 		 	PROGBAR_SetText(hProgbar, countString); 	//use string for text on progress bar

	 		}


//Move sprite around the screen
//	 		if(Right){				//move in x direction
//	 			xpos+=xAccel;
//	 		}
//	 		else{
//	 			xpos-=xAccel;
//	 		}
	 		Right ? (xpos+=xAccel) : (xpos-=xAccel); //move in x direction
	 		if (xpos>TFTX-60){		//check for right edge
	 			Right=false;
	 			xAccel=rand()%6;
	 		}
	 		if (xpos<0){			//check for left edge
	 			Right=true;
	 			xAccel=rand()%6;
	 		}

	 		Down ? ypos++: ypos--;	//move in y direction
	 		if (ypos>TFTY-60){		//check bottom edge
	 			Down=false;
	 			yAccel=rand()%6;
	 		}
	 		if (ypos<0){			//check top edge
	 			Down=true;
	 			yAccel=rand()%6;
	 		}

	 		GUI_SPRITE_SetPosition(hSprite, xpos, ypos);
//end of move sprite code

	 		GUI_Delay(20);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* CRC init function */
static void MX_CRC_Init(void)
{

  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LCD_BL_Pin|OrangeLED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LCD_RESET_Pin */
  GPIO_InitStruct.Pin = LCD_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : User_Pin */
  GPIO_InitStruct.Pin = User_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(User_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TP_PEN_Pin */
  GPIO_InitStruct.Pin = TP_PEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TP_PEN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_BL_Pin OrangeLED_Pin */
  GPIO_InitStruct.Pin = LCD_BL_Pin|OrangeLED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{
  FSMC_NORSRAM_TimingTypeDef Timing;
  FSMC_NORSRAM_TimingTypeDef ExtTiming;

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 30;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */
  ExtTiming.AddressSetupTime = 5;
  ExtTiming.AddressHoldTime = 15;
  ExtTiming.DataSetupTime = 5;
  ExtTiming.BusTurnAroundDuration = 5;
  ExtTiming.CLKDivision = 16;
  ExtTiming.DataLatency = 17;
  ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

  if (HAL_SRAM_Init(&hsram1, &Timing, &ExtTiming) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
