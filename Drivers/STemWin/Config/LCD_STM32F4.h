/*
 * LCD_STM32F4.h
 *
 *  Created on: 11.4.2012
 *  Author: martin, jopl
 */

/*
 * How to connect LCD with STM32F4Discovery:
 *
 * RS    ->  PD11
 * WR    ->  PD5
 * RD    ->  PD4
 * CS    ->  PD7
 * RESET ->  PC13
 *
 * DB0   ->  PD14
 * DB1   ->  PD15
 * DB2   ->  PD0
 * DB3   ->  PD1
 * DB4   ->  PE7
 * DB5   ->  PE8
 * DB6   ->  PE9
 * DB7   ->  PE10
 * DB8   ->  PE11
 * DB9   ->  PE12
 * DB10  ->  PE13
 * DB11  ->  PE14
 * DB12  ->  PE15
 * DB13  ->  PD8
 * DB14  ->  PD9
 * DB15  ->  PD10
 *
 * BL_CNT -> PD12
 *
 * TP_IRQ -> PB12
 * TP_SO -> PB14
 * TP_SI -> PB15
 * TP_SCK -> PB13
 * TP_CS -> PC6
 *
 *
 * Display coordinates:
 *  [x,y]
 *     [0,0]                                 [319, 0]
 *       -----------------------------------------
 *       |                                       |
 *       |                                       |
 *       |                                       |
 *       |                                       |
 *       |             TOUCH DISPLAY             |
 *       |                                       |
 *       |                                       |
 *       |                                       |
 *       |                                       |
 *       -----------------------------------------
 *   [239,0]                               [319,239]
 */

#ifndef LCD_STM32F4_H_
#define LCD_STM32F4_H_

/* Includes ******************************************************************/

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* Private defines */
#define LCD_CMD 	(uint32_t *)0x60000000
#define LCD_Data	(uint32_t *)0x60020000


#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7



/* Definitions ***************************************************************/

/*
 * FSMC Memory Bank 1: 0x60000000 to 6FFFFFFF
 * NAND bank selections:
 *   SECTION NAME      HADDR[17:16]  ADDRESS RANGE
 *   Address section   1X            0x020000-0x03FFFF
 *   Command section   01            0x010000-0x01FFFF
 *   Data section      00            0x000000-0x0FFFF
 */

#define LCD_BASE            ((uint32_t) (0x60000000 | 0x0001FFFE))
#define LCD                 ((LCD_TypeDef *) LCD_BASE)

#define GDDRAM_PREPARE      0x0022  /* Graphic Display Data RAM Register. */


/* Function Prototypes *******************************************************/


void Write_Command(uint16_t reg, uint16_t data);
void Write_CD(uint16_t reg, uint16_t data);

void LcdWriteReg(uint16_t reg);
void LcdWriteData(uint16_t data);

void Init_GPIO(void);
void Init_FSMC(void);
void Init_SysTick(void);
void TimingDelay_Decrement(void);
void _InitController(void);

void InitLCD_SSD1289(void);
void InitLCD_ILI9341(void);
void InitLCD_SSD1963(void);
void InitLCD_ILI9481(void);

#endif /* LCD_STM32F4_H_ */
