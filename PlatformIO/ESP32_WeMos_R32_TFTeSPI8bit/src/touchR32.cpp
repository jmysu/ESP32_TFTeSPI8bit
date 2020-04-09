/*
https://www.instructables.com/id/4-wire-Touchscreen-Interfacing-With-Arduino/

DOCUMENTATION BY CHANDAN KUMAR 01/03/2015
INTERFACE 4 WIRE RESISTIVE TOUCH SCREEN WITH ARDUINO
           ________________
          |                |
          |     TOUCH      |
          |     SCREEN     |
          |                |
           ________________
           |   |    |    |
           +   +    +    +
          |X1| |Y1| |X2| |Y2|
           ||   ||   ||   ||
          -X13 Y12  +X34  (Y35)
           X13 -Y12 (X34) +Y35
             
//FOR Y-AXIS:----------------------------------
   X1 GND
   X2 +5V
   MAKE Y1 AND Y2 AS INPUT MEANS HIGH IMPEDANCE
   NOW READ ANALOG VALUE ON Y2

//FOR X-AXIS:----------------------------------
   Y1 GND
   Y2 +5V
   MAKE X1 AND X2 AS INPUT MEANS HIGH IMPEDANCE
   NOW READ ANALOG VALUE ON Y2

*/        
#include <Arduino.h>
#include <Wire.h>      

int range[16][2] ={
            {210,10},{220,10},{200, 50},{190, 90},
            {180,10},{180,20},{160, 70},{130,110},
            {100,10},{110,40},{100,100},{ 65,145},
            { 10,10},{ 10,60},{ 10,128},{ 10,180}
            };

//Wemos R32 + McuFriend 2.4" touch screen
int XP=13, XM=34, YP=12, YM=35;

/*
   Set +Y HIGH and -X LOW.  
   Do analogRead() at -Y for Z.
   
   return pressure from 0~330
*/
int getTouchZ()
{
   int z;            
       pinMode(XP,INPUT);
       pinMode(YM,INPUT);
       pinMode(YP,OUTPUT);
       pinMode(XM,OUTPUT);
       digitalWrite(YP,HIGH);
       digitalWrite(XM,LOW);
       delayMicroseconds(20);
       z=700-analogRead(YM); //YM=700~370

   return z;
}

int getTouchX()
{
   int x;
       pinMode(XP,INPUT);
       pinMode(XM,INPUT);
       pinMode(YP,OUTPUT);
       pinMode(YM,OUTPUT);
       digitalWrite(YP,HIGH);
       digitalWrite(YM,LOW);
       delayMicroseconds(20);
       x=analogRead(XM);
   return x;    
}

int getTouchY()
{
   int y;
       pinMode(YP,INPUT);
       pinMode(YM,INPUT);
       pinMode(XP,OUTPUT);
       pinMode(XM,OUTPUT);
       digitalWrite(XP,HIGH);
       digitalWrite(XM,LOW);
       delayMicroseconds(20);
       y=analogRead(YM);
   return y;    
}

//-------------------------------------
void setup() 
{
Serial.begin(9600);
}

void loop() 
{
   int x, y, z;
   char buf[64];
   int r;

       x= getTouchX();
       y= getTouchY();
       z= getTouchZ();

       if (z>0) {
         for (r=0;r<16;r++) 
            if ( (x<(range[r][0]+40)) && (x>(range[r][0]-40)) )
               if ( (y<(range[r][1]+40)) && (y>(range[r][1]-40)) ){
                  break;
                  }

                     
         sprintf(buf, "x:%4d y:%4d z:%4d RX:%d RY:%d\n", x, y, z, r/4, r%4); 
         Serial.print(buf); 
         delay(30); //debounce
         }
}
