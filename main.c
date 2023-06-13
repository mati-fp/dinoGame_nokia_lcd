
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "nokia5110.h"

uint8_t dino1[] = 
{
    0b0100000,
    0b1110000,
    0b0111000,
    0b1111110,
    0b0000110
}; // largura = 5  altura = 7

int start_screen(void)
{
    nokia_lcd_clear();
    nokia_lcd_set_cursor(3, 0);
    nokia_lcd_write_string("Dinosaur Game",1);
    nokia_lcd_set_cursor(5, 12);
    nokia_lcd_write_string("Press Start!", 1);
    print_dino();
    nokia_lcd_set_cursor(20, 24);
    nokia_lcd_write_string("W - JUMP", 1);
    nokia_lcd_set_cursor(20, 36);
    nokia_lcd_write_string("S - DUCK", 1);
    print_floor();
    nokia_lcd_render();

    return 0;
}

int print_dino(void)
{
    nokia_lcd_custom(1, dino1);
    nokia_lcd_set_cursor(1, 48);
    nokia_lcd_write_string("\001", 2);
}

int print_jumping_dino(int x, int y)
{
    nokia_lcd_custom(1, dino1);
    nokia_lcd_set_cursor(x, y);
    nokia_lcd_write_string("\001", 2);
}

int print_ducking_dino(int x, int y)
{
    nokia_lcd_custom(1, dino1);
    nokia_lcd_set_cursor(x, y);
    nokia_lcd_write_string("\001", 2);
}

int print_floor(void)
{
    nokia_lcd_drawline(0, 47, 83, 47);
}

int main(void)
{
   nokia_lcd_init();
   start_screen();
   while (1) {

   }
}


//tam altura = 0x47
//tam largura = 0x83