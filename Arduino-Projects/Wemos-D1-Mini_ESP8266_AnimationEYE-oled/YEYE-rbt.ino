#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI);
  //uint8_t w, uint8_t h, SPIClass *spi_ptr,
    //                               int8_t dc_pin, int8_t rst_pin, int8_t cs_pin,
      //                             uint32_t bitrate)

//reference state
int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;
//current state of the eyes
int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = 32;
int left_eye_y = 32;
int right_eye_x = 32+ref_eye_width+ref_space_between_eye;
int right_eye_y = 32;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;


// 'Background-ezgif', 29x64px
const unsigned char logos [] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x0e, 0x03, 0x03, 0x00, 0x0f, 0x03, 0x07, 0x00, 0x0f, 0x03, 0x0f, 0x00, 
	0x0f, 0xc7, 0x1f, 0x80, 0x0f, 0xe7, 0x3f, 0x80, 0x0e, 0xff, 0xff, 0xc0, 0x0e, 0x7f, 0xf3, 0xc0, 
	0x0e, 0x3f, 0x23, 0xc0, 0x0e, 0x1e, 0x03, 0xc0, 0x0e, 0x0e, 0x03, 0xc0, 0x1e, 0x06, 0x03, 0xc0, 
	0x1e, 0x06, 0x03, 0xc0, 0x0e, 0x06, 0x03, 0xc0, 0x06, 0x07, 0x03, 0xc0, 0x02, 0x07, 0x03, 0xc0, 
	0x02, 0x07, 0x03, 0xc0, 0x06, 0x07, 0x03, 0xc0, 0x0e, 0x07, 0x03, 0xc0, 0x0e, 0x07, 0x03, 0xc0, 
	0x1e, 0x07, 0x03, 0xc0, 0x1e, 0x07, 0x01, 0xc0, 0x1e, 0x07, 0x01, 0xc0, 0x1e, 0x07, 0x01, 0xc0, 
	0x1e, 0x07, 0x00, 0xc0, 0x0e, 0x07, 0x00, 0x00, 0x0e, 0x07, 0x00, 0x00, 0x0f, 0x07, 0x00, 0x00, 
	0x0f, 0x03, 0x00, 0x00, 0x0f, 0x03, 0x00, 0x40, 0x0f, 0x23, 0x00, 0xc0, 0x1e, 0xff, 0x01, 0xc0, 
	0x1e, 0x3f, 0x01, 0xc0, 0x0e, 0x1f, 0x01, 0xc0, 0x0e, 0x1f, 0x01, 0xc0, 0x0e, 0x07, 0x01, 0xc0, 
	0x1e, 0x07, 0xc3, 0xc0, 0x1e, 0x07, 0xc1, 0xc0, 0x1e, 0x07, 0xe1, 0xc0, 0x1e, 0x07, 0xfb, 0x80, 
	0x1e, 0x07, 0xbf, 0x00, 0x00, 0x07, 0x1f, 0x80, 0x00, 0x07, 0x1f, 0x80, 0x00, 0x07, 0x07, 0x80, 
	0x00, 0x06, 0x03, 0x80, 0x00, 0x06, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 
	0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 
	0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define NUMFLAKES     3 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   64 //64
#define LOGO_WIDTH    29 //29

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2
void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}

static const unsigned char image_ir_10px_bits[] = {0x3f,0x00,0x40,0x80,0x9e,0x40,0x21,0x00,0x0c,0x00,0x00,0x00,0x0c,0x00,0x1a,0x00,0x1e,0x00,0xff,0xc0};
static const unsigned char image_Alert_9x8_bits[] = {0x08,0x00,0x1c,0x00,0x14,0x00,0x36,0x00,0x36,0x00,0x7f,0x00,0x77,0x00,0xff,0x80};
// int loadlogo = 0;
void logopingK() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 0, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK1() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 10, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK2() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 20, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK3() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 30, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK4() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 40, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK5() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 50, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK6() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 60, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK7() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 70, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK8() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 80, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK9() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 90, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}
void logopingK10() {
  display.clearDisplay();
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, 100, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);


//   loadlogo = loadlogo + 1;
// if(loadlogo > 100){
//   loadlogo = 0;
// }

  display.display();
}


void logoping() {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - 29 ) / 2,
    (display.height() - 64) / 2,
    logos, 29, 64, 1);
  display.display();
  delay(1000);
}

void draw_eyes(bool update=true)
{
    display.clearDisplay();        
    //draw from center
    int x = int(left_eye_x-left_eye_width/2);
    int y = int(left_eye_y-left_eye_height/2);
    display.fillRoundRect(x,y,left_eye_width,left_eye_height,ref_corner_radius,SSD1306_WHITE);
    x = int(right_eye_x-right_eye_width/2);
    y = int(right_eye_y-right_eye_height/2);
    display.fillRoundRect(x,y,right_eye_width,right_eye_height,ref_corner_radius,SSD1306_WHITE);    
    if(update)
    {
      display.display();
    }
    
}


void center_eyes(bool update=true)
{
  //move eyes to the center of the display, defined by SCREEN_WIDTH, SCREEN_HEIGHT
  left_eye_height = ref_eye_height;
  left_eye_width = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width = ref_eye_width;
  
  left_eye_x = SCREEN_WIDTH/2-ref_eye_width/2-ref_space_between_eye/2;
  left_eye_y = SCREEN_HEIGHT/2;
  right_eye_x = SCREEN_WIDTH/2+ref_eye_width/2+ref_space_between_eye/2;
  right_eye_y = SCREEN_HEIGHT/2;
  
  draw_eyes(update);
}

void blink(int speed=12)
{
  draw_eyes();
  
  
  for(int i=0;i<3;i++)
  {
    left_eye_height = left_eye_height-speed;
    right_eye_height = right_eye_height-speed;    
    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_height = left_eye_height+speed;
    right_eye_height = right_eye_height+speed;
    
    draw_eyes();
    delay(1);
  }
}


void sleep()
{
  left_eye_height = 2;
  right_eye_height = 2;
  draw_eyes(true);  
}
void wakeup()
{
  
  sleep();
  
  for(int h=0; h <= ref_eye_height; h+=2)
  {
    left_eye_height = h;
    right_eye_height = h;
    draw_eyes(true);
  }
  

}


void happy_eye()
{
  center_eyes(false);
  //draw inverted triangle over eye lower part
  int offset = ref_eye_height/2;
  for(int i=0;i<10;i++)
  {
    display.fillTriangle(left_eye_x-left_eye_width/2-1, left_eye_y+offset, left_eye_x+left_eye_width/2+1, left_eye_y+5+offset, left_eye_x-left_eye_width/2-1,left_eye_y+left_eye_height+offset,SSD1306_BLACK);
    //display.fillRect(left_eye_x-left_eye_width/2-1, left_eye_y+5, left_eye_width+1, 20,SSD1306_BLACK);

    display.fillTriangle(right_eye_x+right_eye_width/2+1, right_eye_y+offset, right_eye_x-left_eye_width/2-1, right_eye_y+5+offset, right_eye_x+right_eye_width/2+1,right_eye_y+right_eye_height+offset,SSD1306_BLACK);
    //display.fillRect(right_eye_x-right_eye_width/2-1, right_eye_y+5, right_eye_width+1, 20,SSD1306_BLACK);  
    offset -= 2;
    display.display();
    delay(1);
  }
  
  
  display.display();
  delay(1000);
}

void saccade(int direction_x, int direction_y)
{
  //quick movement of the eye, no size change. stay at position after movement, will not move back,  call again with opposite direction
  //direction == -1 :  move left
  //direction == 1 :  move right
  
  int direction_x_movement_amplitude = 8;
  int direction_y_movement_amplitude = 6;
  int blink_amplitude = 8;

  for(int i=0;i<1;i++)
  {
    left_eye_x+=direction_x_movement_amplitude*direction_x;
    right_eye_x+=direction_x_movement_amplitude*direction_x;    
    left_eye_y+=direction_y_movement_amplitude*direction_y;
    right_eye_y+=direction_y_movement_amplitude*direction_y;    
    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    draw_eyes();
    delay(1);
  }
  
  for(int i=0;i<1;i++)
  {
    left_eye_x+=direction_x_movement_amplitude*direction_x;
    right_eye_x+=direction_x_movement_amplitude*direction_x;    
    left_eye_y+=direction_y_movement_amplitude*direction_y;
    right_eye_y+=direction_y_movement_amplitude*direction_y;

    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;

    draw_eyes();
    delay(1);
  }
  
}

void move_right_big_eye()
{
  move_big_eye(1);
}
void move_left_big_eye()
{
  move_big_eye(-1);
}
void move_big_eye(int direction)
{
  //direction == -1 :  move left
  //direction == 1 :  move right

  int direction_oversize = 1;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;

  for(int i=0;i<3;i++)
  {
    left_eye_x+=direction_movement_amplitude*direction;
    right_eye_x+=direction_movement_amplitude*direction;    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    if(direction>0)
    {
      right_eye_height+=direction_oversize;
      right_eye_width+=direction_oversize;
    }else
    {
      left_eye_height+=direction_oversize;
      left_eye_width+=direction_oversize;
    }

    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_x+=direction_movement_amplitude*direction;
    right_eye_x+=direction_movement_amplitude*direction;
    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;
    if(direction>0)
    {
      right_eye_height+=direction_oversize;
      right_eye_width+=direction_oversize;
    }else
    {
      left_eye_height+=direction_oversize;
      left_eye_width+=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }

  delay(1000);

  for(int i=0;i<3;i++)
  {
    left_eye_x-=direction_movement_amplitude*direction;
    right_eye_x-=direction_movement_amplitude*direction;    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    if(direction>0)
    {
      right_eye_height-=direction_oversize;
      right_eye_width-=direction_oversize;
    }else
    {
      left_eye_height-=direction_oversize;
      left_eye_width-=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_x-=direction_movement_amplitude*direction;
    right_eye_x-=direction_movement_amplitude*direction;    
    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;
    if(direction>0)
    {
      right_eye_height-=direction_oversize;
      right_eye_width-=direction_oversize;
    }else
    {
      left_eye_height-=direction_oversize;
      left_eye_width-=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }


  center_eyes();


}

int loadings = 0;

void start1(){
  display.display();
  static const unsigned char PROGMEM image_ir_10px_bits[] = {0x3f,0x00,0x40,0x80,0x9e,0x40,0x21,0x00,0x0c,0x00,0x00,0x00,0x0c,0x00,0x1a,0x00,0x1e,0x00,0xff,0xc0};
static const unsigned char PROGMEM image_Alert_9x8_bits[] = {0x08,0x00,0x1c,0x00,0x14,0x00,0x36,0x00,0x36,0x00,0x7f,0x00,0x77,0x00,0xff,0x80};
display.drawRect(14, 24, 100, 20, 1);
display.fillRect(14, 25, loadings, 18, 1);
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(14, 11);
display.setTextWrap(false);
display.print("Mochi_EvE.ca");
display.setTextColor(1);
display.setTextSize(1);
display.setCursor(27, 50);
display.setTextWrap(false);
display.print("Waking up...");
display.drawBitmap(14, 48, image_ir_10px_bits, 10, 10, 1);
display.drawBitmap(105, 10, image_Alert_9x8_bits, 9, 8, 1);

loadings = loadings + 1;
if(loadings > 100){
  loadings = 0;
}
display.clearDisplay();
}


void judul(){
  display.clearDisplay();

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Mochi EvE | ME-01"));
  display.setCursor(0, 15);
  display.println(F("Creator: mnikhwan"));
  display.display();      // Show initial text
  delay(4000);
  display.clearDisplay();

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  
  
  display.setCursor(0, 0);
  display.println(F("Inintiating System."));
  delay(2000);
  display.setCursor(0, 13);
  display.println(F("Activate Core.."));

display.display();      // Show initial text

  delay(6000);
  display.clearDisplay();
}

void setup() {
  // put your setup code here, to run once:
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  
  
  //for usb communication  
  Serial.begin(115200);
  
  

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  
  // Clear the buffer
  display.clearDisplay();
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Mochi_EvE.ca"));
  display.display();
  delay(800);
  sleep();
  

  // Draw a single pixel in white
  //display.drawPixel(10, 10, SSD1306_WHITE);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:


  
  //send A0 - A5  for animation 0 to 5
  
    
    
    
    
    
    
          logoping();
          delay(7000);
          judul();
          delay(7000);
// --------------------------------
          logopingK();
          delay(1000);
          logopingK1();
          delay(1000);
          logopingK2();
          delay(1000);
          logopingK3();
          delay(1000);
          logopingK4();
          delay(1000);
          logopingK5();
          delay(1000);
          logopingK6();
          delay(1000);
          logopingK7();
          delay(1000);
          logopingK8();
          delay(1000);
          logopingK9();
          delay(1000);
          logopingK10();
// ----------------------------
          delay(7000);
          sleep();
          delay(500);
          start1();
          wakeup();
          delay(1000);
          center_eyes(true);
          delay(1000);
          move_right_big_eye();
          delay(1000);
          move_left_big_eye();
          delay(1000);
          blink(10);
          delay(1000);
          blink(20);
          delay(1000);
          center_eyes(true);
          delay(1000);
          move_right_big_eye();
          delay(1000);
          move_left_big_eye();
          delay(1000);
          blink(10);
          delay(1000);
          blink(20);
          delay(1000);center_eyes(true);
          delay(1000);
          move_right_big_eye();
          delay(1000);
          move_left_big_eye();
          delay(1000);
          blink(10);
          delay(1000);
          blink(20);
          delay(1000);center_eyes(true);
          delay(1000);
          move_right_big_eye();
          delay(1000);
          move_left_big_eye();
          delay(1000);
          blink(10);
          delay(1000);
          blink(20);
          delay(1000);center_eyes(true);
          delay(1000);
          move_right_big_eye();
          delay(1000);
          move_left_big_eye();
          delay(1000);
          blink(10);
          delay(1000);
          blink(20);
          delay(1000);
          sleep();
          delay(2000);
          testanimate(logos, LOGO_WIDTH, LOGO_HEIGHT);
          // int dir_x = random(-1, 2);
          // int dir_y = random(-1, 2);
          // saccade(dir_x,dir_y);
          // delay(600);
          // saccade(-dir_x,-dir_y);
          // delay(600);
          
      
    
  
}