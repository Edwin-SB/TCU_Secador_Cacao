// Real time clock, calendar, temperature, humidity data logger using Arduino, DS3231 and DHT22 sensor
 
#include <SPI.h>              // Include SPI library (needed for the SD card)
#include <SD.h>               // Include SD library
#include <LiquidCrystal_I2C.h>   // LCD library code
#include <Wire.h>             // Include Wire library code (needed for I2C protocol devices)
#include <DHT.h>              // Include DHT library code
#include "RTClib.h"
// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal_I2C lcd(0x27,20,4); 
#define B1      A1            // Button B1 is connected to Arduino pin A1
#define B2      A2            // Button B1 is connected to Arduino pin A2
#define DHTPIN  A3            // DHT22 data pin is connected to Arduino pin A3
 
#define DHTTYPE DHT22         // DHT22 sensor is used
DHT dht(DHTPIN, DHTTYPE);     // Initialize DHT library
 
File dataLog;
boolean sd_ok = 0;
char temperature[] = " 00.0";
char humidity[]    = " 00.0 %";
char Time[]     = "  :  :  ";
char Calendar[] = "  /  /20  ";
int Temp, RH;

DateTime now;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

RTC_DS3231 rtc;

void showDate(void);
void showTime(void);
void showDay(void); 
 
void setup() {
  //lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin())
    Serial.println("initialization failed!");
  else {
    Serial.println("initialization done.");
    sd_ok = 1;
  }
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  lcd.begin(16, 2);                              // Set up the LCD's number of columns and rows
  Wire.begin();                                  // Join i2c bus
  dht.begin();
  
  lcd.setCursor(0, 0);   lcd.print("TIME:");
  lcd.setCursor(0, 1);   lcd.print("DATE:");
  lcd.setCursor(0, 2);   lcd.print("Temp =");
  lcd.setCursor(11, 2);  lcd.write(223);         // Print degree symbol ( °)
  lcd.setCursor(12, 2);  lcd.write('C');
  lcd.setCursor(0, 3);   lcd.print("RH   =");
  
  Serial.println("   DATE    |   TIME   | TEMPERATURE | HUMIDITY");
  
  if(sd_ok) {                                       // If SD card initialization was OK
    dataLog = SD.open("Logger.txt", FILE_WRITE);    // Open file Logger.txt
    if(dataLog) {                                   // if the file opened okay, write to it:
      dataLog.println("   DATE    ,   TIME   , EMPERATURE , HUMIDITY");
      dataLog.close();                              // Close the file
    }
  }
  
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
}// setup 
 

void blink_parameter(){
  byte j = 0;
  while(j < 10 && digitalRead(B1) && digitalRead(B2)){
    j++;
    delay(25);
  }
}
byte edit(byte x, byte y, byte parameter){
  char text[3];
  while(!digitalRead(B1));                        // Wait until button (pin #8) released
  while(true){
    while(!digitalRead(B2)){                      // If button (pin #9) is pressed
      parameter++;
 
      delay(200);                                // Wait 200ms
    }
    
    if(!digitalRead(B1)){                         // If button (pin #8) is pressed
                                        // Increament 'i' for the next parameter
      return parameter;                          // Return parameter value and exit
    }
  }
}
 
void loop() {
  now = rtc.now();
  if(!digitalRead(B1)){                           // If button (pin #8) is pressed
                        // Stop transmission and release the I2C bus
    delay(200);                                 // Wait 200ms
  }
   
                             // Diaplay time & calendar
  
  
    // Read humidity
    RH = dht.readHumidity() * 10;
    //Read temperature in degree Celsius
    Temp = dht.readTemperature() * 10;
    if(Temp < 0){
      temperature[0] = '-';                     // If temperature < 0 put minus sign
      Temp = abs(Temp);                         // Absolute value of 'Temp'
    }
    else
      temperature[0] = ' ';                     // otherwise (temperature > 0) put space
    temperature[1]   = (Temp / 100) % 10  + 48;
    temperature[2]   = (Temp / 10)  % 10  + 48;
    temperature[4]  =  Temp % 10 + 48;
    if(RH >= 1000)
      humidity[0]    = '1';                     // If humidity >= 100.0% put '1' of hundreds
    else
      humidity[0]    = ' ';                     // otherwise (humidity < 100) put space
    humidity[1]      = (RH / 100) % 10 + 48;    
    humidity[2]      = (RH / 10) % 10 + 48;
    humidity[4]     =  RH % 10 + 48;
    lcd.setCursor(6, 2);
    lcd.print(temperature);
    lcd.setCursor(6, 3);
    lcd.print(humidity);
    // Send data to Arduino IDE serial monitor
    Serial.print(Calendar);
    Serial.print(" | ");
    Serial.print(Time);
    Serial.print(" |   ");
    Serial.print(temperature);
    Serial.print("°C   |  ");
    Serial.println(humidity);
    if(sd_ok) {                                       // If SD card initialization was OK
      dataLog = SD.open("Logger.txt", FILE_WRITE);    // Open file Logger.txt
      if(dataLog) {                                   // if the file opened okay, write to it:
        dataLog.print(Calendar);
        dataLog.print(",");
        dataLog.print(Time);
        dataLog.print(",");
        dataLog.print(temperature);
        dataLog.print(",");
        dataLog.println(humidity);
        dataLog.close();                              // Close the file
      }
    }
  
  delay(50);                                          // Wait 50ms
}

void showDate()
 {
  lcd.setCursor(0,0);
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
 }
 void showDay()
 {
  lcd.setCursor(11,0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
 }
 void showTime()
 {
  lcd.setCursor(0,1);
  lcd.print("Time:");
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());
  lcd.print("    ");
 } 

 
