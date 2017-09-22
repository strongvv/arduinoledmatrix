#ifndef LED_MATRIX_WORKER
#define LED_MATRIX_WORKER
#include <Arduino.h>
#include <SPI.h>                  //  include the head file to enable the  library.
#include <avr/pgmspace.h>

const int CE = 10;                                                      // defined  the CE function pin
static byte data[4] = {0x0, 0x0, 0x0, 0x0}; 

static const byte BLACK = 0;
static const byte RED = 1;
static const byte GREEN = 2;
static const byte YELLOW = 3;
static const byte BLUE = 4;
static const byte MAGENTA = 5;
static const byte TURQUOISE = 6;
static const byte WHITE = 7;

static const byte SCREEN_SIZE = 8;

static uint8_t screen[8][8] = { //led colors
                                {0, 1, 1, 0, 0, 0, 0, 0},
                                {1, 1, 1, 1, 2, 0, 0, 2},
                                {1, 1, 1, 1, 2, 2, 2, 2},
                                {1, 4, 4, 1, 2, 2, 2, 2},
                                {4, 4, 4, 4, 3, 2, 2, 3},
                                {4, 4, 4, 4, 3, 3, 3, 3},
                                {4, 0, 0, 4, 3, 3, 3, 3},
                                {0, 0, 0, 0, 0, 3, 3, 0}
                              };

byte bufferLong [16] = {0}; 
static const byte scrollDelay = 75;

void swap(uint8_t & a, uint8_t & b);
void reverse(uint8_t * a, uint8_t left, uint8_t right);
void shift_row(uint8_t * a, uint8_t shift);
void shift_col(uint8_t col, uint8_t shift);
void shift_screen(int offset, bool horizont);                              
void set_pixel(uint8_t x, uint8_t y, uint8_t color);
void set_cursor(uint8_t x,uint8_t y);
void screen_off();
void draw_pixel(uint8_t x, uint8_t y, uint8_t color, bool transparent);
void fill_area(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint8_t color);
void draw_screen();
void scroll_font();
void loadBufferLong(int ascii);
#endif
