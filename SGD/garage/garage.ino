/*
  Ratitutorials.wordpress.com  
  Email: ratitutorials@gmail.com
  
  Secure garage door opener project
  MSP430 Launchpad
 */
#define LED 15

String correctHash = "d4aea11b16e387b6ed1b96479b30b784";
int garage = 5;
char inputHash[32] = {0};


void setup() {
  Serial.begin(9600);  //Begin UART connection at 9600 BAUD
  pinMode(LED, OUTPUT);
  ledblink(200);  
}

// the loop routine runs over and over again forever:
void loop() {
  if(Serial.available()){
      ledblink(100);
      int set = Serial.available();  
     
      if(set == 32){
         Serial.readBytes(inputHash, set);
         
         for(int i=0; i<set; ++i){
            if(inputHash[i] != correctHash[i]) break;
            if(i == 31) openGarage(); 
         }
      } 
      delay(20000);  //Delay 20 seconds to discourage brute force attacks
      //Clear buffer
      while(Serial.available()) Serial.read();
  }
  delay(100);
}

void ledblink(int d){
    digitalWrite(LED, LOW);   
    delay(d);               
    digitalWrite(LED, HIGH);    
    delay(d);
}

void openGarage(){
  //ledblink(300);
  //ledblink(800);
  pinMode(garage, OUTPUT);
  digitalWrite(garage, HIGH);
  delay(1000);
  pinMode(garage, INPUT);
}



