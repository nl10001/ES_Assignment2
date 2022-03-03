#include <Ticker.h>
#include <stdio.h>
#include <math.h>


#define sigB 50 //
// defining the corresponding pins to the ESP32 board 
#define ledPin1 15
#define ledPin2 21
#define button1 22
#define button2 23
#define PIN_4 4
#define AN_PIN_0 0
#define slot 10000
#define t_signal 1
#define NOS_TASKS 4
#define MAX_VOLTAGE 4095

// initialising the button state variables to low (default)
int button1State = 0;
int button2State = 0;

//int ticks = 0;
int counter = 0;
int analog_in = 0;
int wave_freq = 0;
int pinData = 0;
int i = 0;

Ticker Cycle;

void setup() {
  // put your setup code here, to run once:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(AN_PIN_0, INPUT);
  pinMode(PIN_4, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.begin(115200);
  Cycle.attach_ms(1, cycleF);
  delay(100);
}

//what does it mean by measure frequency in task 3 if it gives you the range already?
//for task 4 how do you measure the execution times with the spare digitial output?
//(tasks 4/5 & 7/8)can you put them in the same task function?
//have i done my if statement for task 4 correctly?

void loop() {}

//cyclic executive function
void cycleF() {
  counter++;
  //digitalWrite(ledPin2, HIGH);
  task1(); // every cycle
  //digitalWrite(ledPin2, LOW);
  //Serial.println("1");
  
  if ((counter % 200) == 0){
    //digitalWrite(ledPin2, HIGH);
    task2(); // every 200ms (5Hz)
    //digitalWrite(ledPin2, LOW);
    //Serial.println("2");
  }
  
  if ((counter % 1000) == 0) {
    task3(); // every 1000ms (1Hz)
  }
  
  if ((counter % 42) == 0) {
    //digitalWrite(ledPin2, HIGH);
    task4();
    //digitalWrite(ledPin2, LOW);
    
  }
  digitalWrite(ledPin2, HIGH);
  for(i = 0; i < 10; i++) {
    task6();
  }
  digitalWrite(ledPin2, LOW);
  if((counter % 5000) == 0) {
    task9();
  }
  
  //Serial.println("end");
}

void wait_signal(){
  delay(1);
}

//watchdog waveform
void task1() {
  digitalWrite(ledPin1, HIGH);
  delayMicroseconds(50);
  digitalWrite(ledPin1, LOW);
  return;
}

//monitor digital input (0 = LOW, 1 = HIGH)
void task2() {
  button1State = digitalRead(button1);
  //Serial.println(button1State);
  //delay(150);
  //digitalWrite(ledPin2, HIGH);
  //delayMicroseconds(100);
  //digitalWrite(ledPin2, LOW);
  return;
}

//Measure frequency of 3.3V square wave signal
void task3() {
  pinData = pulseIn(PIN_4, LOW);
  wave_freq = 1/(2*pinData*pow(10,-6));
  //Serial.println(wave_freq);
}

void task4() {
  analog_in = analogRead(AN_PIN_0);
  //Serial.println(analog_in);
  /*
  Serial.println("test");
  digitalWrite(ledPin2, HIGH);
  delayMicroseconds(100);
  digitalWrite(ledPin2, LOW);
  */
}

void task5() {
  
}

//do nothing tasks set to run 10 times per ms rather than 1000 times every 100 ms
void task6() {
  __asm__ __volatile__ ("nop");
  //Serial.println("test");
  //digitalWrite(ledPin2, HIGH);
  //delayMicroseconds(100);
  //digitalWrite(ledPin2, LOW);
  
}

void task7() {
  //if (average_analogue_in > (MAX_VOLTAGE/2))
}

void task8() {
  
}

//log data every 5s in CSV format
void task9() {
  Serial.print(button1State);
  Serial.print(",");
  Serial.println(wave_freq);
}
