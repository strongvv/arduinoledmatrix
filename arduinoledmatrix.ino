#include "led_matrix_worker.h"

void setup(){
  pinMode(CE, OUTPUT);                       
  SPI.begin();                                    
}

void loop(){
  for (uint8_t i = 0; i != SCREEN_SIZE; ++i){
    for (uint8_t j = 0; j != SCREEN_SIZE; ++j)
      screen[i][j] = screen[i][j] != 0 ? ((screen[i][j] + 1) % 7) + 1 : 0;
  }
  
  for (int m = 0; m != 60; ++m){
    draw_screen();
  }
  shift_screen(-1, true);
}
