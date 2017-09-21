
void draw_screen(){
  for (uint8_t i = 0; i != SCREEN_SIZE; ++i){
    for (uint8_t j = 0; j != SCREEN_SIZE; ++j){
      draw_pixel(j, i, screen[i][j], false);
    }
  }
}

void swap(uint8_t & a, uint8_t & b){
    a += b;
    b = a - b;
    a -= b;
}

void reverse(uint8_t * a, uint8_t left, uint8_t right) {
    if (left >= right || right >= SCREEN_SIZE) return;

    for (uint8_t i = left, j = right; i < j; i++, j--) {
        swap(a[i], a[j]);
    }
}

void shift_row(uint8_t * a, uint8_t shift) {
    if (shift < 0){
        while (abs(shift) >= SCREEN_SIZE){
            shift = -abs(shift) % SCREEN_SIZE;
        }
        shift += SCREEN_SIZE;
    }

    if (shift >= SCREEN_SIZE) {
        shift = shift % SCREEN_SIZE;
    }
    if (0 == shift){
        return;
    }
    reverse(a, 0, shift - 1);
    reverse(a, shift, SCREEN_SIZE - 1);
    reverse(a, 0, SCREEN_SIZE - 1);
}

void shift_col(uint8_t col, uint8_t shift){
    shift *= -1;
    uint8_t temp[SCREEN_SIZE] = {};
    for (uint8_t i = 0; i != SCREEN_SIZE; ++i){
        temp[i] = screen[i][col];
    }
    shift_row(temp, shift);
    for (uint8_t i = 0; i != SCREEN_SIZE; ++i){
        screen[i][col] = temp[i];
    }
}

void shift_screen(int offset, bool horizont){
  if (horizont){
    for (uint8_t i = 0; i != SCREEN_SIZE; ++i)
      shift_row(screen[i], offset);
  } else {
    for (uint8_t i = 0; i != SCREEN_SIZE; ++i)
      shift_col(i, offset);    
  }
}

void draw_pixel(uint8_t x, uint8_t y, uint8_t color, bool transparent){
  data[0] = 0xFF; 
  data[1] = 0xFF; 
  data[2] = 0xFF; 
  data[3] = 0; 
    if (color == 0){
        data[0] |= (1 << y);
        data[1] |= (1 << y);
        data[2] |= (1 << y);
    } else {
        if (color & 1)
            data[0] = ~(0x01 << (7-y));
        if ((color >> 1) & 1)    
            data[1] = ~(0x01 << (7-y));
        if ((color >> 2) & 1)    
            data[2] = ~(0x01 << (7-y));    
    } 
    data[3] = 1 << x;
    digitalWrite(CE, LOW);     // when CE is low, it begin to receive data.
    SPI.transfer(data[0]);     //transfer data[0] to the screen(red)
    SPI.transfer(data[2]);     //transfer data[2] to the screen(blue)
    SPI.transfer(data[1]);     //transfer data[1] to the screen(green)
    SPI.transfer(data[3]);     // tansfer data[3] to the screen( scanning and display the data on screen)
    digitalWrite(CE, HIGH);    // when CE is High, means that screen begin to display the array's information to the screen.
    if (transparent){
      delay(1);
    }
}

void set_pixel(uint8_t x, uint8_t y, uint8_t color){
  screen[x][y] = color;
}

void set_cursor(uint8_t x,uint8_t y){
  draw_pixel(x, y, screen[x][y],true);
}
  
void screen_off(){
    for (uint8_t i = 0; i != SCREEN_SIZE; ++i){
        for (uint8_t j = 0; j != SCREEN_SIZE; ++j){
            screen[i][j] = 0;
        }
    }
}

void fill_area(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint8_t color){
  if (x_end >= 8){
    x_end = 7;  
  }
  if (y_end >= 8){
    y_end = 7;  
  }
  if (x_end < x_start){
    swap(x_end, x_start);
  }
  if (y_end < y_start){
    swap(y_end, y_start);
  }
  for (uint8_t m = y_start; m != y_end+1; ++m){
    for(uint8_t l = x_start; l!= x_end+1; ++l){
      set_pixel(l, m, color);
    }
  }
}

void scroll_font() {
  for (int counter=0x20; counter<0xC2; ++counter){
    loadBufferLong(counter);
    delay(200);
  }
}

void loadBufferLong(int ascii){
  if (ascii >= 0x0020 && ascii <=0x00ff){
    for (uint8_t a=0; a != 8; ++a){ // Loop 7 times for a 5x7 font
      // Index into character table to get row data
      unsigned long c = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 9) + a); 
      unsigned long x = bufferLong [a*2];     // Load current scroll buffer
      x = x | c;                              // OR the new character onto end of current
      bufferLong [a*2] = x;                   // Store in buffer
    }
    // Index into character table for kerning data
    byte count = pgm_read_byte_near(font5x7 +((ascii - 0x20) * 9) + 8);  
    for (byte x=0; x != count; ++x){
      rotateBufferLong();
      printBufferLong();
      delay(scrollDelay);
    }
  }
}
