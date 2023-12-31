
// this header is needed for Bluetooth Serial -> works ONLY on ESP32
#include "BluetoothSerial.h" 
#include <ESP32Servo.h>

Servo vertical;
Servo horizontal;
BluetoothSerial ESP_BT; 

int enA = 13;
int in1 = 12;
int in2 = 14;  

int enB = 4;
int in3 = 2;
int in4 = 15;

int pump = 18;
int servoPinv = 21;
int servoPinh = 23;
// Parameters for Bluetooth interface
int incoming;

void setup() {
  Serial.begin(19200);
  ESP_BT.begin("ESP32_Control"); //Name of your Bluetooth interface -> will show up on your phone
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  vertical.setPeriodHertz(50);
  horizontal.setPeriodHertz(50);
  vertical.attach(servoPinv, 500, 2400);
  horizontal.attach(servoPinh, 500, 2400);
  pinMode (enA, OUTPUT);
  pinMode (in1, OUTPUT);
  pinMode (in2, OUTPUT);
  pinMode (enB, OUTPUT);
  pinMode (in3, OUTPUT);
  pinMode (in4, OUTPUT);
  pinMode (pump, OUTPUT);
}

void loop() {
  
  // -------------------- Receive Bluetooth signal ----------------------
  if (ESP_BT.available()) 
  {
    incoming = ESP_BT.read(); //Read what we receive 

    if (incoming >= 70 ) {
      if (incoming <= 160) {
        vertical.write(incoming - 70);
      } else {
        horizontal.write(incoming - 170);
      } 
    } else {

    // separate button ID from button value -> button ID is 10, 20, 30, etc, value is 1 or 0
    int button = floor(incoming / 10);
    int value = incoming % 10;
    switch (button) {
      case 1:  
        Serial.print("Backward:"); Serial.println(value);
        digitalWrite(in1, 0);
        digitalWrite(in2, 1);
        digitalWrite(in3, 0);
        digitalWrite(in4, 1);
        break;
      case 2:  
        Serial.print("Forward:"); Serial.println(value);
        digitalWrite(in1, 1);
        digitalWrite(in2, 0);
        digitalWrite(in3, 1);
        digitalWrite(in4, 0);
        break;
      case 3:  
        Serial.print("Still:"); Serial.println(value);
        digitalWrite(in1, 1);
        digitalWrite(in2, 1);
        digitalWrite(in3, 1);
        digitalWrite(in4, 1);
        break;
      case 4:  
        Serial.print("Left:"); Serial.println(value);
        digitalWrite(in1, 0);
        digitalWrite(in2, 1);
        digitalWrite(in3, 1);
        digitalWrite(in4, 0);
        break;
      case 5:  
        Serial.print("Right:"); Serial.println(value);
        digitalWrite(in1, 1);
        digitalWrite(in2, 0);
        digitalWrite(in3, 0);
        digitalWrite(in4, 1);
        break;
      case 6:  
        Serial.print("Pump:"); Serial.println(value);
        if(value == 1){
        digitalWrite(pump, HIGH);
        digitalWrite(in1, 1);
        digitalWrite(in2, 1);
        digitalWrite(in3, 1);
        digitalWrite(in4, 1);
        }
        if(value == 0){
        digitalWrite(pump, LOW);
        digitalWrite(in1, 1);
        digitalWrite(in2, 1);
        digitalWrite(in3, 1);
        digitalWrite(in4, 1);
        }
        Serial.print("Pump: done"); Serial.println(value);
        break;
      default:
        Serial.print(":(");
        digitalWrite(in1, 1);
        digitalWrite(in2, 1);
        digitalWrite(in3, 1);
        digitalWrite(in4, 1);
        break;
    }
  analogWrite(enA, 255); // Send PWM signal to motor A 
  analogWrite(enB, 255); // Send PWM signal to motor B
  }
  }
}
