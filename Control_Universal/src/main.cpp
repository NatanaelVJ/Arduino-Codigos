#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//VARIABLES ENCODER
int CLK = A0; //PIN A a4
int DT = A1; //PIN B  a5
int BTN = A2; // SW   a6       
int contador = 0;
int estadoActual;
int estadoAntes;
int estado_BTN_Antes;
int aux = 0;
String opciones[] = {"INICIO","SIGNAL MAPPING","ROBOT HUMANOID","BIPEDO"}; //EL ARREGLO PARTE DESDE 0, O=1A
int max_opciones =sizeof(opciones)/sizeof(opciones[0]);

//VARIABLES NRF24
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;     //Remember that this code is the same as in the transmitter
RF24 radio(59, 48);  //CSN and CE pins


//VARIABLES LCD
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

Serial.begin(9600);
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
lcd.print("Bienvenido...");


//VARIABLES ENCODER
pinMode(CLK,INPUT);
pinMode(DT,INPUT);
pinMode(BTN,INPUT_PULLUP);

estadoAntes = digitalRead(CLK);
estado_BTN_Antes = digitalRead(BTN);

}
void mostrar_menu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MENU: ");
  lcd.setCursor(0,1);  
  lcd.print(opciones[contador]);
  } 



void signalMap(){
   
   lcd.clear();
   lcd.setCursor(0,0);    
   lcd.print("MAPEO");
    //Potenciometros
   if(sent_data.ch1>0){
   lcd.clear();
   lcd.setCursor(0,0);    
   lcd.print(" Potenciometros");
   lcd.setCursor(0,1);
   lcd.print("Po1:");
   lcd.print(sent_data.ch1);
   }
   if(sent_data.ch2>0) {
   lcd.setCursor(8,1);
   lcd.print("Po2:");
   lcd.print(sent_data.ch2);
   }
   
   //JOYSTICK DERECHO
   if(sent_data.ch3>128){ 
   lcd.clear();
   lcd.print("Joystick RIGHT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 RIGHT:");
   lcd.print(sent_data.ch3);
   
   }
   else if(sent_data.ch3<126){ 
   lcd.clear();
   lcd.print("Joystick RIGHT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 LEFT:");
   lcd.print(sent_data.ch3);
   
   }
   if(sent_data.ch4<126){ 
   lcd.clear();
   lcd.print("Joystick RIGHT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 UP:");
   lcd.print(sent_data.ch4);
   
   }
   else if(sent_data.ch4>128){ 
   lcd.clear();
   lcd.print("Joystick RIGHT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 DOWN:");
   lcd.print(sent_data.ch4);
   
   }
   
   //JOYSTICK IZQUIERDO
   if(sent_data.ch5>127){ 
   lcd.clear();
   lcd.print("Joystick LEFT");
   lcd.setCursor(0,1);
   lcd.print("SP=125 RIGHT:");
   lcd.print(sent_data.ch5);
   
   }
   else if(sent_data.ch5<124){ 
   lcd.clear();
   lcd.print("Joystick LEFT");
   lcd.setCursor(0,1);
   lcd.print("SP=125 LEFT:");
   lcd.print(sent_data.ch5);
   
   }
   if(sent_data.ch6<125){ 
   lcd.clear();
   lcd.print("Joystick LEFT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 UP:");
   lcd.print(sent_data.ch6);
   
   }
   else if(sent_data.ch6>128){ 
   lcd.clear();
   lcd.print("Joystick LEFT");
   lcd.setCursor(0,1);
   lcd.print("SP=127 DOWN:");
   lcd.print(sent_data.ch6);
  
   }
   if(sent_data.ch7==1){
   lcd.setCursor(0,1);
   lcd.print("SWITCH HIGH:");
   lcd.print(sent_data.ch7);
   }
   
   
   }
  
  
void encoder(){
  estadoActual = digitalRead(CLK);
  if(estadoActual!=estadoAntes){
    if(digitalRead(DT)!=estadoActual){
    contador++;
  }
    else{
    contador--;  
    }
    if(contador<0) contador=max_opciones-1;
    if(contador>max_opciones-1) contador=0;
    mostrar_menu();
}
/*delay(250);
estadoAntes=estadoActual;
*/
} 
 
void run_opciones(){
  if(contador==0){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INICIO");
  }
  if(contador==1){
    signalMap();
  }

  }
void loop() {
  
  
  int estado_BTN = digitalRead(BTN);
  encoder();
  

  if(estado_BTN_Antes == HIGH && estado_BTN == LOW ){
    run_opciones();
  }

  
  estado_BTN_Antes == estado_BTN;
 
   
  
  
  


  sent_data.ch1 = map(analogRead(A10), 0, 1024, 0, 255);
  sent_data.ch2 = map(analogRead(A11), 0, 1024, 0, 255);
  sent_data.ch3 = map(analogRead(A8), 0, 1024, 0, 255);
  sent_data.ch4 = map(analogRead(A9), 0, 1024, 0, 255);
  sent_data.ch5 = map(analogRead(A15), 0, 1024, 0, 255);
  sent_data.ch6 = map(analogRead(A13), 0, 1024, 0, 255);
  sent_data.ch7 = digitalRead(8);
  
  radio.write(&sent_data, sizeof(Data_to_be_sent));


   
   
  }

   
  
   
   

 
   

  
   
   
   
   


