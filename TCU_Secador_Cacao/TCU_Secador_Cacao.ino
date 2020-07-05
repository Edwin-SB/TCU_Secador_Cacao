#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include<stdlib.h>
#include "DHT.h"
#define DHTPIN 9     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#include <SPI.h>
#include <SD.h>
#include <Keypad.h>

DHT dht(DHTPIN, DHTTYPE);
DateTime now;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,20,4);

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
const int LED =13;
const int chipSelect = 10;
const int BUTTON =A0;

float t,h,f;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {7, 2, 3, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 8, 4};    //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



int BUTTONstate = 0;
int buttonACTION =0;
int i,j;
int k = 0;

int posicion=0;    // necesaria para la clave
int cursor=5;      // posicion inicial de la clave en el LCD
int clave=0;       // para el LCD
int luz=0;         // para el LCD
int tiempo=0;      // para el LCD
int ledVerde=11;   // pin para el LED verde
int ledRojo=12;    // pin para el LED rojo
int buzzer=10;     // pin altavoz
char Lastkey;

void setup(){ 
  
   lcd.noBacklight();     // apagamos LCD
      pinMode (ledVerde,OUTPUT);
      pinMode (ledRojo, OUTPUT);
      pinMode (buzzer, OUTPUT);
      digitalWrite(ledRojo,HIGH); // encendemos el LED rojo
      digitalWrite(ledVerde, LOW); // apagamos el verde

      lcd.setCursor(0,0);     // situamos el cursor el la posición 2 de la linea 0.
      lcd.print("Introduzca clave"); // escribimos en LCD
      Serial.print("Introduzca clave"); // escribimos en LCD
      lcd.setCursor(cursor,1); // cursor en la posicion de la variable, linea 1
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  Serial.println("DHTxx test!");
 
  dht.begin();

  
  //delay(2000);
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC Module");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}


void loop(){

  Lastkey = keypad.getKey(); // leemos pulsacion
  //Serial.println(Lastkey);
  if(Lastkey !=NO_KEY){
    switch(Lastkey){ // asterisco para resetear el contador
      case '*':
           Serial.println("Edwin apreto *");// escribimos en LCD
           break;
           
      case '#':  
            
            
            while( Lastkey == '#'){
            Lastkey = keypad.getKey(); // leemos pulsacion
            if(Lastkey == NO_KEY){
             Lastkey = '#';
            }
            //microSD();
            Serial.println("Edwin apreto #");// escribimos en LCD
            }
          
            
            break;
            
      case '3':
            Serial.println("Edwin apreto 3");
            break;
      default:
        Serial.println(Lastkey);
      }  
  }
  Serial.print(Lastkey);
  

    
/*
  String stringUser = "";
    stringUser += Lastkey;
    stringUser += ".csv"; */
    
}//loop


void microSD(){
  
   
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;}
    
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  String stringTime1 = "";
  float h;
    // Read temperature as Celsius
  float t;
    // Read temperature as Fahrenheit
  float f;
 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataLog = SD.open("final20.csv", FILE_WRITE);
  Serial.print(" Llegamos ");
  
  // if the file is available, write to it:
  
    now = rtc.now();
    stringTime1 = "";
    stringTime1 += now.hour();
    stringTime1 += ":";
    stringTime1 += now.minute();
    stringTime1 += ":";
    stringTime1 += now.second();
    
    h = dht.readHumidity();
    // Read temperature as Celsius
    t = dht.readTemperature();
    
    Serial.print("Hora: "); 
    Serial.print(stringTime1);
    Serial.print("  ");
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %");
    Serial.print("  ");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    Serial.print("  ");
    
    
    if (dataLog) {           
          dataLog.print("Hora");
          dataLog.print(",");
          dataLog.print(stringTime1);
          dataLog.print(",");
          dataLog.print("Temperatura");
          dataLog.print(",");
          dataLog.print(t);
          dataLog.print(",");
          dataLog.print("Humedad");
          dataLog.print(",");
          dataLog.println(t);
          Serial.print("Sirvio");
    }// if datolog
    
    else {
    Serial.println("error opening datalog.txt");
    }
    
  
  dataLog.close(); 
}//microsd

void  usuario(){
  
  char pulsacion = keypad.getKey(); // leemos pulsacion
      if (pulsacion != 0) //Si el valor es 0 es que no se ha pulsado ninguna tecla
        { // descartamos almohadilla y asterisco
          if (pulsacion != '#' && pulsacion != '*' && clave==0){
            lcd.print(pulsacion); // imprimimos pulsacion
             Serial.print(pulsacion);
             cursor++;             // incrementamos el cursor
             tone(buzzer,350);     // tono de pulsacion
             delay(200);
             noTone(buzzer);

      //--- Condicionales para comprobar la clave introducida -----------
      // comparamos entrada con cada uno de los digitos, uno a uno
      
          posicion ++; // aumentamos posicion si es correcto el digito

      if (posicion == 4)
       { // comprobamos que se han introducido los 4 correctamente
         digitalWrite (13,HIGH);  // encendemos LED
         lcd.setCursor(0,0);      // situamos el cursor el la pos 0 de la linea 0.
         lcd.print("Usuario Correcto? ");
         Serial.println("  ");
         Serial.println("Usuario Correcto? ");// escribimos en LCD
         Serial.println("SI(#) NO(*)");
         delay(200);                           // tono de clave correcta
         tone(buzzer,500);
         delay(100);
         noTone(buzzer);
         tone(buzzer,600);
         delay(100);
         noTone(buzzer);
         tone(buzzer,800);
         delay(100);
         noTone(buzzer);

         lcd.setCursor(5,1); // cursor en la posicion 5, linea 1
         clave=1; // indicamos que se ha introducido la clave
         digitalWrite(ledRojo,LOW); // apagamos el LED rojo
         digitalWrite(ledVerde, HIGH); // encendemos el verde
     }
     //--- En el caso de que este incompleta o no hayamos acertado ----------
     if(cursor>8)        // comprobamos que no pase de la cuarta posicion
       {   cursor=5;     // lo volvemos a colocar al inicio
           posicion=0;           // borramos clave introducida
           lcd.setCursor(5,1);
           lcd.print(" ");       // borramos la clave de la pantalla
           lcd.setCursor(5,1);
           if(clave==0)         // comprobamos que no hemos acertado
              { tone(buzzer,70,500); // para generar
                delay(250); // tono de error
                noTone(buzzer);
              }
        }
     }
   } 

 //--- Condicionales para encender o apagar el LCD --------------
 if (pulsacion == '#' && luz==0)
     { // comprobamos tecla y encendemos si esta apagado
       lcd.backlight(); // encendemos
       luz=1; // indicamos que esta encendida
       pulsacion =0; // borramos el valor para poder leer el siguiente condicional
     }

 if (pulsacion == '#' && luz==1)
     { // comprobamos tecla y estado
       lcd.noBacklight(); // apagamos
       luz=0; // indicamos que esta apagada
     }

 //--- Condicionales para resetear clave introducida -------------
 switch(pulsacion){ // asterisco para resetear el contador
  case '*':
       posicion = 0;
       cursor = 5;
       clave=0;
       posicion=0;
       lcd.setCursor(0,0); // situamos el cursor el la posición 2 de la linea 0.
       lcd.print("Introduzca clave"); 
       Serial.println("Introduzca clave");// escribimos en LCD
       lcd.setCursor(5,1);
       lcd.print(" "); // borramos de la pantalla los numeros
       lcd.setCursor(5,1);
    
       digitalWrite(ledRojo,HIGH); // encendemos el LED rojo
       digitalWrite(ledVerde, LOW); // apagamos el verde
       break;
   case '#':
        microSD();
        break;
    
    }
    
    
  }//usuario
