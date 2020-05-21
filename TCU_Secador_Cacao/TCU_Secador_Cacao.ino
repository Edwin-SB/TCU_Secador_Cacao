#include <SPI.h>              // SPI library (needed for the SD card)
#include <SD.h>               // SD library
#include <LiquidCrystal.h>    // LCD library code
#include <Wire.h>             // Wire library code (needed for I2C protocol devices)
#include <DHT.h>              // DHT library code

#define B1      A1            // Button B1 is connected to Arduino pin A1
#define B2      A2            // Button B1 is connected to Arduino pin A2
#define DHTPIN  A3            // DHT22 data pin is connected to Arduino pin A3
#define DHTTYPE DHT22         // DHT22 sensor is used
DHT dht(DHTPIN, DHTTYPE);     // Initialize DHT library

// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

File dataLog;


char temperature[] = " 00.0";
char humidity[]    = " 00.0 %";
char Time[]     = "  :  :  ";
char Calendar[] = "  /  /20  ";
byte i, second, minute, hour, date, month, year, previous_second;
int Temp, RH;
boolean sd_ok = 0;

const int pinMicro = 53;

void setup(){
  Serial.begin(9600);
  
  // Inicio de la MicroSD
  Serial.print("Initializing SD card...");
  pinMode(pinMicro, OUTPUT);
  if (!SD.begin(pinMicro)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  dataLog = SD.open("TCU-230.txt", FILE_WRITE);
  Serial.println("   DATE    |   TIME   | TEMPERATURE | HUMIDITY");
  if(sd_ok) {                                       // If SD card initialization was OK
    dataLog = SD.open("Logger.txt", FILE_WRITE);    // Open file Logger.txt
    if(dataLog) {                                   // if the file opened okay, write to it:
      dataLog.println("   DATE    |   TIME   | TEMPERATURE | HUMIDITY");
      dataLog.close();                              // Close the file
    }
  }
 
  // if the file opened okay, write to it:
  microSD();
}//setup
 
void loop()
{
  // nothing happens after setup
}//loop

void microSD(){
  if (dataLog) {
    Serial.print("Writing to test.txt...");
    dataLog.println("hola");
  // close the file:
    dataLog.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  }//microSD

void blink_parameter(){
  byte j = 0;
  while(j < 10 && digitalRead(B1) && digitalRead(B2)){
    j++;
    delay(25);
  }
}// blink parameter

byte edit(byte x, byte y, byte parameter){
  char text[3];
  while(!digitalRead(B1));                        // Wait until button (pin #8) released
  while(true){
    while(!digitalRead(B2)){                      // If button (pin #9) is pressed
      parameter++;
      if(i == 0 && parameter > 23)               // If hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 1 && parameter > 59)               // If minutes > 59 ==> minutes = 0
        parameter = 0;
      if(i == 2 && parameter > 31)               // If date > 31 ==> date = 1
        parameter = 1;
      if(i == 3 && parameter > 12)               // If month > 12 ==> month = 1
        parameter = 1;
      if(i == 4 && parameter > 99)               // If year > 99 ==> year = 0
        parameter = 0;
      sprintf(text,"%02u", parameter);
      lcd.setCursor(x, y);
      lcd.print(text);
      delay(200);                                // Wait 200ms
    }
    lcd.setCursor(x, y);
    lcd.print("  ");                             // Display two spaces
    blink_parameter();
    sprintf(text,"%02u", parameter);
    lcd.setCursor(x, y);
    lcd.print(text);
    blink_parameter();
    if(!digitalRead(B1)){                         // If button (pin #8) is pressed
      i++;                                       // Increament 'i' for the next parameter
      return parameter;                          // Return parameter value and exit
    }
  }
}
 
void DS3231_display(){
  // Convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour >> 4)   * 10 + (hour & 0x0F);
  date   = (date >> 4)   * 10 + (date & 0x0F);
  month  = (month >> 4)  * 10 + (month & 0x0F);
  year   = (year >> 4)   * 10 + (year & 0x0F);
  // End conversion
  Time[7]     = second % 10 + 48;
  Time[6]     = second / 10 + 48;
  Time[4]      = minute % 10 + 48;
  Time[3]      = minute / 10 + 48;
  Time[1]      = hour   % 10 + 48;
  Time[0]      = hour   / 10 + 48;
  Calendar[9] = year   % 10 + 48;
  Calendar[8] = year   / 10 + 48;
  Calendar[4]  = month  % 10 + 48;
  Calendar[3]  = month  / 10 + 48;
  Calendar[1]  = date   % 10 + 48;
  Calendar[0]  = date   / 10 + 48;
  lcd.setCursor(5, 0);
  lcd.print(Time);                               // Display time
  lcd.setCursor(5, 1);
  lcd.print(Calendar);                           // Display calendar
}
