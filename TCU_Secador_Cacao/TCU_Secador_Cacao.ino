#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

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


const int BUTTON =A0;
int BUTTONstate = 0;
int buttonACTION =0;
 int i;


void setup()
{ 
  lcd.begin(16,2);
  
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


void loop()
{
  now = rtc.now();
    // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String stringTime = "";
  stringTime += now.hour();
  stringTime += ":";
  stringTime += now.minute();
  stringTime += ":";
  stringTime += now.second();
  Serial.print("Hora: "); 
  Serial.print(stringTime);
  Serial.print("  ");
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %");
  Serial.print("  ");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.println(" *C");
  Serial.print("  ");

  


 microSD();

  
}//loop


void microSD(){

  BUTTONstate = digitalRead(BUTTON);
  char key = keypad.getKey();// lee lo que se introcice en el teclado
  String stringTime1 = "";
  stringTime1 += now.hour();
  stringTime1 += ":";
  stringTime1 += now.minute();
  stringTime1 += ":";
  stringTime1 += now.second();

  String stringUser = "";
    stringUser += key;
    stringUser += ".txt";
  
   
    
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataLog = SD.open(stringUser, FILE_WRITE);
  
  // if the file is available, write to it:
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
        dataLog.close();  
    // print to the serial port too: 
  }// if datolog
 
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  }//microsd
