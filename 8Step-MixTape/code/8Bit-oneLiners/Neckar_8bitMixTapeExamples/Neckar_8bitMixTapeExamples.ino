// 8Step MixTape Example Code
// Basic Onliner
// Crazy shit 8-bit symphony generator                   
/*
 * inspired by:
 *  http://countercomplex.blogspot.com/2011/10/algorithmic-symphonies-from-one-line-of.html
 *  http://countercomplex.blogspot.com.es/2011/10/some-deep-analysis-of-one-line-music.html
 *
 *  try it online:
 *  http://wurstcaptures.untergrund.net/music/
 *
 */
 
// ATMEL ATTINY85 / ARDUINO
//
//                      +-\/-+
//  Reset A0 (D 5) PB5 1|    |8  Vcc
//        A3 (D 3) PB3 2|    |7  PB2 (D 2) A1
//   A2 pwm4 (D 4) PB4 3|    |6  PB1 (D 1) pwm1
//                 GND 4|    |5  PB0 (D 0) pwm0
//                      +----+   

int speakerPin = PB0;
int button1 = PB1;
int button2 = PB3;

int pot1 = A1; //PB2
int pot2 = A2; //PB4
 
int buttonState1 = 0; 
int lastButtonState1 = 0;
int count = -1;

long t = 0; 

unsigned int p0 = 0;
unsigned int delta_T = 0;

int snd = 0; 

void setup () {
  TCCR0A |= (1<<WGM00)|(1<<WGM01); //Fast pwm
  TCCR0B = TCCR0B & B11111000 | B00000001; //no timer pre-scaler, fast PWM
 
  pinMode (speakerPin, OUTPUT);
  pinMode (button1, INPUT);
  pinMode (button2, INPUT);
  pinMode (pot1, INPUT);
  pinMode (pot2, INPUT);

}
 
void loop () {
    // read the state of the switch into a local variable:
    buttonState1 = digitalRead(button1);
    
    // read the potentiometers into a local variable:
    p0 = (analogRead(pot1)>>4);
    delta_T = ((analogRead(pot2)));
    
    if (buttonState1 != lastButtonState1 && buttonState1 == HIGH) {
    // if the state has changed, increment the counter
      count++;
      t = 0; 
      delay(1000); 
      if (count > 2) {
        count = 0;
      } 
    
    }

    lastButtonState1 = buttonState1;

    switch(count) {
       case 0: // a classic
       snd = t*((t>>12|t>>8)&63&t>>(p0>>3));
       break;
       case 1: // //dubStep
       snd = t>>3&1?t>>4:-t>>(p0>>3) ; //dubStep
       break;
    }
    
    //snd = t>>3&1?t>>4:-t>>(p0>>3) ; //dubStep
    //snd = (t|3) * ((t>>1|t>>6)&p0&t>>3); // 4onTheFloor
    //snd = (t|4) * ((t>>17|t>>11)&p0&t>>3);
    //snd = t>>3&1?t>>17:-t>>p0;
    //snd = t*t/p0;
    //snd = (t|5) * ((t>>p0|t>>11)&p0&t>>3);
    //snd = (t*(t>>8|t>>4))>>(t>>p0);

    //Tribute to Klaus
    //snd = (( ( (t/( ((t>>12)&3)==0?p0:((t>>12)&3)==1?8:((t>>12)&3)==2?12:16) ) & (t>>((10+((t>>16)&3))))) | ((t/( ((t>>14)&3)==0?6:((t>>14)&3)==1?8:((t>>14)&3)==2?12:16)) & (t>>((11+((t>>18)&1))))) )&1)*10;
    //snd = ((((t/3)&(t>>11))|((t/5)&(t>>p0)))&7)*30; 
    //snd = ( (((t>>9)&1)|((t>>10)&2)|((t>>8)&4)|((t>>9)&8)) & ((((t/3)&1)|((t/4)&2)|((t/5)&4)|((t/6)&8) ) )&15)*15;
    //snd = (((((t/3)&((t/2)>>13)))^(((t/2)&((t/(p0>>3))>>9))))&p0)*15;
    
    analogWrite (speakerPin, snd);
    //digitalWrite (speakerPin, snd);
    delayMicroseconds((delta_T+1>>4)+0);
    t++;
 
}
