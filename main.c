#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "nokia5110.h"

#define TIMER_CLK F_CPU / 1024
#define IRQ_FREQ 1

// tam altura = 0x47
// tam largura = 0x83

volatile int melhorTempo = 0;
volatile int jogoRolando = 0;
volatile int dinoJump = 0;
volatile int dinoDuck = 0;
volatile int contagem = 10;
volatile int tempoAtual = 0;
volatile int colisao = 0;
volatile int posicaoFlecha = 69;
volatile int posicaoArvore = 63;
volatile int contDuck = 2;
volatile int contJump = 2;
volatile int flechaNaTela = 1;
volatile int arvoreNaTela = 0;

uint8_t dinoUp[] =
    {
        0b0100000,
        0b1110000,
        0b0111000,
        0b1111110,
        0b0000110}; // largura = 5  altura = 7

uint8_t dinoDown[] =
    {
        0b0100000,
        0b1110000,
        0b0110000,
        0b1111000,
        0b0011000

}; // largura = 5  altura = 8
uint8_t tree[] =
    {
        0b0011000,
        0b0011100,
        0b1111100,
        0b0011100,
        0b0011000}; // largura = 5  altura = 7

uint8_t arrow[] =
    {
        0b000100,
        0b1100010,
        0b0111111,
        0b1100010,
        0b0000100}; // largura = 7  altura = 5

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
}

int print_dino(void)
{
    nokia_lcd_custom(1, dinoUp);
    nokia_lcd_set_cursor(0, 48);
    nokia_lcd_write_string("\001", 2, 0);
}

int print_jumping_dino(void)
{
    nokia_lcd_custom(1, dinoUp);
    nokia_lcd_set_cursor(1, 24);
    nokia_lcd_write_string("\001", 2, 0);
}

int print_ducking_dino(void)
{
    nokia_lcd_custom(1, dinoDown);
    nokia_lcd_set_cursor(1, 48);
    nokia_lcd_write_string("\001", 2, 0);
}

int print_floor(void)
{
    nokia_lcd_drawline(0, 47, 83, 47);
}

int print_score(void)
{
    nokia_lcd_set_cursor(0, 0);
    char score[30];
    sprintf(score, "Score: %d", tempoAtual);
    nokia_lcd_write_string(score, 1, 0);
    nokia_lcd_drawline(0, 9, 83, 9);
}

int print_tree(int x)
{
    if (x == 0)
    {
        posicaoArvore = 63;
        arvoreNaTela = !arvoreNaTela;
    }

    nokia_lcd_custom(1, tree);
    nokia_lcd_set_cursor(x, 32);
    nokia_lcd_write_string("\001", 2, 0);
}

int print_arrow(int x)
{
    if (x == 0)
    {
        posicaoFlecha = 69;
        flechaNaTela = !flechaNaTela;
    }

    nokia_lcd_custom(1, arrow);
    nokia_lcd_set_cursor(x, 32);
    nokia_lcd_write_string("\001", 1, 1);
}

int comeca_jogo(void)
{
    nokia_lcd_clear();
    sei();
    while (colisao == 0)
    {
        nokia_lcd_clear();
        print_score();

        if (PIND & (1 << PD2) | dinoJump)
        { // verificando pulo
            dinoDuck = 0;
            print_jumping_dino();
            dinoJump = 1;
        }

        if (PIND & (1 << PD3) | dinoDuck)
        { // verificando duck
            dinoJump = 0;
            print_ducking_dino();
            dinoDuck = 1;
        }

        if (!(dinoJump) & !(dinoDuck))
            print_dino();

        if (flechaNaTela)
        {
            print_arrow(posicaoFlecha); // posicao inicial da flecha
            posicaoFlecha--;
        }
        if (arvoreNaTela)
        {
            print_tree(posicaoArvore); // posicao inicial da arvore
            posicaoArvore--;
        }
        print_floor();
        nokia_lcd_render();
    }
    cli();
    contagem = 10;
}

ISR(TIMER1_COMPA_vect)
{
    if (jogoRolando)
        tempoAtual++;

    if (dinoDuck == 1)
    {
        contDuck--;
    }

    if (dinoJump == 1)
    {
        contJump--;
    }

    if (contDuck <= 0)
    {
        dinoDuck = 0;
        contDuck = 2;
    }

    if (contJump <= 0)
    {
        dinoJump = 0;
        contJump = 2;
    }
}

int main(void)
{
    // desabilita interrupcoes
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
    // habilita mascara do timer1
    TIMSK1 |= (1 << OCIE1A);

    DDRD &= ~(1 << PD1) | ~(1 << PD2) | ~(1 << PD3); // entradas
    PORTD |= (1 << PD1) | (1 << PD2) | (1 << PD3);   // desabilita pull-up

    // PCICR |= (1 << PCIE2);         // habilita vetor de interrupcao para PD7 .. PD0
    // PCMSK2 |= (1 << PCINT17);      // habilita i nterrupcao para PD1
    // PCMSK2 |= (1 << PCINT18);      // habilita interrupcao para PD2
    // PCMSK2 |= (1 << PCINT19);      // habilita interrupcao para PD3

    // sei();

    nokia_lcd_init();
    start_screen();

    while (1)
    {
        if (PIND & (1 << PD1))
            if (!(jogoRolando))
            {
                jogoRolando = 1;
                comeca_jogo();
            }
    }
}

// ISR(PCINT2_vect)
// {
//     if (PIND & (1 << PD1))
//         if (!(jogoRolando))
//         {
//             jogoRolando = 1;
//             comeca_jogo();
//         }

//     if (PIND & (1 << PD2))
//     { // verificando pulo
//         dinoJump = !dinoJump;
//         _delay_ms(2000); // debounce
//         dinoJump = !dinoJump;
//     }

//     if (PIND & (1 << PD3))
//     { // verificando duck
//         dinoDuck = !dinoDuck;
//         _delay_ms(2000); // debounce
//         dinoDuck = !dinoDuck;
//     }
// }