#include<stdio.h>

#define sigB 50 //
// defining the corresponding pins to the ESP32 board 
#define ledPin1 15
#define ledPin2 21
#define button1 22
#define button2 23

void setup() {
  // put your setup code here, to run once:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
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

//using loop() as this function uses for(;;)
void loop() {
  // put your main code here, to run repeatedly:
  watchdog();
  task1();
}

void watchdog() {
  digitalWrite(ledPin2, HIGH);
  delay(5000);
  digitalWrite(ledPin2, LOW);
  delay(1000);
}

void task1() {
  button1State = digitalRead(button1);
}

void task2() {
  
}
