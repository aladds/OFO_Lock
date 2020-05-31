#include <Arduino_nRF5x_lowPower.h>

// define pins (varies per shield/board)
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9


// Connections for ofo board
#define LED_PIN 30
#define BUZZ_PIN 0
#define LIM_BLUE 2
#define GPS_RX 3
#define GPS_TX 4
//#define TX_OTHER 5
//#define OUT_SW 6
#define LIM_GRN 7
#define MOTOR_PIN 11
#define KEY_B 12
#define KEY_4 13
#define KEY_3 14
#define KEY_2 15
#define KEY_1 16
#define LTE_PW1 18
#define LTE_PW2 19
#define KEY_PW 20
#define LTE_TX 21
#define LTE_RX 22
#define GPS_PW 25
//#define OUT_SW2 28


void beep(int times, int dur = 100);

//The code
String setCode = "1232";
String enterCode;

// Timing for sleep
unsigned long startupTime;
#define TIME_OUT 10000

void setup() {

  // Pin setup
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(LIM_BLUE, INPUT_PULLUP);
  pinMode(LIM_GRN, INPUT_PULLUP);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(KEY_B, INPUT_PULLUP);
  pinMode(KEY_4, INPUT_PULLUP);
  pinMode(KEY_3, INPUT_PULLUP);
  pinMode(KEY_2, INPUT_PULLUP);
  pinMode(KEY_1, INPUT_PULLUP);


  startupTime = millis();
  nRF5x_lowPower.enableWakeupByInterrupt(KEY_B, FALLING);

  //And low power mode
  nRF5x_lowPower.powerMode(POWER_MODE_LOW_POWER);
}

void isr(){
  beep(10);
  startupTime = millis();
}


void loop() {
  
  if(!digitalRead(KEY_1)){
    enterCode += "1";
    beep(1);
  }
  if(!digitalRead(KEY_2)){
    enterCode += "2";
    beep(1);
  }
  if(!digitalRead(KEY_3)){
    enterCode += "3";
    beep(1);
  }
  if(!digitalRead(KEY_4)){
    enterCode += "4";
    beep(1);
  }
  if(!digitalRead(KEY_B)){
    enterCode = "";
    beep(2);
    if(!digitalRead(KEY_B)){
      //TODO: Make it possible to change code without recompile, store in EEPROM.
      //beep(4,50);
      //if (unlocked() & enterCode != ""){
      //  setCode = enterCode;
      //}
    }
  }
  if (enterCode == setCode) {
    beep(3);
    unlock();
    enterCode = "";
    
  }

  if (millis() > (startupTime + TIME_OUT)){  // 10 seconds?
    //sleep now
    beep(5,50);
    nRF5x_lowPower.powerMode(POWER_MODE_OFF); // power down!
    
  }
}

void beep(int times, int dur) {
  for (int i=0;i<times;i++){
    digitalWrite(BUZZ_PIN,HIGH);
    delay(dur);
    digitalWrite(BUZZ_PIN,LOW);
    delay(dur);
  }
}

void unlock() {
  //Unlock routine: Run motor to release pin
  digitalWrite(MOTOR_PIN,HIGH);
  delay(200); // this siming and the 150 below may need to be tweaked depending on the power supply. 
              // From a 3.3v supply they need to be shorter, from a battery longer.
  while(digitalRead(LIM_GRN)) {
    //wait for motor to be in almost the right place
  }
  delay(150); // and a bit more
  digitalWrite(MOTOR_PIN,LOW); //now we're done.
  // Allow 5 seconds for user to retreive pin if it gets stuck
  delay(5000);
  lock();
}

void lock() {
  //lock routine: Run motor to release pin
  digitalWrite(MOTOR_PIN,HIGH);
  while(!digitalRead(LIM_BLUE)) {
    
  }
  delay(100);
  digitalWrite(MOTOR_PIN,LOW);
}
