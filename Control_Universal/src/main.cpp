#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CLK A4 //ENCODER A
#define DT A5 //ENCODER A
#define SW A6 //ENCODER A
int contador = 0;
int estadoActual;
int estadoAntes;



const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;     //Remember that this code is the same as in the transmitter
RF24 radio(59, 48);  //CSN and CE pins

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

struct Data_to_be_sent {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
};

Data_to_be_sent sent_data;

void setup() {

radio.begin();
radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);  
radio.openWritingPipe(my_radio_pipe); 

sent_data.ch1 = 0; //pot1
sent_data.ch2 = 0; //pot2
sent_data.ch3 = 127; //X_RIGHT
sent_data.ch4 = 127; //Y_RIGHT
sent_data.ch5 = 127; //X_LEFT
sent_data.ch6 = 127; //Y_LEFT
sent_data.ch7 = 0; //SWITCH_A

lcd.begin(16, 2);
lcd.print("iniciando...");
Serial.begin(9600);
delay(500);

//VARIABLES ENCODER
pinMode(CLK,INPUT);
pinMode(DT,INPUT);
pinMode(SW,INPUT);

estadoAntes = digitalRead(CLK);

}

void encoder(){
  estadoActual = digitalRead(CLK);
  if(estadoActual != estadoAntes){
    if(digitalRead(DT)!=estadoActual){
      contador ++;
    }
    else{
      contador --;
    }
    lcd.clear();
    lcd.print("Contador: ");
    lcd.setCursor(0,1);
    lcd.print(contador);
    delay(200);
  }
  estadoAntes = estadoAntes;
}

void loop() {

  encoder();
  if(0<contador && contador <2 && SW==1){
    lcd.clear();
    lcd.print("MENU 1");

  }

  sent_data.ch1 = map(analogRead(A10), 0, 1024, 0, 255);
  sent_data.ch2 = map(analogRead(A11), 0, 1024, 0, 255);
  sent_data.ch3 = map(analogRead(A8), 0, 1024, 0, 255);
  sent_data.ch4 = map(analogRead(A9), 0, 1024, 0, 255);
  sent_data.ch5 = map(analogRead(A15), 0, 1024, 0, 255);
  sent_data.ch6 = map(analogRead(A13), 0, 1024, 0, 255);
  sent_data.ch7 = digitalRead(8);
  
  radio.write(&sent_data, sizeof(Data_to_be_sent));


   
   //Potenciometros
   if(sent_data.ch1>0 || sent_data.ch2>2){
   lcd.clear();    
   lcd.print(" Potenciometros");
   if(sent_data.ch1>2){
   lcd.setCursor(0,1);
   lcd.print("Po1:");
   lcd.print(sent_data.ch1);
   
   }
   if(sent_data.ch2>0){ 
   lcd.setCursor(8,1);
   lcd.print("Po2:");
   lcd.print(sent_data.ch2);
   }

   delay(50);
   }

   
   
   if(sent_data.ch3>128){ 
   lcd.clear();
   lcd.print("Joystick RIGHT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 RIGHT:");
   lcd.print(sent_data.ch3);
   delay(50);
   }
   if(sent_data.ch3<126){ 
   lcd.clear();
   lcd.print("Joystick RIGHT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 LEFT:");
   lcd.print(sent_data.ch3);
   delay(50);
   }
   if(sent_data.ch4<126){ 
   lcd.clear();
   lcd.print("Joystick RIGHT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 UP:");
   lcd.print(sent_data.ch4);
   delay(50);
   }
   if(sent_data.ch4>128){ 
   lcd.clear();
   lcd.print("Joystick RIGHT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 DOWN:");
   lcd.print(sent_data.ch4);
   delay(50);
   }

   //JOYSTICK IZQUIERDO
   if(sent_data.ch5>127){ 
   lcd.clear();
   lcd.print("Joystick LEFT");
   lcd.setCursor(0,1);
   lcd.print("SP=125 RIGHT:");
   lcd.print(sent_data.ch5);
   delay(50);
   }
   if(sent_data.ch5<124){ 
   lcd.clear();
   lcd.print("Joystick LEFT");
   lcd.setCursor(0,1);
   lcd.print("SP=125 LEFT:");
   lcd.print(sent_data.ch5);
   delay(50);
   }
   if(sent_data.ch6<125){ 
   lcd.clear();
   lcd.print("Joystick LEFT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 UP:");
   lcd.print(sent_data.ch6);
   delay(50);
   }
   if(sent_data.ch6>128){ 
   lcd.clear();
   lcd.print("Joystick LEFT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 DOWN:");
   lcd.print(sent_data.ch6);
   delay(50);
   }
  if(sent_data.ch7==1){
   lcd.setCursor(0,1);
   lcd.print("SWITCH HIGH:");
   lcd.print(sent_data.ch7);
   delay(50); 
  }
   /*else{
   lcd.clear();
   }*/
  
   
  
   
   

 
   

  
   
   
   
   

}
