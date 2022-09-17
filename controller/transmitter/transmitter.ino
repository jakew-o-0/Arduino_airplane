#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

//created variables
RF24 radio(7, 8);
const byte Adress[6] = "42069";
int count;
#define Joy1X A1
#define Joy1Y A2
#define Joy2X A3
#define Joy2Y A4
#define Btn1 1
#define Btn2 2
#define Btn3 3
#define LED1 4
#define LED2 5
#define LED3 6
#define LED4 9


//made data structure to hold the data
struct Data_Packet{
  byte Joy1XData;
  byte Joy1YData;
  byte Joy2XData;
  byte Joy2YData;
  byte Btn1Data;
  byte Btn2Data;
  byte CountData;
};
//made an instance of the struct
Data_Packet Data;

void setup() {
  //attached the variables to pins
  pinMode(Joy1X, INPUT);
  pinMode(Joy1Y, INPUT);
  pinMode(Joy2X, INPUT);
  pinMode(Joy2Y, INPUT);
  pinMode(Btn1, INPUT);
  pinMode(Btn2, INPUT);
  pinMode(Btn3, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  //setting up the radio
  radio.begin();
  radio.openWritingPipe(Adress);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

void loop() {
  //reading the joysticks, converting them to the size of a byte and saving it to the struct
  Data.Joy1XData = map(analogRead(Joy1X), 0, 1023, 0, 225);
  Data.Joy1YData = map(analogRead(Joy1Y), 0, 1023, 0, 225);
  Data.Joy2XData = map(analogRead(Joy2X), 0, 1023, 0, 225);
  Data.Joy2YData = map(analogRead(Joy2Y), 0, 1023, 0, 225);
  //reading the digital inputs
  Data.Btn1Data = map(digitalRead(Btn1), 1000, 2000, 0, 225);
  Data.Btn2Data = map(digitalRead(Btn2), 1000, 2000, 0, 225);

    if(digitalRead(Btn3) == HIGH){
      count++;
      if(count > 3){
        count = 0;
      }
      if(count == 0){
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
      }
      if(count == 1){
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
      }
      if(count == 2){
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, LOW);
      }
      if(count == 3){
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, HIGH);
      }
    }
  Data.CountData = count;
  
  //sending the data to the reciever
  radio.write(&Data, sizeof(Data_Packet));
}
