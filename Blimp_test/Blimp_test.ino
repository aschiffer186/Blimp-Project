#include <Servo.h>

const int ch3Pin = 10; //channel 2
const int ch1Pin = 7; //channel 1
const int servoPin = 9; //sero 1
const int servoPin2 = 8; //servo 2l
const int motorPin1 = A7;
const int ch4Pin = 2;
int ch3Val = 0;
int ch1Val = 0;
int ch4Val = 0;
int servoValue = 90;
int servoValue2 = 90;
Servo myServo;
Servo myServo2;
const double range = 1909.0-1054.0;

struct Channel {
  double min_value;
  double max_value;
};
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(ch3Pin, INPUT);
  pinMode(ch1Pin, INPUT);
  pinMode(servoPin, INPUT);
  pinMode(servoPin2, INPUT);
  pinMode(ch4Pin, INPUT);
  pinMode(motorPin1, OUTPUT);
  myServo2.attach(servoPin2);
  myServo2.write(servoValue2);
  myServo.attach(servoPin);
  myServo.write(servoValue);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  ch3Val = pulseIn(ch3Pin, HIGH);
  ch1Val = pulseIn(ch1Pin, HIGH);
  ch4Val = pulseIn(ch4Pin, HIGH);
  ch4Val = ch4Val - (1874-1078);
  double frac = ch4Val/(1874.0-1078.0);
  int val3 = (int) frac * 255;
  //Serial.print("max val = ");
  //Serial.print(HIGH);
  //Serial.println('\n');
  //Serial.print("ch3Val = ");
  //Serial.print(ch3Val);
  //Serial.print('\n');
 
  (ch3Val > 1500) ? digitalWrite(13, HIGH) : digitalWrite(13, LOW);
  int val = convert(ch3Val - range, range);
  int val2 = convert(ch1Val - range, range);
  Serial.print(val);
  Serial.print(val2);
  myServo.writeMicroseconds(val);
  myServo2.writeMicroseconds(val2);
  analogWrite(motorPin1, val3);
  //Ch3 (Left stick, up and down)
  //Min val: 876 (w/ trim down)
  //Min val: 1258 (w/ tirm up)
  //Min val: 1068 (w/ trim middle)
  //Middle val: 1197 (w/ trim down)
  //Middle val: 1456 (w/ trim middle)
  //Middle val: 1576 (w/ trim up)
  //Max val: 1715 (w/ trim down)
  //Max val: 2093 (w/ trim up)
  //Max val: 1904 (w/ trim middle)
  //Ch2
  //Min val: 871 (w/ trim down)
  //Min val: 1237 (w/ trim up)
  //Min val: 1054 (w/ middle trim)
  //Middle val: 1491 (w/ trim middle)
  //Max val: 1719 (w/ trim down)
  //Max val: 1909 (w/ middle trim)
  //Max val: 2093 (w/ trim up)
  //Ch 1
  //Min val (all the way left): 1098
  //Max val (all the way right): 1878
  //Middle val: 1471
  //Ch 4
  //Min Val (all the way left): 1078 (w/ middle trim)
  //Middle Val: 1481 (w/ middle trim)
  //Max val: 1874 (w/ middle trim)
  delay(750);
}
int convert(int a, double maxVal) {
  Serial.print("a = ");
  Serial.println(a);
  double frac = (double) a/maxVal;
  Serial.print("frac = ");
  Serial.print(frac);
  Serial.print('\n');
  return (int) (frac * 1000) + 1000;
}

int convertChannelValue(const Channel *channel, double input_value) {
  double range = (double) channel->min_value - channel->max_value;
  double frac = input_value/range;
  return (int) (frac * 1000) + 1000;
}


