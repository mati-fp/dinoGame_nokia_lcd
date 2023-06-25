#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "nokia5110.h"

//tam altura = 0x47
//tam largura = 0x83

#define TIMER_CLK F_CPU / 1024
#define IRQ_FREQ 1

volatile int melhorTempo = 0;
volatile int comecarJogo = 1;
volatile int dinoPulando = 0;
volatile int dinoDuck = 0;

uint8_t dinoUp[] = 
{
    0b0100000,
    0b1110000,
    0b0111000,
    0b1111110,
    0b0000110
}; // largura = 5  altura = 7

uint8_t dinoDown[] = 
{
    0b0010100,
    0b0011111,
    0b0011111,
    0b1111101,
    0b1110000
}; // largura = 7  altura = 5 chama a função diferente do glyph aqui


int start_screen(void)
{
    nokia_lcd_clear();
    nokia_lcd_set_cursor(3, 0);
    nokia_lcd_write_string("Dinosaur Game", 1, 0);
    nokia_lcd_set_cursor(5, 12);
    nokia_lcd_write_string("Press Start!", 1, 0);
    print_dino();
    nokia_lcd_set_cursor(20, 24);
    nokia_lcd_write_string("W - JUMP", 1, 0);
    nokia_lcd_set_cursor(20, 36);
    nokia_lcd_write_string("S - DUCK", 1, 0);
    print_floor();
    nokia_lcd_render();

    return 0;
}

int print_dino(void)
{
    nokia_lcd_custom(1, dinoUp);
    nokia_lcd_set_cursor(1, 48);
    nokia_lcd_write_string("\001", 2, 0);
}

int print_jumping_dino(void)
{
    nokia_lcd_custom(1, dinoUp);
    nokia_lcd_set_cursor(1, 36);
    nokia_lcd_write_string("\001", 2, 0);
}

int print_ducking_dino(void)
{
    nokia_lcd_custom(1, dinoDown);
    nokia_lcd_set_cursor(1, 37);
    nokia_lcd_write_string("\001", 2, 1);
}

int print_floor(void)
{
    nokia_lcd_drawline(0, 47, 83, 47);
}

int print_score(void)
{
    nokia_lcd_set_cursor(0, 0);
    char score[30];
    sprintf(score, "Melhor tempo: %d", melhorTempo);
    nokia_lcd_write_string(score, 1, 0);
    nokia_lcd_drawline(0, 9, 83, 9);
}

int comeca_jogo(void){

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

    PCICR |= (1 << PCIE2);         // habilita vetor de interrupção para PD0 .. PD7
    PCMSK2 |= (1 << PCINT16);       // habilita interrupção para PD0
    PCMSK2 |= (1 << PCINT17);       // habilita interrupção para PD1
    PCMSK2 |= (1 << PCINT18);       // habilita interrupção para PD2

   nokia_lcd_init();
   start_screen();

   sei();

   while (1) {

   }
}

ISR(TIMER1_COMPA_vect)
{

}

ISR(INT0_vect)
{
    if (PIND & (1 << PD0))
        if (comecarJogo)
        {
            naTelaInicial = 0;
            comeca_jogo();
        }
    
    if (PIND & (1 << PD1))
    {
        print_jumping_dino();
        dinoPulando = 1;
    }

    if (PIND & (1 << PD2))
        while (PIND & (1 << PD2))
        {
            print_ducking_dino();
            dinoDuck = 1;
        }
}   


