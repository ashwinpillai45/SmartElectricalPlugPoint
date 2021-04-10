const int currentPin = A0;
double amps = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
for(int i=0;i<100;i++){
amps = amps + ((512-analogRead(currentPin)) * 50.00 /1023);
//delay(10);  
}
amps = amps / 100;
Serial.println(amps);
delay(100);
}
