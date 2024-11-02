/**
  ******************************************************************************
  * @file    VFO_LED_DISPLAY_BFO_1_DDS-1
  * @author  Stefano Homebrew
  * @version V1.0
  * @date    02-11-2024
  *
  *
  ******************************************************************************
  *
  *                     VFO_LED_DISPLAY_BFO_1_DDS-1 - PROJECT
  *
  * This project "VFO_LED_DISPLAY_BFO_1_DDS-1 with DDS AD9850", define an experimental open platform to build
  * a RX Vfo for Radio's Homebrewing 
  * 
  * BUTTONS:
  * bottoneBfo:  BFO ON/OFF
  * bottoneBand: FOR DEFINED RADIO BANDS
  * bottoneMfrq: I.F. FREQUENCY VALUE
  * bottoneStep: ROTARY ENCODER BUTTON FOR STEP FREQUENCY
  * 
  * Library: 
  * Led_Control https://www.pjrc.com/teensy/td_libs_LedControl.html
  * Rotary Encoder https://github.com/brianlow/Rotary
  * DDS Library https://github.com/F4GOJ/AD9850
  * 
  * 
  * My projects videos https://www.youtube.com/@stefanohomebrew
  *
  * NOTE: this is an experimental project and the functions can be changed
  * without any advise.
  * The VFO_LED_DISPLAY_BFO_1_DDS-1 uses examples and projects freeware and available in the opensource
  * community.
  *
  * The VFO_LED_DISPLAY_BFO_1_DDS-1 openSource software is released under the license:
  *              Common Creative - Attribution 3.0
  ******************************************************************************
*/

#include "LedControl.h"
#include <Rotary.h>
#include "Wire.h"
#include <AD9850.h>

unsigned long freq_old = 0;
unsigned long freq = 9670000; 
unsigned long freq3 = (freq);
unsigned long freqSi = 455000;
unsigned long freqPLL = 0;

double trimFreq = 125000000;//24999500;
int phase = 0;

long bfoPLL = 455000;
byte bfo= 0;
byte varBfo= 0;
int passo = 10000;
byte pass= 1;
byte band = 0;
byte mdfrq = 0;

const int W_CLK_PIN = 12;
const int FQ_UD_PIN = 11;
const int DATA_PIN = 10;
const int RESET_PIN = 9;


int bottoneStep = A0;
int bottoneBand = A1;
int bottoneMfrq = A2;
int bottoneBfo = A3;

LedControl lc=LedControl(6,8,7,1);
Rotary r = Rotary(3, 2);



void setup() {
    Serial.begin (9600);
    pinMode(bottoneStep,INPUT_PULLUP);
    pinMode(bottoneBand,INPUT_PULLUP);
    pinMode(bottoneMfrq,INPUT_PULLUP);
    pinMode(bottoneBfo,INPUT_PULLUP);
    pinMode(5,OUTPUT);

    digitalWrite(5, LOW);
    
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
    sei();
//////////////////////////////////////////////SI5351 INIT VALUE
DDS.begin(W_CLK_PIN, FQ_UD_PIN, DATA_PIN, RESET_PIN);
DDS.calibrate(trimFreq);
    
//////////////////////////////////////////////LED INIT VALUE    
    lc.shutdown(0,false);// turn off power saving, enables display
    lc.setIntensity(0,10);// sets brightness (0~15 possible values)
    lc.clearDisplay(0);// clear screen
    lc.setChar(0,7,'-',false);
    lc.setChar(0,6,'-',false);
    lc.setChar(0,5,'-',false);
    lc.setChar(0,4,'-',false);
    lc.setChar(0,3,'-',false);
    lc.setChar(0,2,'1',false);
    lc.setChar(0,1,'.',false);
    lc.setChar(0,0,'0',false);
    delay(2000);  
    printNumber(freq3);
    inviaDds();  
}
 
/////////////////////////////////////////////////////  
ISR(PCINT2_vect) {
  unsigned char result = r.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {  
    freq = (freq + passo);
    freq3 = (freq);
   printNumber(freq3);
      Serial.println("freqPLL");
}
  else if (result == DIR_CCW) {   
    freq = (freq - passo);
    freq3 = (freq);
   printNumber(freq3);
      Serial.println("freqPLL");


  } 
}
//////////////////////////////////////////////////////////////////
void loop() {
  
////////////////////////////BOTTONE RTX PORT A3///DIVENTA BOTTONE BFO ON/OFF
  if (digitalRead(bottoneBfo) == LOW){// ATTIVA DISATTIVA BFO
  delay(300); 
  bfo1();
  }
//////////////////////////////END BOTTONE RTX  
    if (digitalRead(bottoneStep) == LOW) {
    delay(200); 
    Step();
    }
    ////////////////////////////////START OPTIONAL BANDS SWITCH
 
    if (digitalRead(bottoneBand) == LOW) {
    delay(200); 
    Bands();
    }
   
    ////////////////////////////////END OPTIONAL BANDS SWITCH
    if (digitalRead(bottoneMfrq) == LOW) {
    delay(200); 
    MediaFrq();
    }
    /////////////////////////////////////IF ROTARY ENCODER ROTATION         
     if (freq != freq_old)  {   
     inviaDds();
     freq_old = freq;
     }
}
////////////////////////////////
void inviaDds(){
DDS.setfreq(freq - freqSi, phase);
delay(10);


}

void printNumber(long num)
{
  long terzo;
  long quinto;
  long quarto;
  long sesto;
  long settimo;
  long ottavo;
  long var4=(long)num;
  long secondo,primo; 
  primo=var4%10;
  var4=var4/10;
  secondo=var4%10;
  var4=var4/10;
  terzo=var4%10;
  var4=var4/10;
  quarto=var4%10;
  var4=var4/10;
  quinto=var4%10; 
  var4=var4/10; 
   sesto=var4%10; 
  var4=var4/10; 
  settimo=var4%10; 
  var4=var4/10; 
  ottavo=var4%10; 
  var4=var4/10; 
  
  //Now print the number digit by digit
  lc.setDigit(0,7,(byte)ottavo,false);
  lc.setDigit(0,6,(byte)settimo,true);
  lc.setDigit(0,5,(byte)sesto,false);
  lc.setDigit(0,4,(byte)quinto,false);
  lc.setDigit(0,3,(byte)quarto,true);
  lc.setDigit(0,2,(byte)terzo,false);
  lc.setDigit(0,1,(byte)secondo,false);
  lc.setDigit(0,0,(byte)primo,false);
  delay (300);  
}
////////////////////////////////STEP     
void Step(){   
    pass++;
    if (pass > 3) {
      (pass = 1);
    }
    if (pass < 1) {
      (pass = 1);
    }
   if (pass == 1) {
   printNumber(1000);
    
      passo = 1000;
    }
   if (pass == 2) {
   printNumber(10000);
      
      passo = 10000;
    }
   if (pass == 3) {
   printNumber(100);
     
      passo = 100;    
    }        
 Serial.println(passo);   
}

////////////////////////////////OPTIONAL BANDS SWITCH
void Bands(){  
   band++;
   if (band == 1) {
    freq = 9670000;
    inviaDds();
    freq3 = (freq);
   printNumber(freq3);
     
      passo = 1000;
    }
    if (band == 2) {
    freq = 10000000;
    inviaDds();
    freq3 = (freq);
   printNumber(freq3);
    
      passo = 1000;
    }   
   if (band == 3) {
    freq = 30000000;
    inviaDds();
    freq3 = (freq);
   printNumber(freq3);
     
      passo = 1000;
    }   
   if (band == 4) {
    freq = 40000000;
    inviaDds();
    freq3 = (freq);
   printNumber(freq3);
      
      passo = 1000;
    }
       if (band == 4) {
        band = 0;
       }
   //    Serial.println(band);
}
////////////////////////////////END BANDS 
/////////////////////////////////START MEDIA FRQ
void MediaFrq(){  
   mdfrq++;
   if (mdfrq == 1) {
    freqSi = 0;
    inviaDds();
   
   printNumber(0);
     

    }   
   if (mdfrq == 2) {
    freqSi = 455000;
    inviaDds();
   printNumber(freqSi);
      
      passo = 1000;
    }   
   if (mdfrq == 3) {
    freqSi = 450000;
    inviaDds();

   printNumber(freqSi);
    

    }
       if (mdfrq == 3) {
        mdfrq = 0;
       }
      // Serial.println(mdfrq);
}
///////////////////////////////////////////////BFO OUT - non attivo
void bfo1(){ 
  /*
    bfo++;
     switch (bfo) {  
case 1:   
    si5351.output_enable(SI5351_CLK1, 1);
    si5351.set_freq ((bfoPLL * 100), SI5351_CLK1);// non attivo sul DDS
    lc.setChar(0,7,'-',false);
    lc.setChar(0,6,'B',false);
    lc.setChar(0,5,'F',false);
    lc.setChar(0,4,'0',false);
    lc.setChar(0,3,'-',false);
    lc.setChar(0,2,'4',false);
    lc.setChar(0,1,'5',false);
    lc.setChar(0,0,'5',false);
break;
case 2:
    si5351.output_enable(SI5351_CLK1, 0);//non attivo sul DDS
    lc.setChar(0,7,'-',false);
    lc.setChar(0,6,'0',false);
    lc.setChar(0,5,'F',false);
    lc.setChar(0,4,'F',false);
    lc.setChar(0,3,'-',false);
    lc.setChar(0,2,'4',false);
    lc.setChar(0,1,'5',false);
    lc.setChar(0,0,'5',false);   
    bfo=0;
break;     
    }
    */ 
} 
      
