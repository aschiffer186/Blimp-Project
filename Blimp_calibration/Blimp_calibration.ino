const int chPin = A2;
const int chPin2 = A1;
const int chPin3 = 8;
const int chPin4 = 7;
int chValue = 50;
int chValue2 = 50;
int chValue3 = 50;
int chValue4 = 50;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(chPin, INPUT);
  pinMode(chPin2, INPUT);
  pinMode(chPin3, INPUT);
  pinMode(chPin4, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  chValue = pulseIn(chPin, HIGH);
  chValue2 = pulseIn(chPin2, HIGH);
  chValue3 = pulseIn(chPin3, HIGH);
  chValue4 = pulseIn(chPin4, HIGH);
  //chValue2 = pulseIn(chPin2, HIGH);
  Serial.print("Ch1 value: ");
  Serial.print(chValue);
  Serial.print("\n");
  Serial.print("Ch2 value: ");
  Serial.print(chValue2);
  Serial.print("\n");
   Serial.print("Ch3 value: ");
  Serial.print(chValue3);
  Serial.print("\n");
   Serial.print("Ch4 value: ");
  Serial.print(chValue4);
  Serial.print("\n");
  /*
  Serial.print("Ch4 value: ");
  Serial.print(chValue2);
  */
  Serial.print("\n");
  delay(500);
  digitalWrite(13, LOW);
  delay(250);
  //Ch1: SMin, TMin - 936
  //Ch1: SMin, TMid - 1107
  //Ch1: SMax, TMid - 1888
  //Ch1: SMax, TMax - 2063
  
  //Ch2: SMin, TMin - 871
  //Ch2: SMin, TMid - 1077
  //Ch2: SMax, TMid - 1933
  //Ch2: SMax, TMax - 2092
  
  //Ch3: SMin, TMin - 877
  //Ch3: SMin, TMid - 1048
  //Ch3: SMax, TMid - 1912
  //Ch3: SMax, TMax - 2092
  
  //Ch4: SMin, TMin - 912
  //Ch4: SMin, TMid - 1067
  //Ch4: SMax, TMid - 1867
  //Ch4: SMax, TMax - 2062
  
}
