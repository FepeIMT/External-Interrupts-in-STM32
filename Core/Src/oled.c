/*
 * oled.c
 *      Author: Fernando Aguilar
 */

#include <Main_Screen.h>
#include "oled.h"


#define DELAY_GIF (1)
extern I2C_HandleTypeDef hi2c1;

uint8_t OLED_init()
{
	/* Check if the slave device is ready to start the communication */
	if (HAL_I2C_IsDeviceReady(&hi2c1, OLED_ADDRESS, 1, 20000) != HAL_OK)
	{
			/* Return false */
			return 0;
	}

	writeCommand(0xae); //Display OFF
	writeCommand(0xd5); //Set Dclk
	writeCommand(0x50); //100 hz
	writeCommand(0x20); // Set row address
	writeCommand(0x81); //Set contrast control
	writeCommand(0x80); //
	writeCommand(0xa0); //Segment remap
	writeCommand(0xa4); //Set Entire Display On
	writeCommand(0xa6); //Normal Display
	writeCommand(0xad); //Set external VCC
	writeCommand(0x80); //
	writeCommand(0xc0); //Set common scan direction
	writeCommand(0xd9); //Set phase length
	writeCommand(0x1f); //
	writeCommand(0xdb); //Set Vcomh voltage
	writeCommand(0x27); //
	Clear_All_RAM();
	writeCommand(0xaf); //Display On

	return 1;  /* If the device is ready return 1 */

}

void Clear_All_RAM(void)
{
	unsigned char i, j;
	/*Cycle for to clear all ram in each page */
	for(i = 0; i < PAGES; i++)
	{
		writeCommand(0xb0+i);  /*Skip to the next page  */
		writeCommand(0x00);
		writeCommand(0x10);
		for( j = 0; j < HEIGHT; j++)
		{
			writeData(0x00);
		}
	}
}

void writeCommand(uint8_t data)
{
	uint8_t dt[2];
	dt[0] = REG_COMMAND;  /*Value to indicate that the next value is a command config */
	dt[1] = data; /* Command option */
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS , dt, 2, 10);  /*Function HAL to transmit through I2C port selected  */
}

void writeData(uint8_t data)
{
	uint8_t dt[2];
	dt[0] = REG_DATA; /*Value to indicate that the next value is data to write in the RAM Oled Screen */
	dt[1] = data;     /*Value to write in the OLED Screen RAM */
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, dt, 2, 10);
}


void printGif()
{

	printBitmap(Screen1);
	printBitmap(Screen2);
	printBitmap(Screen3);
	printBitmap(Screen4);
	printBitmap(Screen5);
	printBitmap(Screen6);
	printBitmap(Screen7);
	printBitmap(Screen8);



}


void printBitmap(const unsigned char* Bitmap)
{
	   unsigned char i, j, k;
	   /*Because the Bitmap is a vector we need to use auxiliary variable to send all information in the correct order  */
	   int d = 0;
	   k = 15;

	    /*Cycles for to write all bitmap in all screen OLED */
	  	for(i = 0; i < PAGES; i++)
	  	{
	  		writeCommand(0xb0+i);
	  		writeCommand(0x00);
	  		writeCommand(0x10);
	  		for( j = 0; j < HEIGHT ; j++)
	  		{
	  			writeData(Bitmap[k + d]);
	  			d = d +16;
	  		}
	  		d = 0;
	  		k--;
	  	}
}




