
#include <Servo.h>

const int buttonPin = 2;
//int buttonState = 0;
const int sensorPin = A7;
const int servoPin = 9;

Servo servo;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(90);
}

void loop() {
  int buttonState = 0;
  int sensorValue = 0;
  digitalWrite(13, HIGH);
  buttonState = digitalRead(buttonPin);
  //Serial.print("Button = ");
  //Serial.print(buttonState);
  sensorValue = analogRead(sensorPin);
  //Serial.print("Sensor = ");
  //Serial.print(sensorValue);
  //Serial.println("\n");
  servo.write((int) sensorValue * 180./1023.);
  (buttonState == HIGH) ? digitalWrite(3, HIGH) : digitalWrite(3, LOW);
  //digitalWrite(3, LOW);
  //delay(1000);
  digitalWrite(13, LOW);
  (buttonState == HIGH) ? digitalWrite(3, LOW) : digitalWrite(3, HIGH);
  //delay(500);
  //Max speed that LED could blink: 1/61,000 seconds
  //Fastest speed it can do something useful: 4.3 KHz
  
}
