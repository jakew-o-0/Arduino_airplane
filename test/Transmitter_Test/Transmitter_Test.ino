#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

RF24 radio(7, 8);
const byte Adress[42069];

int num = 0;

struct Data{
  byte sentData;
  };
Data data;
void setup() {
  radio.begin();
  radio.openWritingPipe(Adress);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  radio.read(&data, sizeof(Data));
  Serial.write(sentData);
}
