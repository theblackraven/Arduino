#include <Adafruit_GFX.h>

//-- Andrologiciels TouchPaint example for TFT 2.4 '' ------------//
// with Spfd5408 controller                                  //
// (c) AndroLogiciels 2015                                   //
///////////////////////////////////////////////////////////////
// You can 'calibrate' the screen with CalX and CalY values  //
// Multimeter X+ (Pin 6) & X- (pin A2) = 'Sens'itivity value // 
///////////////////////////////////////////////////////////////
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>     // Touch library
#include <TimerOne.h> // Timer 
#include <Thermistor100k.h>
//-- Calibrates value
#define CalX 0
#define CalY 0
#define Sens 300
//240x320
//-- These are the pins for the shield!
#define YP A2 //A1
#define XM A1 //A2
#define YM 6   //7
#define XP 7   //6
//-- Screen coordinates 240 x 320 resolution 
#define TS_MINX  140
#define TS_MINY  90
#define TS_MAXX  880
#define TS_MAXY  887
//-- Init TouchScreen :
TouchScreen ts = TouchScreen(XP, YP, XM, YM, Sens);
//-- LCD Pin
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4 // Optional : otherwise connect to Arduino's reset pin
//-- Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE     0x001F
#define	RED    0xF800 
#define	GREEN   0x07E0
#define CYAN    0xFFE0 
#define MAGENTA 0xF81F
#define YELLOW  0x07FF
#define WHITE   0xFFFF

int second=620;
int second_check;
int second_modulo;
int minute;
int saved_time;
int secondcount0=0;
int secondcount1=0;
double temp_target=45.0;
double temp_actual=40.0;
boolean stop_timer=true;
boolean set_timer=false;
boolean time_changed=true; //show default countdown
boolean pump_on=false;
boolean pump_status=true;
boolean warm_status=false;
boolean heat_on=false;
boolean heat_status=true;
Thermistor100k thermo;





//-- Init LCD
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//-- Setup
void setup(void) {
  
  Timer1.initialize();                  // Initialise timer 1
  Timer1.attachInterrupt( Countdown );           // attach the ISR routine here


  
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341);
  tft.fillScreen(BLACK);
  tft.setRotation(1);

  
  tft.drawRect(0, 0, 85, 45, RED);
  tft.fillRect(0, 0, 85, 45, RED);
  tft.setCursor(22, 5);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Set");
  tft.setCursor(15, 25);
  tft.println("Stop");

  tft.drawRect(119, 0, 85, 45, GREEN);
  tft.fillRect(119, 0, 85, 45, GREEN);
  tft.setCursor(130, 15);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Start");

  tft.drawRect(235, 0, 85, 45, RED);
  tft.fillRect(235, 0, 85, 45, RED);
  tft.setCursor(250, 15);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Warm");

  tft.fillRect(178, 65, 60, 25, BLACK);
  tft.setCursor(178, 75);
  tft.setTextColor(BLUE, BLACK);
  tft.setTextSize(3);
  tft.print("Temp");

  //Target Temp
  tft.setCursor(180, 115);
  tft.fillRect(178, 113, 75, 28, BLACK);
  tft.drawRect(178, 113, 75, 28, WHITE);
  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(3);
  tft.println(temp_target, 1);

  //actual Temp
  tft.setCursor(180, 145);
  tft.fillRect(178, 143, 75, 28, BLACK);
  tft.drawRect(178, 143, 75, 28, WHITE);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.println(temp_actual, 1);

  tft.fillRect(265, 65, 40, 40, GREEN);
  tft.fillRect(275, 81, 20, 8, WHITE);
  tft.fillRect(281, 75, 8, 20, WHITE);

  tft.fillRect(265, 145, 40, 40, RED);
  tft.fillRect(275, 161, 20, 8, WHITE);
  
  
  
 

  
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void Countdown()
{
  secondcount0=secondcount0+1;
  if (!stop_timer){
  second=second -1;
  if (second<1){
  pump_on=false;
  stop_timer=true;
  second=saved_time;
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second_modulo, DEC);
  Serial.print("\n");
  }
}



void loop()
{

  if (secondcount0!=secondcount1){
    secondcount1=secondcount0;
    Serial.print("\n");
    Serial.print("Uptime in seconds:");
    Serial.print(secondcount0, DEC);



   
  //actual Temp
  tft.setCursor(180, 145);
  tft.fillRect(178, 143, 75, 28, BLACK);
  tft.drawRect(178, 143, 75, 28, WHITE);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.println(thermo.get_temp(5), 1);
  }
 
 

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      
    // scale from 0->1023 to tft.width  
    //Original code, contain bug (inverted coordinate)   
    //Code below to fix the bug of inverted coordinates
   p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
   p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
   
    
    //Correct offset of touch. Manual calibration
    p.x+=CalX;
    p.y+=CalY;
  

  if (p.x >0 && p.x<40 && p.y > 0 && p.y < 85) { //Set Countdown
  stop_timer=true;
  set_timer=true;
  tft.fillRect(0, 70, 50, 40, GREEN);
  tft.fillRect(15, 86, 20, 8, WHITE);
  tft.fillRect(21, 80, 8, 20, WHITE);
  
  tft.fillRect(0, 150, 50, 40, RED);
  tft.fillRect(15, 166, 20, 8, WHITE);
  
  tft.fillRect(70, 70, 50, 40, GREEN);
  tft.fillRect(85, 86, 20, 8, WHITE);
  tft.fillRect(91, 80, 8, 20, WHITE);

  tft.fillRect(70, 150, 50, 40, RED);
  tft.fillRect(85, 166, 20, 8, WHITE);
  
  }

  

  if (p.x >0 && p.x<40 && p.y > 119 && p.y < 204) { //start Countdown
  stop_timer=false;
  set_timer=false;
  pump_on=true;
  heat_on=true;
  tft.fillRect(0, 70, 50, 40, BLACK);
  tft.fillRect(0, 150, 50, 40, BLACK);
  tft.fillRect(70, 70, 50, 40, BLACK);
  tft.fillRect(70, 150, 50, 40, BLACK);
  }

  if (p.x >0 && p.x<50 && p.y > 235 && p.y < 320) {
  pump_on = false;
  heat_on=false;
  warm_status = !warm_status;
  if (warm_status==true)
  {
  tft.drawRect(235, 0, 85, 45, GREEN);
  tft.fillRect(235, 0, 85, 45, GREEN);
  tft.setCursor(250, 15);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Warm");
  pump_on=true;
  heat_on=true;
  }
  else
  {
  tft.drawRect(235, 0, 85, 45, RED);
  tft.fillRect(235, 0, 85, 45, RED);
  tft.setCursor(250, 15);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Warm");
  pump_on=false;
  heat_on=false;
  }
  }

  tft.fillRect(0, 220, 150, 20, BLACK);

  
  tft.setCursor(0, 220);
  tft.setTextColor(BLUE, BLACK);
  tft.setTextSize(2);
  tft.print(p.x, DEC);
  
  tft.setCursor(50, 220);
  tft.setTextColor(BLUE, BLACK);
  tft.setTextSize(2);
  tft.print(p.y, DEC);

  tft.setCursor(100, 220);
  tft.setTextColor(BLUE, BLACK);
  tft.setTextSize(2);
  tft.print(p.z, DEC);
  
  }

  if (second_check!=second){
  minute=second/60;
  second_modulo= second % 60;
  tft.fillRect(0, 120, 150, 25, BLACK);
  tft.setCursor(20, 120);
  tft.setTextColor(RED, BLACK);
  tft.setTextSize(3);
  if (minute<10){
  tft.print("0");
  tft.setCursor(39, 120);  
  }
  tft.print(minute, DEC);
  tft.setCursor(55, 120);
  tft.setTextColor(WHITE, BLACK);
  tft.print(":");
  tft.setCursor(72, 120);
  tft.setTextColor(GREEN, BLACK);
  if (second_modulo<10){
  tft.setCursor(72, 120);
  tft.print("0");  
  tft.setCursor(91, 120);
  }
  if (second_modulo>=10){
  tft.setCursor(72, 120);
  }
  tft.print(second_modulo, DEC);
  time_changed=false;
  }
  second_check=second;

  if (p.x >70 && p.x<110 && p.y > 0 && p.y < 50 && set_timer==true){
  second=second+60;
  saved_time=second;
  }

  if (p.x >150 && p.x<190 && p.y > 0 && p.y < 50 && set_timer==true){
  second=second-60;
  saved_time=second;
  }

   if (p.x >70 && p.x<110 && p.y > 70 && p.y < 120 && set_timer==true){
  second++;
  saved_time=second;
  }

  if (p.x >150 && p.x<190 && p.y > 70 && p.y < 120 && set_timer==true){
  second--;
  saved_time=second;
  }

  if (p.x >65 && p.x<105 && p.y > 265 && p.y < 305){
  temp_target=temp_target+0.1;
  tft.setCursor(180, 115);
  tft.fillRect(178, 113, 75, 28, BLACK);
  tft.drawRect(178, 113, 75, 28, WHITE);
  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(3);
  tft.println(temp_target, 1);
  }

  if (p.x >145 && p.x<185 && p.y > 265 && p.y < 305){
  temp_target=temp_target-0.1;
  tft.setCursor(180, 115);
  tft.fillRect(178, 113, 75, 28, BLACK);
  tft.drawRect(178, 113, 75, 28, WHITE);
  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(3);
  tft.println(temp_target, 1);
  }

  if (pump_status!=pump_on){
  if (pump_on)
  {
  tft.drawCircle(200, 215, 20, GREEN);
  tft.drawTriangle(200,235,200,195,220,215, GREEN);  
  }
  else
  {
  tft.drawCircle(200, 215, 20, WHITE);
  tft.drawTriangle(200,235,200,195,220,215, WHITE);
  }
  pump_status=pump_on;
  }

  if (heat_status!=heat_on){
  if (heat_on)
  {
  tft.drawRect(240,195,20,40,GREEN);
  tft.drawFastHLine(245,205,10, GREEN);
  tft.drawFastHLine(245,215,10, GREEN);
  tft.drawFastHLine(245,225,10, GREEN);
  }
  else
  {
  tft.drawRect(240,195,20,40,WHITE);
  tft.drawFastHLine(245,205,10, WHITE);
  tft.drawFastHLine(245,215,10, WHITE);
  tft.drawFastHLine(245,225,10, WHITE);
  }
  heat_status=heat_on;
  }

  

  
}
