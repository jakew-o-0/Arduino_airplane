#include <SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<Servo.h>

//creating variables
RF24 radio(7, 8);
const byte Adress[6] = "42069";
int ser1Data = 0;
int ser2Data = 0;
int ser3Data = 0;
int ser4Data = 0;
int Btn1OutDat = 0;
int Btn2OutDat = 0;
Servo ser1;//throttle
Servo ser2;//roll
Servo ser3;//elevator
Servo ser4;//rudder
Servo ser5;// extra for car control
#define BtnOut1 9
#define BtnOut2 6

//recived data
struct Data_Packet{
  byte Joy1XData;
  byte Joy1YData;
  byte Joy2XData;
  byte Joy2YData;
  byte Btn1Data;
  byte Btn2Data;
  byte CountData;
};
Data_Packet Data;


void Four_Channel_Controls(){
  ser1.write(ser1Data);
  ser2.write(ser2Data);
  ser3.write(ser3Data);
  ser4.write(ser4Data);
}
void Elevon_Controls(){
  ser1.write(ser1Data);
  ser4.write(ser4Data);
  if(ser2Data == 90 && ser3Data != 90){
    ser2.write(ser3Data);
    ser3.write(ser3Data);
  }
  else{
    ser2.write(ser2Data);
    ser3.write(180 - ser2Data);
  }
}
void Car_Controls(){
  ser1.write(ser1Data);
  ser2.write(ser1Data);
  ser3.write(ser1Data);
  ser4.write(ser1Data);
  ser5.write(ser4Data);
}
void differential_Car_Controls(){
  ser1.write(ser1Data);
  ser2.write(ser3Data);
  ser3.write(ser1Data);
  ser4.write(ser3Data);
}


void setup() {
  //attaching servos and other controler outputs
  ser1.attach(1);
  ser2.attach(2);
  ser3.attach(3);
  ser4.attach(4);
  ser5.attach(5);
  pinMode(BtnOut1, OUTPUT);
  pinMode(BtnOut2, OUTPUT);

  //setting up radio
  radio.begin();
  radio.openWritingPipe(Adress);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() {
  if(radio.available()){
    radio.read(&Data, sizeof(Data_Packet));
  }
  //maping the transmited data to values of a servo
  ser1Data = map(Data.Joy1YData, 0, 225, 0, 180);
  ser2Data = map(Data.Joy1XData, 0, 225, 0, 180);
  ser3Data = map(Data.Joy2YData, 0, 225, 0, 180);
  ser4Data = map(Data.Joy2XData, 0, 225, 0, 180);
  //maping the transmited data to either a 1 or a 0
  Btn1OutDat = map(Data.Btn1Data, 0, 225, 0, 1);
  Btn2OutDat = map(Data.Btn2Data, 0, 225, 0, 1);

  //using the data stored in data.CountData it will change the control method used
  if(Data.CountData == 0){
    Four_Channel_Controls();
  }
  else if(Data.CountData == 1){
    Elevon_Controls();
  }
  else if(Data.CountData == 2){
    Car_Controls();
  }
  else{
    Differential_Car_Controls();
  }

  if(Btn1OutDat > 0){
    BtnOut1.digitalWrite(HIGH);
  }
  else if(Btn2OutDat > 0){
    BtnOut2.digitalWrite(HIGH);
  }
  else{
    BtnOut1.digitalWrite(LOW);
    BtnOut2.digitalWrite(LOW);
  }
}
