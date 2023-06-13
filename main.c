
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
}; // largura = 10  altura = 14

int main(void)
{
    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_set_cursor(3, 0);
    nokia_lcd_write_string("Dinosaur Game",1);
    nokia_lcd_set_cursor(5, 12);
    nokia_lcd_write_string("Press Start!", 1);
    nokia_lcd_custom(1, dino1);
    nokia_lcd_set_cursor(1, 48);
    nokia_lcd_write_string("\001", 2);
    nokia_lcd_set_cursor(20, 24);
    nokia_lcd_write_string("W - JUMP", 1);
    nokia_lcd_set_cursor(20, 36);
    nokia_lcd_write_string("S - DUCK", 1);
    nokia_lcd_render();
   
   while (1) {

   }
}


//tam altura = 0x47
//tam largura = 0x83