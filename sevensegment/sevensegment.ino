/*
RatiTutorials @ http://ratitutorials.wordpress.com

2 digit 7 Segment display example:
Using the P1.3 pushbutton on TI's MSP430-2553 this sketch will increment
the connected 7 segment display all the way up to 99 using timer interrupts
which means that this fully runs in the background!

*/


#define LED 2
#define DIGIT_L 7
#define DIGIT_R 6

// Global Variables
const int buttonPin = PUSH2;     // the number of the pushbutton pin
const byte number[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};  // Default bit configurations for numerically counting through a 7 seg display
boolean flicker = true;       // Used in 7 seg display ISR to switch between digits
uint8_t value = 0;           //  Global value used to display to 7 seg
  
// ------------------------ Setup ------------------------- //
void setup() {
  // Set pin directions  
  pinMode(LED, OUTPUT);    
  pinMode(DIGIT_L, OUTPUT);
  pinMode(DIGIT_R, OUTPUT); 
  pinMode(buttonPin, INPUT_PULLUP);
  for (uint8_t i=8; i<15; ++i){
     pinMode(i, OUTPUT);
  }
  
  // Set default values
  digitalWrite(DIGIT_L, HIGH);  // Default value as off
  digitalWrite(DIGIT_R, HIGH);  // Default value as off
}


// ------------------------ Main ------------------------- //
void loop(void) {

  // Setting up timer interrupt
  // Check out this awesome blog by Enrico Garante which was the driving force behind the values set in this sketch
  // Keep watchdog timer on to run program.  May disable it if program runs just on interrupts
  // http://www.embeddedrelated.com/showarticle/182.php
  
  // WDTCTL = WDTPW + WDTHOLD;
  CCTL0 = CCIE;    // Enable CCR0 to hold value to count to
  // http://www.npeducations.com/2013/05/understanding-timers-in-msp430-launch.htmlv
  TACTL = TASSEL_1 + MC_2 + ID_0;  // Check the link out above to understand this line of code which basically sets the speed at which the interrupt clock counts
  CCR0 = 5000;  // Initial value the interrupt clock counts to. This value will change in the ISR
  //_BIS_SR(LPM0_bits + GIE);  // Set MSP430 in Low Power Mode and enable global variables
  
  while(true){
      if(!digitalRead(buttonPin)){
        // Need to put some debouncing stuff here...
        while(!digitalRead(buttonPin)){;}
        P1OUT ^= BIT0;  // Blink LED
        value+=1;
        if (value > 99) value = 1;  // Reset value if it reaches 100! My 7 seg display can only count to 99
      }
  }
}

// ------------------------ Interrupt Service Routine (ISR) ------------------------- //
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
  //P1OUT ^= BIT0;  // Blink LED
  flicker = !flicker;  // This changes which 7 seg digit is being used
  
  digitalWrite(DIGIT_L, flicker);  // Left digit enable on 7 seg display
  digitalWrite(DIGIT_R, !flicker); // Right digit enable on 7 seg display
  
  if (flicker){  // #Left
    for (uint8_t i=8; i<15; ++i){  // 7 seg LEDs are connected on pins 8 - 14
      digitalWrite(i, (number[int(value/10)] >> 14 - i) & 0x01);  // Get first digit by (value/10), look it up in "number" array and write all 7 bits to 7 seg LEDs
    }
  }
  else {      // #Right
    for (uint8_t i=8; i<15; ++i){  // 7 seg LEDs are connected on pins 8 - 14
       digitalWrite(i, (number[int(value%10)] >> 14 - i) & 0x01); // Get second digit by (value%10) which is moduls, look it up in "number" array and write all 7 bits to 7 seg LEDs
    }
  }
  
  CCR0 += 100;  // Set new timer value for interrupt to count to. The higher the number the slower this ISR will be exacuted. 
                // If this number is higher than 500 then most people will see the 7 seg display flicker badly!
}


