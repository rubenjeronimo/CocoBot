#include <AFMotor.h>
#include <SoftwareSerial.h>

#define trigPin 14
#define echoPin 15

SoftwareSerial BT(17, 16); // RX, TX
char datoBT;


 long duracion, distancia;

int ledPin= 18;

int programa=0;
int cerca=10;
int medidaIzquierda=0;
int medidaDerecha=0;


int pulsador=14;


double pasos = 2048; //Numero de pasos para 1 rotacion total
 
int puertoMotor1 = 2; //1 para motor en M1/M2 y 2 para motor en M3/M4
int puertoMotor2 = 1;
int angulo = 90; //Angulo de rotacion de eje
 
double numero_de_pasos = 0; 
 
AF_Stepper stepper1(pasos, puertoMotor2); 
AF_Stepper stepper2(pasos, puertoMotor1); 
void ledParpadea(){
 for(int i=0;i<=5;i++){
 digitalWrite(ledPin,HIGH);
delay(10);
digitalWrite(ledPin,LOW);
delay(100);

} 
}
void setup()
{
pinMode(ledPin,OUTPUT);
stepper1.setSpeed(20); 
stepper2.setSpeed(20);
Serial.begin(9600);
 //  pinMode(pulsador,INPUT);
 // pinMode(led,OUTPUT);
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
    while (!Serial) {
  }

  BT.begin(9600);
  Serial.println("Conectado... Enviar los comandos AT");
ledParpadea();
BT.print("AT+NAME"); BT.print("COCOBOT"); delay(1000);
}
 
void frente (int steps) {
  while (steps--) {
    stepper1.step(1, FORWARD, SINGLE); 
    stepper2.step(1, FORWARD, SINGLE); 
  }
  
} 
 
void atras (int steps) {
  while (steps--) {
    stepper1.step(1, BACKWARD, SINGLE); 
    stepper2.step(1, BACKWARD, SINGLE); 
  }
} 

void giraIzquierda (int steps) {
  while (steps--) {
    stepper1.step(1, BACKWARD, SINGLE); 
    stepper2.step(1, FORWARD, SINGLE); 
  }
} 

void giraDerecha (int steps) {
  while (steps--) {
    stepper1.step(1, FORWARD, SINGLE); 
    stepper2.step(1, BACKWARD, SINGLE); 
  }
} 
void parar(){
 stepper1.release();
stepper2.release(); 
}

void leerDistancia(){
digitalWrite(trigPin,LOW);
delayMicroseconds(2);
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);
duracion=pulseIn(echoPin,HIGH);
distancia=(duracion/2)/29.1;
BT.println(distancia);
}

void evitarObstaculos(){
 leerDistancia();
if(distancia<cerca){
 esquiva();
} 
else{
 frente(numero_de_pasos); 
}
}

void esquiva(){
  parar();
  delay(100);
  giraIzquierda(numero_de_pasos);
  leerDistancia();
  medidaIzquierda=distancia;
  delay(100);
  giraDerecha(numero_de_pasos);
  giraDerecha(numero_de_pasos);
  leerDistancia();
  medidaDerecha=distancia;
  delay(100);
  giraIzquierda(numero_de_pasos);
  delay(100);
  if(medidaIzquierda<medidaDerecha){
   giraDerecha(numero_de_pasos);
  delay(100);   
  }
  else
  {
  giraIzquierda(numero_de_pasos);
  delay(100);  
  }
}

void loop()
{
  if(programa==1){
    evitarObstaculos();
  }
    if (BT.available()){
      datoBT=BT.read();
    Serial.write(BT.read());
    switch (datoBT){
      case '0':
      programa=0;
      parar();
      break;
      case '1':
      programa=0;
      frente(numero_de_pasos);
      break;
      case '2':
      programa=0;
      giraIzquierda(numero_de_pasos);
      break;
      case '3':
      programa=0;
      giraDerecha(numero_de_pasos);
      break;
      case '4':
      programa=0;
      atras(numero_de_pasos);
      break; 
      case '5':
      programa=0;
      ledParpadea();
      break;
      case '6':
      programa=1;
      BT.println("programa 1");
      break;
    }
    }
  if (Serial.available())
    BT.write(Serial.read());
  
  
leerDistancia();
if (distancia<20){
Serial.println("ATCHUNG!!!");

 digitalWrite(ledPin,HIGH);
delay(10);
digitalWrite(ledPin,LOW);
delay(100);

}
numero_de_pasos = angulo / (360 / pasos);

}

