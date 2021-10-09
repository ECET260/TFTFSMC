/*
 * LCD_STM32F4.c
 *
 *  Created on: 11.4.2012
 *      Author: martin
 */

/* Includes ******************************************************************/

#include "stm32f4xx_hal.h"
#include "LCD_STM32F4.h"

//#include "stm32f4xx_conf.h"


#include "LCDConf.h"
#include "main.h"



/* Private Variables *********************************************************/
extern SRAM_HandleTypeDef hsram1;


/* Functions *****************************************************************/


/*
 * Reset and Initialize Display.
 */

void Init_LCD2(void)
{
  /* Reset */


  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(30);
  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(10);

  /* Display ON Sequence (data sheet page 72 Solomon Systech SSD1289 Rev1.3 Apr2007) */

  Write_Command(0x0007, 0x0021);
  Write_Command(0x0000, 0x0001);
  Write_Command(0x0007, 0x0023);
  Write_Command(0x0010, 0x0000);  /* Exit Sleep Mode */
  HAL_Delay(30);
  Write_Command(0x0007, 0x0033);

  /*
   * Entry Mode R11h = 6018h
   *
   * DFM1 = 1, DFM0 = 1 => 65k Color Mode
   * ID0 = 1, AM = 1    => the way of automatic incrementing
   *                       of address counter in RAM
   */
  Write_Command(0x0011, 0x6018);
  Write_Command(0x0002, 0x0600);  /* LCD driver AC Setting */

  /* Initialize other Registers */

  /*
   * Device Output Control R01h = 2B3Fh
   *
   * REV = 1            => character and graphics are reversed
   * BGR = 1            => BGR color is assigned from S0
   * TB  = 1            => sets gate output sequence (see datasheet page 29)
   * MUX[8, 5:0]        => specify number of lines for the LCD driver
   */
  Write_Command(0x0001, 0x2B3F);



}

//adapted from http://forum.arduino.cc/index.php?topic=153663.0;nowap
//solved SSD189 ID code 0x8999 blurry fonts issue.
void InitLCD_SSD1289()
{
	  /* Reset */

	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	  HAL_Delay(30);
	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	  HAL_Delay(10);

	  Write_CD(0x00,0x0001); // oscillator bit0 OSCEN: 1 = on, 0 = off
	  //    TFT_Orientation(_orientation);
	  Write_CD(0x01,0x3B3F); // driver output control
	  	  	  	  	  	  	 // REV = 1: Displays all character and graphics display sections with reversal
	  	  	  	  	  	  	 // BGR = 1: <B><G><R> color is assigned from S0
	  	  	  	  	  	  	 // TB = 1: G0 shifts to G319
	  Write_CD(0x02,0x0600); // driving waveform control
	  Write_CD(0x0C,0x0007); // power control 2 VRC[2:0]: Adjust VCIX2 output voltage to 5.8V
	  Write_CD(0x0D,0x0006); // power control 3 VRH[3:0]: Set amplitude magnification of V LCD63 to Vref x 2.020
	  Write_CD(0x0E,0x3200); // power control 4
	  Write_CD(0x1E,0x00BB); // power control 5
	  Write_CD(0x03,0x6A64); // power control
	  Write_CD(0x0F,0x0000); // gate scan position
	  Write_CD(0x44,0xEF00); // horizontal ram address position
	  Write_CD(0x45,0x0000); // vertical ram address position
	  Write_CD(0x46,0x013F); // vertical ram address position

	  Write_CD(0x30, 0x0000); // gamma control
	  Write_CD(0x31, 0x0706); //
	  Write_CD(0x32, 0x0206); //
	  Write_CD(0x33, 0x0300); //
	  Write_CD(0x34, 0x0002); //
	  Write_CD(0x35, 0x0000); //
	  Write_CD(0x36, 0x0707); //
	  Write_CD(0x37, 0x0200); //
	  Write_CD(0x3A, 0x0908); //
	  Write_CD(0x3B, 0x0F0D); //

//	  Write_CD(0x28,0x0006);
	  Write_CD(0x10,0x0000);  // sleep mode: 0 = exit, 1 = enter
	  HAL_Delay(50);
	  Write_CD(0x11,0x6070);  // entry mode
	  Write_CD(0x07,0x0033);  // display control
	  HAL_Delay(100);
}

void InitLCD_SSD1289_2()
{
	  /* Reset */

	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	  HAL_Delay(30);
	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	  HAL_Delay(10);

    Write_CD(0x0000, 0x0001 ); // oscillator: 1 = on, 0 = off
    Write_CD(0x0003, 0xA8A4 ); // power control
    Write_CD(0x000C, 0x0000 ); // power control 2
    Write_CD(0x000D, 0x080C ); // power control 3
    Write_CD(0x000E, 0x2B00 ); // power control 4
    Write_CD(0x001E, 0x00B7 ); // power control 5
    Write_CD(0x0002, 0x0600 ); // driving waveform control
    Write_CD(0x0010, 0x0000 ); // SLP bit 0:sleep mode: 0 = exit, 1 = enter
//    TFT_Orientation(_orientation);
    Write_CD(0x0001, 0x2B3F ); // driver output control
//    Write_CD(0x0011, 0x6078 ); // entry mode
    Write_CD(0x0011, 0x6830); // por entry mode
    //
    Write_CD(0x0005, 0x0000 ); // compare register
    Write_CD(0x0006, 0x0000 ); // compare register
    Write_CD(0x0016, 0xEF1C ); // horizontal porch
    Write_CD(0x0017, 0x0003 ); // vertical porch
    Write_CD(0x0007, 0x0233 ); // display control
    Write_CD(0x000B, 0x0000 ); // frame cycle control
    Write_CD(0x000F, 0x0000 ); // gate scan position
    Write_CD(0x0041, 0x0000 ); // vertical scroll control
    Write_CD(0x0042, 0x0000 ); // vertical scroll control
    Write_CD(0x0048, 0x0000 ); // 1st screen driving position
    Write_CD(0x0049, 0x013F ); // 1st screen driving position
    Write_CD(0x004A, 0x0000 ); // 2nd screen driving position
    Write_CD(0x004B, 0x013F ); // 2nd screen driving position
    Write_CD(0x0044, 0xEF00 ); // horizontal ram address position
    Write_CD(0x0045, 0x0000 ); // vertical ram address position
    Write_CD(0x0046, 0x013F ); // vertical ram address position
    Write_CD(0x0030, 0x0707 ); // gamma control
    Write_CD(0x0031, 0x0204 ); // gamma control
    Write_CD(0x0032, 0x0204 ); // gamma control
    Write_CD(0x0033, 0x0502 ); // gamma control
    Write_CD(0x0034, 0x0507 ); // gamma control
    Write_CD(0x0035, 0x0204 ); // gamma control
    Write_CD(0x0036, 0x0204 ); // gamma control
    Write_CD(0x0037, 0x0502 ); // gamma control
    Write_CD(0x003A, 0x0302 ); // gamma control
    Write_CD(0x003B, 0x0302 ); // gamma control
    Write_CD(0x0023, 0x0000 ); // GRAM write mask for red and green pins
    Write_CD(0x0024, 0x0000 ); // GRAM write mask for blue pins
    Write_CD(0x0025, 0x8000 ); // frame frequency control
    Write_CD(0x004e, 0x0000 ); // ram address set
    Write_CD(0x004f, 0x0000 ); // ram address set
 //   Write_Command( 0x0022 ); // write GRAM
//    HAL_SRAM_Write_16b(&hsram1, LCD_CMD, 0x0022, 1);
}

/*
 * Write to LCD RAM.
 */

void Write_Command(uint16_t reg, uint16_t data)
{
//	*LCD_CMD = reg;
//	*LCD_Data = data;
	HAL_SRAM_Write_16b(&hsram1, LCD_CMD, &reg, 1);
  	HAL_SRAM_Write_16b(&hsram1, LCD_Data, &data, 1);
}

void Write_CD(uint16_t reg, uint16_t data)
{
	Write_Command(reg, data);
}

void InitLCD_ILI9341(void) {
	/* Force reset */
	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	  HAL_Delay(30);
	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	  HAL_Delay(10);

	/* Delay for RST response */
	HAL_Delay(200);

	/* Software reset */
	LcdWriteReg(ILI9341_RESET);
	HAL_Delay(500);

	LcdWriteReg(ILI9341_POWERA);
	LcdWriteData(0x39);
	LcdWriteData(0x2C);
	LcdWriteData(0x00);
	LcdWriteData(0x34);
	LcdWriteData(0x02);
	LcdWriteReg(ILI9341_POWERB);
	LcdWriteData(0x00);
	LcdWriteData(0xC1);
	LcdWriteData(0x30);
	LcdWriteReg(ILI9341_DTCA);
	LcdWriteData(0x85);
	LcdWriteData(0x00);
	LcdWriteData(0x78);
	LcdWriteReg(ILI9341_DTCB);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteReg(ILI9341_POWER_SEQ);
	LcdWriteData(0x64);
	LcdWriteData(0x03);
	LcdWriteData(0x12);
	LcdWriteData(0x81);
	LcdWriteReg(ILI9341_PRC);
	LcdWriteData(0x20);
	LcdWriteReg(ILI9341_POWER1); //4.6V
	LcdWriteData(0x23);
	LcdWriteReg(ILI9341_POWER2);
	LcdWriteData(0x10);
	LcdWriteReg(ILI9341_VCOM1);
	LcdWriteData(0x3E);		//vcomh 4.250V
	LcdWriteData(0x28);		//vcoml -1.500V
	LcdWriteReg(ILI9341_VCOM2);
	LcdWriteData(0x86);
	LcdWriteReg(ILI9341_MAC); //memory access control
	LcdWriteData(0x48);	//48
	LcdWriteReg(ILI9341_PIXEL_FORMAT);
	LcdWriteData(0x55);	//rgb  16bits/pixel mcu 16bits/pixel
	LcdWriteReg(ILI9341_FRC); //Frame Rate Cntrol
	LcdWriteData(0x00); //diva freq == fosc
	LcdWriteData(0x18);	//79Hz refresh
	LcdWriteReg(ILI9341_DFC);//display function control
	LcdWriteData(0x08);
	LcdWriteData(0xA2); //normally white, gs - g1-g320, ss - s720-s1
	LcdWriteData(0x27);
	LcdWriteReg(ILI9341_3GAMMA_EN);
	LcdWriteData(0x00);
	LcdWriteReg(ILI9341_COLUMN_ADDR);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0xEF);
	LcdWriteReg(ILI9341_PAGE_ADDR);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0x01);
	LcdWriteData(0x3F);
	LcdWriteReg(ILI9341_GAMMA);
	LcdWriteData(0x01);
	LcdWriteReg(ILI9341_PGAMMA);
	LcdWriteData(0x0F);
	LcdWriteData(0x31);
	LcdWriteData(0x2B);
	LcdWriteData(0x0C);
	LcdWriteData(0x0E);
	LcdWriteData(0x08);
	LcdWriteData(0x4E);
	LcdWriteData(0xF1);
	LcdWriteData(0x37);
	LcdWriteData(0x07);
	LcdWriteData(0x10);
	LcdWriteData(0x03);
	LcdWriteData(0x0E);
	LcdWriteData(0x09);
	LcdWriteData(0x00);
	LcdWriteReg(ILI9341_NGAMMA);
	LcdWriteData(0x00);
	LcdWriteData(0x0E);
	LcdWriteData(0x14);
	LcdWriteData(0x03);
	LcdWriteData(0x11);
	LcdWriteData(0x07);
	LcdWriteData(0x31);
	LcdWriteData(0xC1);
	LcdWriteData(0x48);
	LcdWriteData(0x08);
	LcdWriteData(0x0F);
	LcdWriteData(0x0C);
	LcdWriteData(0x31);
	LcdWriteData(0x36);
	LcdWriteData(0x0F);
	LcdWriteReg(ILI9341_SLEEP_OUT);

	HAL_Delay(1000);

	LcdWriteReg(ILI9341_DISPLAY_ON);
	LcdWriteReg(ILI9341_GRAM);
}


void InitLCD_SSD1963(void)
{
	//800x480
	  /* Reset */

	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	  HAL_Delay(30);
	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	  HAL_Delay(10);

    //Set the Multiplier & Divider of PLL
	LcdWriteReg(0xE2);    //PLL multiplier, set PLL clock to 120M
    LcdWriteData(0x0023);        //N=0x36 for 6.5M, 0x23 for 10M crystal
    LcdWriteData(0x0002);
    LcdWriteData(0x0054);
    //Set & start the PLL.
    LcdWriteReg(0xE0);  // PLL enable
    LcdWriteData(0x0001);
    HAL_Delay(100);
    LcdWriteReg(0xE0);
    LcdWriteData(0x0003);        // now, use PLL output as system clock
    HAL_Delay(10);
    // software reset
    LcdWriteReg(0x01);
    HAL_Delay(20);
    //Set the LSHIFT (pixel clock) frequency
    LcdWriteReg(0xE6);    //PLL setting for PCLK, depends on resolution
    LcdWriteData(0x0003);
    LcdWriteData(0x0033);
    LcdWriteData(0x0033);
    //Set the LCD panel mode and resolution
    LcdWriteReg(0xB0);    //LCD SPECIFICATION
    LcdWriteData(0x0000); //24 bit TFT panel 0x20
    LcdWriteData(0x0000); //Hsync+Vsync +DE mode  TFT mode
    LcdWriteData(799>>8);  //Set HDP
    LcdWriteData(799&0x00FF);
  LcdWriteData(479>>8);  //Set VDP
    LcdWriteData(479&0x00FF);
  LcdWriteData(0x0000); //Set RGB sequence for Even & Odd lines
  //Set front porch and back porch
  LcdWriteReg(0xB4);    //HSYNC Parameters
    LcdWriteData(0x04);  //Set HT
    LcdWriteData(0x1F);
    LcdWriteData(0x00);  //Set HPS
    LcdWriteData(0xD2);
    LcdWriteData(0x00);     //Set HPW
    LcdWriteData(0x00);  //Set LPS
    LcdWriteData(0x00);
    LcdWriteData(0x00);  //Set LPSPP
//Set the vertical blanking interval between last scan line and next LFRAME pulse
    LcdWriteReg(0xB6);    //VSYNC Parameters
    LcdWriteData(0x02);   //Set VT
    LcdWriteData(0x0C);
    LcdWriteData(0x00);  //Set VPS
    LcdWriteData(0x22);
    LcdWriteData(0x00);        //Set VPW
    LcdWriteData(0x00);  //Set FPS
    LcdWriteData(0x00);

//Set the GPIOs configuration. If the GPIOs are not used for LCD, set the direction. Otherwise, they are toggled with
//LCD signals by 0xC0 – 0xCF.

    LcdWriteReg(0xB8);
    LcdWriteData(0x000F);    //GPIO is controlled by host GPIO[3:0]=output   GPIO[0]=1  LCD ON  GPIO[0]=1  LCD OFF
    LcdWriteData(0x0001);    //GPIO0 normal
//Set GPIO value for GPIO configured as output
    LcdWriteReg(0xBA);
    LcdWriteData(0x0001);    //GPIO[0] out 1 --- LCD display on/off control PIN
//Set the read order from host processor to frame buffer by A[7:5] and A[3] and from frame buffer to the display panel by
//A[2:0] and A[4].

    LcdWriteReg(0x36); //rotation
    LcdWriteData(0x0000);//RGB mode


    LcdWriteReg(0x3A); //Set the current pixel format for RGB image data
    LcdWriteData(0x0050);//16-bit/pixel

    LcdWriteReg(0xF0); //Pixel Data Interface Format
    LcdWriteData(0x0003);//16-bit(565 format) data

  //Set the image post processor
    LcdWriteReg(0xBC);
    LcdWriteData(0x0040);//contrast value
    LcdWriteData(0x0080);//brightness value
    LcdWriteData(0x0040);//saturation value
    LcdWriteData(0x0001);//Post Processor Enable

    HAL_Delay(5);
  //Show the image on the display panel
    LcdWriteReg(0x29); //display on


    LcdWriteReg(0xBE); //set PWM for B/L
    LcdWriteData(0x0006);  //Set the PWM frequency
    LcdWriteData(0x0080);  //Set the PWM duty cycle
    LcdWriteData(0x0001);  //PWM configuration & control
    LcdWriteData(0x00F0);  //DBC manual brightness
    LcdWriteData(0x0000);  //DBC minimum brightness
    LcdWriteData(0x0000);  //Brightness prescaler

    //Set the Dynamic Backlight Control configuration
  LcdWriteReg(0xD0);
    LcdWriteData(0x000D);


}


void InitLCD_ILI9481(void)
{
	  /* Reset */

	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	  HAL_Delay(300);
	  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	  HAL_Delay(100);

    LcdWriteReg(0x11);		// exit sleep mode, 0x0010 to enter
 //   LcdWriteData(0x00);
    HAL_Delay(50);

    LcdWriteReg(0x13);		// enter the normal display mode


    LcdWriteReg(0xd0);		// set the power parameters follow three parameters
    LcdWriteData(0x07);		//1.0xVci 07
    LcdWriteData(0x42);		// 40
    LcdWriteData(0x18);		//. 1C

    LcdWriteReg(0xd1);     	// Vcom subsequent
    LcdWriteData(0x00);		//00 18 1d
    LcdWriteData(0x07);
    LcdWriteData(0x10);

    LcdWriteReg(0xd2);		// set the parameters of the power in the normal mode
    LcdWriteData(0x01);
    LcdWriteData(0x11);

    LcdWriteReg(ILI9341_POWER1);		// Setting Panel Driving
    LcdWriteData(0x10);		// Set the scan mode 00
    LcdWriteData(0x3b);		// set the number of lines 480 lines 3B
    LcdWriteData(0x00);
    LcdWriteData(0x02);		// 5frames 02
    LcdWriteData(0x11);

    LcdWriteReg(ILI9341_POWER2);		// Setting the Timing
    LcdWriteData(0x10);
    LcdWriteData(0x0b);
    LcdWriteData(0x88);



    LcdWriteReg(ILI9341_VCOM1);		// the Frame Rate Control Inversion
    LcdWriteData(0x01);

    LcdWriteReg(0xC8);		// gamma correction, the subsequent 12 parameters
    LcdWriteData(0x00);
    LcdWriteData(0x32);
    LcdWriteData(0x36);
    LcdWriteData(0x45);
    LcdWriteData(0x06);
    LcdWriteData(0x16);
    LcdWriteData(0x37);
    LcdWriteData(0x75);
    LcdWriteData(0x77);
    LcdWriteData(0x54);
    LcdWriteData(0x0f);
    LcdWriteData(0x00);

//    LcdWriteReg(0x36);
//    LcdWriteData(0x0a);//0a no effect


    LcdWriteReg(ILI9341_PIXEL_FORMAT);		//Set_pixel_format
    LcdWriteData(0x55);		//16bit/pixel (65,536 colors)


    LcdWriteReg(ILI9341_COLUMN_ADDR);
    LcdWriteData(0x00);
    LcdWriteData(0x00);
    LcdWriteData(0x01);
    LcdWriteData(0x3f);

    LcdWriteReg(ILI9341_PAGE_ADDR);
    LcdWriteData(0x00);
    LcdWriteData(0x00);
    LcdWriteData(0x01);
    LcdWriteData(0xdf);


    HAL_Delay(50);

    LcdWriteReg(ILI9341_DISPLAY_ON);		//Set_display_on
    LcdWriteReg(ILI9341_GRAM);		// start writing data
    HAL_Delay(25);

}
