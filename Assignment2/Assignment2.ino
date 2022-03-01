#include<stdio.h>
#include<mbed.h>

#define sigB 50 //
// defining the corresponding pins to the ESP32 board 
#define ledPin1 15
#define ledPin2 21
#define button1 22
#define button2 23
#define slot 10000
#define t_signal 1
#define NOS_TASKS 4

Ticker 

// initialising the button state variables to low (default)
int button1State = 0;
int button2State = 0;

//int ticks = 0;
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.begin(250000);
  delay(100);
}

/*
int main(void) {
  for(;;) {
    //wait
    //task1
    //task1();
    //wait
    //task2
    //....
  }
}
*/

/*
//using loop() as this function uses for(;;)
void loop() {
  // put your main code here, to run repeatedly:
  ticks++;
  task1(); // watchdog waveform
  //Serial.print("1");
  
  ticks++;
  if((ticks % 5) < NOS_TASKS) {
    task2();
    Serial.print("2");
    //ticks = ticks - (NOS_TASKS * 10);    
  }
  ticks++;
  if((ticks % 25) < NOS_TASKS) {
    task3();
    Serial.print("3");
    //ticks = ticks - (NOS_TASKS * 25);
  }
  Serial.println();
}
*/
void loop() {
  counter++;
  task1(); // every cycle
  //Serial.println("1");
  
  if ((counter % 200) == 0){
    task2(); // every 200ms (5Hz)
    Serial.println("2");
  }
  
  if ((counter % 1000) == 0) {
    task3(); // every 1000ms (1Hz)
  }

  //Serial.println();
}

void wait_signal(){
  delay(1);
}

void task1() {
  digitalWrite(ledPin1, HIGH);
  delayMicroseconds(50);
  digitalWrite(ledPin1, LOW);
  return;
}

void task2() {
  button1State = digitalRead(button1);
  //delay(150);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin2, LOW);
  return;
}

void task3() {
  String freq = "frequency is blah";
  Serial.println(freq);
}
