#include <Ticker.h>
#include <stdio.h>
#include <math.h>

//define the constants for the cyclic executive
#define SIG_B 50 
#define LEDPIN1 15
#define LEDPIN2 21
#define BUTTON1 22
#define BUTTON2 23
#define TASK_EXE_PIN 17
#define PIN_4 4
#define AN_PIN_0 0
#define CYCLE_LENGTH 8
#define NOS_TASKS 4
#define MAX_RANGE 4095

//define macro for computing indices of base 10
#define POW_BASE10(i) pow(10,i)

//initialise variables to be used in cyclic executive
int button1State = 0;
int button2State = 0;
int counter_main = 0;
int counter_task4_5 = 0;
int analog_in = 0;
int average_an_data = 0;
int average_an;
int wave_freq = 0;
int pinData = 0;
int i = 0;
int sum = 0;
int unfiltered_an_data = 0;
int task7_data = 0;
int error_code_data = 0;
int error_code = 0;
int data_array[] = {0,0,0,0};

//creating the ticker
Ticker Cycle;

void setup() {
  //put your setup code here, to run once including all i/o pins and serial monitor
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(AN_PIN_0, INPUT);
  pinMode(PIN_4, INPUT);
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(TASK_EXE_PIN, OUTPUT);
  Serial.begin(115200);
  //setup ticker to run the function cycleF at a period of CYCLE_LENGTH 
  Cycle.attach_ms(CYCLE_LENGTH, cycleF);
  delay(100);
}

void loop() {}

//cyclic executive function
void cycleF() {
  //increment the main counter every time the function runs
  counter_main ++;
  
  //execute watchdog every cycle  
  task1();

  //execute task2() every 200ms (5Hz)
  if ((counter_main % 25) == 0){
    task2();
  }

  //execute task3() every 1s (1Hz)
  if ((counter_main % 125) == 0) {
    task3(); // every 1000ms (1Hz)
  }

  //execute task4() and task5() every 42ms (~24Hz rounded from 41.667)
  if ((counter_main % 5) == 0) {
    //store the returned integer from task 4 as the unfiltered analog data
    digitalWrite(TASK_EXE_PIN, HIGH);
    unfiltered_an_data = task4();
    digitalWrite(TASK_EXE_PIN, LOW);
    //run task5() with the data from task4()
    average_an_data = task5(unfiltered_an_data);
    
  }

  //execute task6() 80 times every 8ms (10Hz) instead of 1000 times every 100ms
  for (i = 0; i < 80; i++) {
    task6();
  }

  //execute task7() and task8() every 333ms (~3Hz rounded from 333.333)
  if ((counter_main % 42) == 0) {
    //store the returned integer from task7() as the error code data
    error_code_data = task7(average_an_data);
    //run task8() with the data from task7()
    task8(error_code_data);  
  }

  //execute task9() eveyr 5s (0.2Hz)
  if ((counter_main % 625) == 0) {
    task9(button1State, wave_freq, average_an_data);
  }
}

//watchdog waveform
void task1() {
  digitalWrite(LEDPIN1, HIGH);
  delayMicroseconds(SIG_B);
  digitalWrite(LEDPIN1, LOW);
}

//monitor digital input (0 = LOW, 1 = HIGH)
void task2() {
  button1State = digitalRead(BUTTON1);
}

//measure frequency of 3.3V square wave signal
void task3() {
  //use pulseIn function to measure the period of the wave 
  pinData = pulseIn(PIN_4, LOW);
  
  //calculate the frequency by using f = (1/T)*2
  wave_freq = 1/(2*pinData*POW_BASE10(-6));
}

//read the analog input connected to a signal generator
int task4() {
  analog_in = analogRead(AN_PIN_0);
  //Serial.println(analog_in);
  
  return analog_in;
}

//average the last 4 readings of task4()
int task5(int data) {
  //switch case to decide what position in the array the data should be added
  switch(counter_task4_5) {
    case 0:
      data_array[counter_task4_5] = data;
      break;
    case 1:
      data_array[counter_task4_5] = data;
      break;
    case 2:
      data_array[counter_task4_5] = data;
      break;
    case 3:
      data_array[counter_task4_5] = data;
      break;
  }
  
  //increment the new counter 
  counter_task4_5++;
  
  //if the new counter reaches 4 then reset to start over which then updates the oldest element of the array (FILO)
  if(counter_task4_5 == 4) {
    counter_task4_5 = 0;
  }
  
  //sum is set to 0 to ensure no ambiguity
  sum = 0;
  
  //iterate through the array and store the sum of all values in sum
  for(i = 0; i < 4; i++) {
    sum += data_array[i];
  }
  
  //calculate the avergage value of the last 4 readings
  average_an = sum/4;
  //Serial.println(average_an);
  
  //return the average analog value
  return average_an;
}

//do nothing tasks set to run 80 times per 8ms rather than 1000 times every 100 ms
void task6() {
  __asm__ __volatile__ ("nop"); 
}

//perform the check on the average analog value versus the max_range/2
int task7(int data) {
  //if the average analog value is greater than half then chnage error code to HIGH
  if (data > (MAX_RANGE/2)) {
    error_code = 1;
  }
  //otherwise stay LOW
  else {
    error_code = 0;
  }
  
  //return the state of the error_code
  return error_code;
}

//visualise the errod_code state on an LED
void task8(int data) {
  //if the error_code is HIGH then set the red LED to HIGH
  if(data == 1) {
    digitalWrite(LEDPIN2, HIGH);
  }
  //otherwise stay LOW
  else {
    digitalWrite(LEDPIN2, LOW);
  } 
}

//log data every 5s in CSV format
void task9(int data1, int data2, int data3) {
  //print the state of button 1
  Serial.print(data1);
  Serial.print(", ");
  
  //print the frequency of the 3.3v square wave signal
  Serial.print(data2); 
  Serial.print(", ");
  
  //print the filtered analog input
  Serial.println(data3);
}
