#include "stm32f4xx_hal.h"

#define SSD1306_I2C_ADDR 0x78

#define SSD1306_WIDTH 128

#define SSD1306_HEIGHT 64

uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

void ssd1306_WriteCommand(I2C_HandleTypeDef *hi2c, uint8_t command){
    HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR, 0x00, 1, &command, 1, 10);
}

void scrinit(I2C_HandleTypeDef *hi2c){
    HAL_Delay(100);
    ssd1306_WriteCommand(hi2c, 0xAE);   // Display off
    ssd1306_WriteCommand(hi2c, 0x20);   // Set Memory Addressing Mode
    ssd1306_WriteCommand(hi2c, 0x10);   // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    ssd1306_WriteCommand(hi2c, 0xB0);   // Set Page Start Address for Page Addressing Mode,0-7
    ssd1306_WriteCommand(hi2c, 0xC8);   // Set COM Output Scan Direction
    ssd1306_WriteCommand(hi2c, 0x00);   // Set low column address
    ssd1306_WriteCommand(hi2c, 0x10);   // Set high column address
    ssd1306_WriteCommand(hi2c, 0x40);   // Set start line address
    ssd1306_WriteCommand(hi2c, 0x81);   // set contrast control register
    ssd1306_WriteCommand(hi2c, 0xFF);
    ssd1306_WriteCommand(hi2c, 0xA1);   // Set segment re-map 0 to 127
    ssd1306_WriteCommand(hi2c, 0xA6);   // Set normal display

    ssd1306_WriteCommand(hi2c, 0xA8);   // Set multiplex ratio(1 to 64)
    ssd1306_WriteCommand(hi2c, SSD1306_HEIGHT - 1);

    ssd1306_WriteCommand(hi2c, 0xA4);   // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    ssd1306_WriteCommand(hi2c, 0xD3);   // Set display offset
    ssd1306_WriteCommand(hi2c, 0x00);   // No offset
    ssd1306_WriteCommand(hi2c, 0xD5);   // Set display clock divide ratio/oscillator frequency
    ssd1306_WriteCommand(hi2c, 0xF0);   // Set divide ratio
    ssd1306_WriteCommand(hi2c, 0xD9);   // Set pre-charge period
    ssd1306_WriteCommand(hi2c, 0x22);

    ssd1306_WriteCommand(hi2c, 0xDA);   // Set com pins hardware configuration
    ssd1306_WriteCommand(hi2c, 0x32);

    ssd1306_WriteCommand(hi2c, 0xDB);   // Set vcomh
    ssd1306_WriteCommand(hi2c, 0x20);   // 0x20,0.77xVcc
    ssd1306_WriteCommand(hi2c, 0x8D);   // Set DC-DC enable
    ssd1306_WriteCommand(hi2c, 0x14);   //
    ssd1306_WriteCommand(hi2c, 0xAF);   // Turn on SSD1306 panel
}

void scrclear(){
	for(int i = 0; i != SSD1306_WIDTH * SSD1306_HEIGHT / 8; i++){
		SSD1306_Buffer[i] = 0x00;
	}
}

void scrfill(){
	for(int i = 0; i != SSD1306_WIDTH * SSD1306_HEIGHT / 8; i++){
		SSD1306_Buffer[i] = 0xFF;
	}
}

void scrupdate(I2C_HandleTypeDef *hi2c){
    for (uint8_t i = 0; i < 8; i++) {
        ssd1306_WriteCommand(hi2c, 0xB0 + i);
        ssd1306_WriteCommand(hi2c, 0x00);
        ssd1306_WriteCommand(hi2c, 0x10);

        HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR, 0x40, 1, &SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH, 100);
    }
}

void fillpixel(uint8_t x, uint8_t y)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT){
        return;
    }
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
}

void clearpixel(uint8_t x, uint8_t y)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT){
        return;
    }
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}

