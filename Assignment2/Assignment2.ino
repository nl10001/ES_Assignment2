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
#define POW(i) pow(10,i)

// initialising the button state variables to low (default)
int button1State = 0;
int button2State = 0;

//int ticks = 0;
int counter_main = 0;
int counter_task4_5 = 0;
int analog_in = 0;
int average_an;
int wave_freq = 0;
int pinData = 0;
int i = 0;
int data4 = 0;
int sum = 0;
int task7_data = 0;
int task8_data = 0;
int error_code = 0;

int data_array[] = {0,0,0,0};

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
  counter_main++;
  //digitalWrite(ledPin2, HIGH);
  task1(); // every cycle
  //digitalWrite(ledPin2, LOW);
  //Serial.println("1");
  
  if ((counter_main % 200) == 0){
    //digitalWrite(ledPin2, HIGH);
    task2(); // every 200ms (5Hz)
    //digitalWrite(ledPin2, LOW);
    //Serial.println("2");
  }
  
  if ((counter_main % 1000) == 0) {
    task3(); // every 1000ms (1Hz)
  }
  
  if ((counter_main % 42) == 0) {
    //digitalWrite(ledPin2, HIGH);
    data4 = task4();
    //digitalWrite(ledPin2, LOW);
  }
  
  if ((counter_main % 42) == 21) {
    //digitalWrite(ledPin2, HIGH);
    task7_data = task5(data4);
    //digitalWrite(ledPin2, LOW);
  }
/*
  if ((counter_main % 21) == 0) {
    counter_task5_6++;
    if ((counter_task % 2) == 0) {
      
    }
  }
 */ 
  for(i = 0; i < 10; i++) {
    task6();
  }

  if((counter_main % 333) == 0) {
    task8_data = task7(task7_data);  
  }

  if((counter_main % 333) == 167) {
    task8(task8_data);
  }

  if((counter_main % 5000) == 0) {
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
  wave_freq = 1/(2*pinData*POW(-6));
  //Serial.println(wave_freq);
}

int task4() {
  analog_in = analogRead(AN_PIN_0);
  //Serial.println(analog_in);
  return analog_in;
}

int task5(int data4) {
  
  //Serial.println(counter_task4_5);
  switch(counter_task4_5) {
    case 0:
      data_array[counter_task4_5] = data4;
      break;
    case 1:
      data_array[counter_task4_5] = data4;
      break;
    case 2:
      data_array[counter_task4_5] = data4;
      break;
    case 3:
      data_array[counter_task4_5] = data4;
      break;
  }
  counter_task4_5++;
  /*
  for(i = 0; i < 4; i++) {
    Serial.print(data_array[i]);
    Serial.print("\t");
    
  }
  Serial.println();
  */
  if(counter_task4_5 == 4) {
    counter_task4_5 = 0;
  }
  sum = 0;
  for(i = 0; i < 4; i++) {
    sum += data_array[i];
  }
  average_an = sum/4;
  Serial.println(average_an);
  return average_an;
}

//do nothing tasks set to run 10 times per ms rather than 1000 times every 100 ms
void task6() {
  __asm__ __volatile__ ("nop");
  //Serial.println("test");
  //digitalWrite(ledPin2, HIGH);
  //delayMicroseconds(100);
  //digitalWrite(ledPin2, LOW);
  
}

int task7(int data) {
  if (data > (MAX_VOLTAGE/2)) {
    error_code = 1;
  }
  else {
    error_code = 0;
  }
  return error_code;
}

void task8(int data) {
  if(data == 1) {
    digitalWrite(ledPin2, HIGH);
  }
  else {
    digitalWrite(ledPin2, LOW);
  }
  
}

//log data every 5s in CSV format
void task9() {
  Serial.print(button1State);
  Serial.print(",");
  Serial.println(wave_freq);
}
