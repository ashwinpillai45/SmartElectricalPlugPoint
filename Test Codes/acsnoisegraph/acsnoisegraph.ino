const int currentPin = A0;
 double amps = 0;
 int count = 1;
 double maxValue=0;
 double minValue=0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  amps = (512-analogRead(currentPin)) * 50.00 /1023;
  //Serial.print("Raw Sensor Value = " );
  //Serial.print(adcValue);
  //maxValue=max(maxValue,amps);
  //minValue=min(minValue,amps); 
  //double noise = maxValue-0;
  Serial.print("Count : ");
  Serial.print(count);
  //Serial.print("\t Noise : ");
  //Serial.print(noise,3);
  Serial.print("\t Amps : ");
  Serial.print(amps,3);
  Serial.println();
  count++;
  if(Serial.read() != -1) {maxValue=amps;minValue=amps;}
  //if(currentValue >= -0.222 && currentValue <= 0)
  //digitalWrite(13,LOW);
  //else
 //digitalWrite(13,HIGH);
 // delay(2000);
 delay(100);
}
