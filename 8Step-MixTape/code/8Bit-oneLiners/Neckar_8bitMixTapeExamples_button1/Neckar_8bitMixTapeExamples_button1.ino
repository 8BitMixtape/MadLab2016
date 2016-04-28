// 8Bit MixTape Example Code
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
//                    +-\/-+
//    Reset A0 / PB5 1|    |8  Vcc
//  button2 A3 / PB3 2|    |7  PB2 / A1 = pot1
//   A2 = pot2 / PB4 3|    |6  PB1 / pwm1 button 1
//               GND 4|    |5  PB0 / pwm0 speakerPin
//                    +----+   

int speakerPin = PB0;
int button1 = PB1;
int button2 = PB3;

int pot1 = A1; //PB2
int pot2 = A2; //PB4
 
int buttonState1 = 0; 
int lastButtonState1 = 0;
int count = 0;

int buttonState2 = 0; 
int lastButtonState2 = 0;
int outputStyle = 0;

long t = 0; 

unsigned int p0 = 0;
unsigned int delta_T = 0;

int snd = 0; 

void setup () {
  TCCR0B = TCCR0B & B11111000 | B00000001; //no timer pre-scaler, fast PWM
 
  pinMode (speakerPin, OUTPUT);
  pinMode (button1, INPUT_PULLUP);
  pinMode (button2, INPUT_PULLUP);
  pinMode (pot1, INPUT);
  pinMode (pot2, INPUT);

}
 
void loop () {
    // read the state of the switch into a local variable:
    buttonState1 = digitalRead(button1);
    buttonState2 = digitalRead(button2);
    
    // read the potentiometers into a local variable:
    p0 = (analogRead(pot1)>>4);
    delta_T = ((analogRead(pot2)));
    
    if (buttonState1 != lastButtonState1 && buttonState1 == LOW) {
    // if the state has changed, increment the counter
      count++;
      t = 0; 
      delay(1000); 
      if (count > 8) {
        count = 0;
      } 
    }
    lastButtonState1 = buttonState1;
    
    if (buttonState2 != lastButtonState2 && buttonState2 == LOW) {
    // if the state has changed, increment the counter
      outputStyle++;
      
      if (outputStyle > 1) {
        outputStyle = 0;
      } 
    }
    lastButtonState2 = buttonState2;

    //count = 5;
    
    switch(count) {
     case 0: // //dubStep
       snd = t>>3&1?t>>4:-t>>(p0>>3) ; //dubStep
       break;
       
     case 1: // a classic
       snd = (t*(t>>8|t>>4))>>(t>>p0);
       break;
     
     case 2: // a classic
       snd = (t|3) * ((t>>1|t>>6)&p0&t>>3);
       break;

     case 3: // a classic
       snd = (t|5) * ((t>>p0|t>>11)&p0&t>>3);
       break;
       
     case 4: // a classic
       snd = t*((t>>12|t>>8)&63&t>>(p0>>3));
       break;
       
     case 5: // a classic
       snd = t * ((t>>p0|t>>3)&17&t>>9);
       break; 
      
     case 6: // a classic
       snd = t*t/p0;
       break;
        
     case 7: // Tribute to Klaus
       snd = ((((t/3)&(t>>11))|((t/5)&(t>>p0)))&7)*30;  
       break;
    }

   //outputStyle = 0;
   
   if (outputStyle == 0) {
    analogWrite (speakerPin, snd);
   }
   if (outputStyle == 1) {
    digitalWrite (speakerPin, snd);
   }
   
   delayMicroseconds((delta_T+1>>0)+0);
   t++; 
}
