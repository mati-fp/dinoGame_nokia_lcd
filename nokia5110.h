/* Nokia 5110 LCD AVR Library
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */

#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include <avr/pgmspace.h>
#include <stdint.h>

/*
 * LCD's port
 */
#define PORT_LCD PORTB
#define DDR_LCD DDRB

/*
 * LCD's pins
 */
#define LCD_SCE PB1
#define LCD_RST PB2
#define LCD_DC PB3
#define LCD_DIN PB4
#define LCD_CLK PB5

#define LCD_CONTRAST 0x40

/*
 * Must be called once before any other function, initializes display
 */
void nokia_lcd_init(void);

/*
 * Clear screen
 */
void nokia_lcd_clear(void);

/**
 * Power of display
 * @lcd: lcd nokia struct
 * @on: 1 - on; 0 - off;
 */
void nokia_lcd_power(uint8_t on);

/**
 * Desenha uma linha
 * @x1: coord. x do ponto 1
 * @y1: coord. y do ponto 1
 * @x2: coord. x do ponto 2
 * @y2: coord. y do ponto 2
 */
void nokia_lcd_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/**
 * Desenha um retângulo
 * @x1: coord. x de um dos cantos
 * @y1: coord. y de um dos cantos
 * @x2: coord. x do outro canto
 * @y2: coord. y do outro canto
 */
void nokia_lcd_drawrect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/**
 * Desenha um círculo
 * @x: coord. x do centro
 * @y: coord. y do centro
 * @r: raio em pixels
 */
void nokia_lcd_drawcircle(uint8_t x, uint8_t y, uint8_t r);

/**
 * Set single pixel
 * @x: horizontal position
 * @y: vertical position
 * @value: show/hide pixel
 */
void nokia_lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value);

/**
 * Draw single char with 1-6 scale
 * @code: char code
 * @scale: size of char
 */
void nokia_lcd_write_char(char code, uint8_t scale);

void nokia_lcd_write_char_opposite(char code, uint8_t scale);

/**
 * Draw string. Example: writeString("abc",3);
 * @str: sending string
 * @scale: size of text
 */
void nokia_lcd_write_string(const char *str, uint8_t scale, int opposite);

/**
 * Set cursor position
 * @x: horizontal position
 * @y: vertical position
 */
void nokia_lcd_set_cursor(uint8_t x, uint8_t y);

/*
 * Render screen to display
 */
void nokia_lcd_render(void);

/*
 * Define custom char (ASCII 0-31)
 */
void nokia_lcd_custom(char code, uint8_t *glyph);

#endif
