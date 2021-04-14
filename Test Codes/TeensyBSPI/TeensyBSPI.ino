#include <Adafruit_BluefruitLE_SPI.h>

#define BLUEFRUIT_SPI_CS 10
#define BLUEFRUIT_SPI_IRQ 15
#define BLUEFRUIT_SPI_RST 14
#define BLUEFRUIT_SPI_SCK 13
#define BLUEFRUIT_SPI_MISO 12
#define BLUEFRUIT_SPI_MOSI 11

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK,BLUEFRUIT_SPI_MISO,BLUEFRUIT_SPI_MOSI,BLUEFRUIT_SPI_CS,BLUEFRUIT_SPI_IRQ,BLUEFRUIT_SPI_RST);

int32_t acsServiceID;
int32_t getStatusID;
int32_t setStatusID;
int32_t currentID;  

void error(const __FlashStringHelper*err)
{
  Serial.println(err);
  while(1);
}

void updateIntCharacteristic(String characteristic, String charID)
{
  String msg = String(characteristic);
  ble.print(F("AT+GATTCHAR="));
  ble.print(charID);
  ble.print(F(","));
  ble.println(msg);

  if ( !ble.waitForOK() ) Serial.println(F("Failed to get response!"));
}
  
void emitData(String statusP, String current){
  updateIntCharacteristic(statusP,getStatusID);
  updateIntCharacteristic(current,currentID);
}
  
void setup() {
  // put your setup code here, to run once:
  bool disp;
Serial.begin(115200);
boolean success;
if(!ble.begin(true)) error(F("Couldn't find the bluetooth module")); else Serial.println("Bluetooth module found");
if (!ble.factoryReset(false)) error(F("Couldn't factory reset")); else Serial.println("Device Resetted");
ble.echo(false);
ble.info();
if(!ble.sendCommandCheckOK(F("AT+GAPDEVNAME=TEENSY1"))) error(F("Could not set device name"));
success=ble.sendCommandWithIntReply(F("AT+GATTADDSERVICE=UUID128=88-dd-97-3c-f2-c1-45-e2-8e-32-95-bb-e7-cb-be-9e"), &acsServiceID);
if(!success) error(F("Cannot Add Service"));
success=ble.sendCommandWithIntReply(F("AT+GATTADDCHAR=UUID128=88-dd-97-3c-f4-c1-45-e2-8e-32-95-bb-e7-cb-be-9e, PROPERTIES=0x02, MIN_LEN=1, MAX_LEN=20, VALUE=0, DATATYPE=1"), &getStatusID);
if(!success) error(F("Cannot Add Characteristic"));
success=ble.sendCommandWithIntReply(F("AT+GATTADDCHAR=UUID128=88-dd-97-3c-f5-c1-45-e2-8e-32-95-bb-e7-cb-be-9e, PROPERTIES=0x08, MIN_LEN=1, MAX_LEN=20, VALUE=1, DATATYPE=1"), &setStatusID);
if(!success) error(F("Cannot Add Characteristic"));
success=ble.sendCommandWithIntReply(F("AT+GATTADDCHAR=UUID128=88-dd-97-3c-f6-c1-45-e2-8e-32-95-bb-e7-cb-be-9e, PROPERTIES=0x02, MIN_LEN=1, MAX_LEN=20, VALUE=0, DATATYPE=1"), &currentID);
if(!success) error(F("Cannot Add Characteristic"));
}

void loop() {
  // put your main code here, to run repeatedly:
int statusP=1;
float current=random(1,1000);
emitData(String(statusP),String(current));
}
