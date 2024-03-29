#include <TimerOne.h>

/*
  Scrounged LED sign found at ER 2 by HeatSync Labs.

  The sign uses differential inverting amplifiers LM2901 to drive Schmitt−Trigger
  inverters (74HCT14) and utimately drive two parallel rows of ST2221A LED drivers.

  Top and bottom display the SAME data -- so you must blank them alternately using the top and bottom enable lines!
  Don't do this too slowly or your user will get a flicker headache

  When running on an arduino nano (16Mhz) it takes about 380 microseconds to shift out data to the display (usually this will be 1/2 of the display)
*/
//uncomment below to output highlife instead of Conway (highlife is more active and feeds fed less often)
//#define HIGHLIFE

#define TOP 0
#define BOT 1

int A[] ={00100,
          01001,
          01111,
          01001,
          01001};

#define bit0 B00000001
#define bit1 B00000010
#define bit2 B00000100
#define bit3 B00001000
#define bit4 B00010000
#define bit5 B00100000
#define bit6 B01000000
#define bit7 B10000000

//VCC J1 pin13
//GND J1 pin15

#define DL_N 3 //data and latch reference (data and latch are tied on the negative side, positive sides MUST transition together, and oppose this) //PD3
#define DLP_N bit3 //PD3 //J1 pin6

#define DATA_P 7 //PD7 //J1 pin5
#define DATAP_P bit7 //PD7

#define LATCH_P 2 //PD2 //J1 pin7
#define LATCHP_P bit2 //PD2

#define CLOCK_P 5 //PD5
#define CLOCK_N 6 //PD6

#define CLOCKP_P bit5 //PD5  //J1 pin3
#define CLOCKP_N bit6 //PD6 //J1 pin2

#define BOT_EN 8 //PB0 //J1 pin9 //Might have a differential friend, but this signal doesn't need to be fast so it works as-is
#define TOP_EN 9 //PB1 //J1 pin11 //Might have a differential friend, but this signal doesn't need to be fast so it works as-is

#define BOTP_EN bit0
#define TOPP_EN bit1

#define CLK_US 0
// the setup function runs once when you press reset or power the board

#define VisDuration 500 //this controls the overall brightness, don't go above 7500 or it gets flickery
//const int Height = 14; //top bit of each byte half is not shown!
#define Height 14
#define Width 36
//const int Width = 15;
//const int Width = 64;
unsigned int fbuffer[Width]; //LEDs -- high byte is bottom low byte is top. bit0 is the very top.
unsigned long time1=0;
unsigned long time2=0;
char timerState=BOT;

void setup() {
  randomSeed(analogRead(0));
  memset(fbuffer, 0, Width * 2);
  
  //we need some random pixels to set the board
  /*
  putPixel(0,0);    
  putPixel(1,1);    
  putPixel(2,2);    
  putPixel(3,3);    
  putPixel(4,4);    
  putPixel(5,5);    
  putPixel(6,6);    
  putPixel(7,7);    
  putPixel(8,8);    
  putPixel(9,9);    
  putPixel(10,10);    
  putPixel(11,11);    
  putPixel(12,12);    
  putPixel(13,13);    
  putPixel(14,14);    
  */

  
  for(int x=1; x < Width-1; x++)
  {
    for(int y=1; y < Height-1; y++)
      {
      if(random(100) > 50) 
        putPixel(x,y);
      }
  }
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(DATA_P, OUTPUT);
  pinMode(DL_N, OUTPUT);
  pinMode(CLOCK_P, OUTPUT);
  pinMode(CLOCK_N, OUTPUT);
  pinMode(LATCH_P, OUTPUT);

  pinMode(BOT_EN, OUTPUT);
  pinMode(TOP_EN, OUTPUT);

  digitalWrite(DATA_P, LOW);
  digitalWrite(LATCH_P, LOW);
  digitalWrite(DL_N, HIGH);

  digitalWrite(CLOCK_P, HIGH);
  //BOTTOM ROW
  digitalWrite(BOT_EN, HIGH);   digitalWrite(TOP_EN, LOW);
  //TOP ROW
  digitalWrite(BOT_EN, LOW);    digitalWrite(TOP_EN, HIGH);
  //TOP ROW
  digitalWrite(BOT_EN, HIGH);    digitalWrite(TOP_EN, HIGH);
  Timer1.initialize(VisDuration);
  Timer1.attachInterrupt(timerCallback);  
}

void timerCallback()
{
  //BLANK DISPLAY
  PORTB &= ~(BOTP_EN | TOPP_EN);

  //SHIFT OUT TOP OR BOTTOM DATA
  if(timerState == BOT)
    {
    timerState = TOP;
    writeFrameBuffer(BOT);
    //SHOW BOTTOM HALF
    PORTB |= BOTP_EN;  PORTB &= ~TOPP_EN;
    }
  else
    {
    timerState = BOT;
    writeFrameBuffer(TOP);
    //SHOW TOP HALF
    PORTB |= TOPP_EN;  PORTB &= ~BOTP_EN;
    } 
}

// the loop function runs over and over again forever
void loop() {
  //time = micros();
  //reset frame buffer
  //memset(fbuffer, 0, Width * 2); //don't clear the framebuffer every time maybe? we will have to call dead lifeforms by clearpixel
  
  //BLANK DISPLAY
  //PORTB &= ~(BOTP_EN | TOPP_EN);

  //Write to framebuffer here
  

  //if(micros() - time1 > 20000)
    // {
     
     //time1 = micros();
     //}
  
  //feed the cells every second
  //HIGHLIFE every 30 seconds
  #ifndef HIGHLIFE
  if(micros() - time2 > 3000000)
  #endif
  #ifdef HIGHLIFE
  if(micros() - time2 > 30000000)
  #endif
    {
     time2 = micros();
     for(int x=1; x < Width-1; x++)
       {
       for(int y=1; y < Height-1; y++)
        {
        
        if(random(100) > 90) 
          putPixel(x,y);
        }
       }
     }

    //writeATest();
  //SHIFT BOTTOM DATA 
  drawScreen(A,false); 
  
  //writeFrameBuffer(BOT);
  //SHOW BOTTOM HALF
  //PORTB |= BOTP_EN;  PORTB &= ~TOPP_EN;

  //delayMicroseconds(VisDuration);

  //BLANK DISPLAY
  //PORTB &= ~(BOTP_EN | TOPP_EN);

  //SHIFT OUT TOP DATA
  //writeFrameBuffer(TOP);
  //SHOW TOP HALF
  //PORTB |= TOPP_EN;  PORTB &= ~BOTP_EN;
  //delayMicroseconds(VisDuration);
}

void writeFrameBuffer(char TopOrBot)
{
  unsigned char RegShadow;
  unsigned char Byte;
  if (TopOrBot == 1) //high byte, top
  {
    for (int col = 0; col < Width; col++)
    {
      Byte = (fbuffer[col] >> 7); //only 7 bits used, not 8 (2 unused bits at top of word)
      for (long row = 0; row < 7; row++)
      {
        if (Byte & 0x1 == 0x1)
        {
          //Clock in a differential 1
          RegShadow = PORTD | DATAP_P | LATCHP_P;
          PORTD = RegShadow & ~(DLP_N);
        }
        else
        {
          //Clock in a differential 0
          RegShadow = PORTD | DLP_N;
          PORTD = RegShadow & ~(DATAP_P | LATCHP_P);
        }

        //Clock in data, but set and clear the differential
        //clock bits at the same time using a shadow reg
        RegShadow = PORTD | CLOCKP_P;
        PORTD = RegShadow & ~CLOCKP_N;

        //clear data lines back to zero (we don't really need this)
        //RegShadow = PORTD | DLP_N;
        //PORTD = RegShadow & ~(DATAP_P | LATCHP_P);

        //Reset Clock, but set and clear the differential
        //clock bits at the same time using a shadow reg
        RegShadow = PORTD | CLOCKP_N;
        PORTD = RegShadow & ~CLOCKP_P;

        Byte = Byte >> 1; //Get the next bit ready
      }
    }
  }
  else
  {
    for (int col = 0; col < Width; col++)
    {
      Byte = fbuffer[col];
      for (long row = 0; row < 7; row++)
      {
        if (Byte & 0x1 == 0x1)
        {
          //Clock in a differential 1
          RegShadow = PORTD | DATAP_P | LATCHP_P;
          PORTD = RegShadow & ~(DLP_N);
        }
        else
        {
          //Clock in a differential 0
          RegShadow = PORTD | DLP_N;
          PORTD = RegShadow & ~(DATAP_P | LATCHP_P);
        }

        //Clock in data, but set and clear the differential
        //clock bits at the same time using a shadow reg
        RegShadow = PORTD | CLOCKP_P;
        PORTD = RegShadow & ~CLOCKP_N;

        //clear data lines back to zero (we don't really need this)
        //RegShadow = PORTD | DLP_N;
        //PORTD = RegShadow & ~(DATAP_P | LATCHP_P);

        //Reset Clock, but set and clear the differential
        //clock bits at the same time using a shadow reg
        RegShadow = PORTD | CLOCKP_N;
        PORTD = RegShadow & ~CLOCKP_P;

        Byte = Byte >> 1; //Get the next bit ready
      }
    }
  }
}

//checks commented out for speed (but doesn't appear to matter much)
void clearPixel(unsigned int x, unsigned int y)
{
//if(x < Width && y < Height)
//  {
  //if(y > 6) 
   // y++;
  fbuffer[x] &= ~(1 << y);
//  }
}

//enable checks to allow invalid pixels to be chosen (this lets us work to the edge of the board)
bool getPixel(unsigned int x, unsigned int y)
{
if(x < Width && y < Height)
   {
//   if( y > 6) //top bit isn't output (7 rows each half), we have to shift up
//      y++; 
   if( (fbuffer[x] & (1<<y)) > 0) 
     return true;
   else 
     return false;
   }
return false;
}

void putPixel(unsigned int x, unsigned int y)
{
//if(x < Width && y < Height)
  //{
  //if(y > 6) //top bit isn't output (7 rows each half), we have to shift up
  //   y++;
  fbuffer[x] |= (1 << y);
  //}
}

void writeWordBuffer(unsigned int Word, int col)
{
  if (col < Width)
    fbuffer[col] |= Word;
}

void drawScreen(int Byte[],boolean space)
{ 
char board[Width][Height];

  for(int x = 0; x < 4; x++)
    {
    for(int y = 0; y < 4; y++)
      {
      if(board[x][y] == 1){
     
            putPixel(x,y);
         
      }else{
            clearPixel(x,y);

      }
      }
    }
}
       

      
 /* for(int x =0;x<5;x++){
    for(int y =0;y<5;y++){
      for(int i =0;i<Byte;i++){
        if(Byte[i]=1){
          putPixel(x,y);
        }else{
          clearPixel(x,y);

    }
    }
    }
  }*/

void writeByte(byte Byte[])
{
  unsigned char RegShadow;
  for (long row = 0; row < 5; row++)
  {
     for (byte a = 0; a < 5; a++)    // count next row
     {
          
         //Byte = (Byte[row] >> a) & 0x01;
         //putPixel(row,a);
     }
     //Get the next bit ready
  }
}

void writeATest(){
 char board[Width][Height];

  for(int x = 0; x < Width; x++)
    {
    for(int y = 0; y < Height; y++)
      {
      if(board[x][y] == 1){
         putPixel(35,8);
         putPixel(35,9);
         putPixel(35,10);
         putPixel(35,11);
         putPixel(35,12);
         putPixel(35,13);
  
         putPixel(34,7);
         putPixel(33,6);
         putPixel(32,7);

         putPixel(32,10);
         putPixel(33,10);
         putPixel(34,10);
         
         putPixel(31,8);
         putPixel(31,9);
         putPixel(31,10);
         putPixel(31,11);
         putPixel(31,12);
         putPixel(31,13);
      }else{
         clearPixel(x,y);
      }
      }
    }
}
