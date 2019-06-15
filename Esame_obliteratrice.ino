//include librerie
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>

unsigned long intervalloT;

//dichiara led RGB
#define Red 6
#define Green 5
#define Blue 3

//dichiara LCD I2C 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 11;    // Trigger
const int echoPin = 12;    // Echo
long duration;
int cm;

Servo mioServo;      //dichiara servo motore
int pos = 0;         //posizione iniziale

void setup() {
  //definisce trigger e echo del sensore di distanza
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  

  //inizializzazione porta seriale
  Serial.begin (9600);

  //inizializza LCD
  lcd.init();
  lcd.begin(16, 2);

  //dichiara led RGB
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
}

void loop() {
  //il sensore è triggherato da un in impulso HIGH di 10 o più microsendi.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echoPin, HIGH);
  
  //scrive in console distanza in cm
  Serial.print(duration);
  Serial.println();

  lcd.setBacklight(1);          //accendi l'illuminazione del display
  /*
    lcd.setBacklight(0);        //spenge l'illuminazione
    delay(1500);
  */

  //se la distanza dal sensore di prossimità registra un movimento in
  //meno di  453 microsecondi, timbra il biglietto   
  if ( duration < 453 ) {
    boolean finito = false;
    intervalloT = millis();
    int flag = 0;
    while(finito == false){
        unsigned long deltaT = millis();

        //led rgb rosso
        digitalWrite(Red, HIGH);
        digitalWrite(Green, LOW);
        digitalWrite(Blue, LOW);
        if(flag == 0){
          lcd.clear();
          //messaggio attendi
          lcd.print("Attendi");

          delay(2000);
          
          mioServo.attach(8);     //dichiara Servo motore
          mioServo.write(0);      //posizione iniziale
          flag++;
        }

        //azione servo motore
        if( deltaT > intervalloT + 5000){
          mioServo.write(0);
        }
        if( deltaT > intervalloT + 6000){
          mioServo.write(180);    //gira di 180°
          finito = true;  
        }

        if(finito == true){
          //led rgb verde
          digitalWrite(Red, LOW);
          digitalWrite(Green, HIGH);
          digitalWrite(Blue, LOW);
          
          lcd.clear();
          //messagio di fine
          lcd.print("Estrai il");
          lcd.setCursor(0,1);
          lcd.print("biglietto");
          delay(5000);
          lcd.clear();
          break;
        }
    }
  }else{
    mioServo.detach();      //scollega servo

    //led rgb spento
    digitalWrite(Red, LOW);
    digitalWrite(Green, LOW);
    digitalWrite(Blue, LOW);

    //messagio di inserire biglietto
    lcd.print("Inserisci qui");
    lcd.setCursor(0,1);
    lcd.print("il tuo biglietto");
  }
}
