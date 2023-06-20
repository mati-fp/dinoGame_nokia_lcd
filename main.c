
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "nokia5110.h"

#define TIMER_CLK F_CPU / 1024
#define IRQ_FREQ 1

uint8_t dino1[] = 
{
    0b0100000,
    0b1110000,
    0b0111000,
    0b1111110,
    0b0000110
}; // largura = 5  altura = 7

//ducking dino
uint8_t dino2[] =
{
    0b0100000,
    0b1110000,
    0b0110000,
    0b1111000,
    0b0011000

};

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

int print_dino()
{
    nokia_lcd_custom(1, dino1);
    nokia_lcd_set_cursor(0, 48);
    nokia_lcd_write_string("\001", 2);
}

int print_jumping_dino(int x, int y)
{
    nokia_lcd_custom(1, dino1);
    nokia_lcd_set_cursor(x, y);
    nokia_lcd_write_string("\001", 2);
    //print_jumping_dino(1,18);
    //altura possível para o pulo: 1,18 (um pixel acima da cabeça)
}

int print_ducking_dino(int x, int y)
{
    nokia_lcd_custom(1, dino2);
    nokia_lcd_set_cursor(x, y);
    nokia_lcd_write_string("\001", 2);
}

int print_floor(void)
{
    nokia_lcd_drawline(0, 47, 83, 47);
}

int main(void)
{
    // desabilita interrupções
    cli();
    // resseta contadores para TIMER1
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    // seta o registrador output compare
    OCR1A = (TIMER_CLK / IRQ_FREQ) - 1;
    // liga modo CTC
    TCCR1B |= (1 << WGM12);
    // seta CS10 e CS12 para prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // habilita máscara do timer1
    TIMSK1 |= (1 << OCIE1A);
    DDRD &= ~(1 << PD0) | ~(1 << PD1) | ~(1 << PD2) | ~(1 << PD3); // entradas
    PORTD |= (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3);    // desabilita pull-up
    nokia_lcd_init();
    start_screen();
    while (1) {
        if((1<<PD0)){
            nokia_lcd_clear();
        }
   }
}


//tam altura = 0x47
//tam largura = 0x83