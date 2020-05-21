// Real time clock, calendar, temperature, humidity data logger using Arduino, DS3231 and DHT22 sensor
 
#include <SPI.h>              // Include SPI library (needed for the SD card)
#include <SD.h>               // Include SD library
#include <LiquidCrystal_I2C.h>   // LCD library code
#include <Wire.h>             // Include Wire library code (needed for I2C protocol devices)
#include <DHT.h>              // Include DHT library code
 
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
byte i, second, minute, hour, date, month, year, previous_second;
int Temp, RH;
 
void setup() {
  lcd.init();                      // initialize the lcd 
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
  lcd.begin(20, 4);                              // Set up the LCD's number of columns and rows
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
 
void loop() {
  if(!digitalRead(B1)){                           // If button (pin #8) is pressed
    i = 0;
    hour   = edit(5, 0, hour);
    minute = edit(8, 0, minute);
    date   = edit(5, 1, date);
    month  = edit(8, 1, month);
    year   = edit(13, 1, year);
    // Convert decimal to BCD
    minute = ((minute / 10) << 4) + (minute % 10);
    hour = ((hour / 10) << 4) + (hour % 10);
    date = ((date / 10) << 4) + (date % 10);
    month = ((month / 10) << 4) + (month % 10);
    year = ((year / 10) << 4) + (year % 10);
    // End conversion
    // Write data to DS3231 RTC
    Wire.beginTransmission(0x68);               // Start I2C protocol with DS3231 address
    Wire.write(0);                              // Send register address
    Wire.write(0);                              // Reset sesonds and start oscillator
    Wire.write(minute);                         // Write minute
    Wire.write(hour);                           // Write hour
    Wire.write(1);                              // Write day (not used)
    Wire.write(date);                           // Write date
    Wire.write(month);                          // Write month
    Wire.write(year);                           // Write year
    Wire.endTransmission();                     // Stop transmission and release the I2C bus
    delay(200);                                 // Wait 200ms
  }
 
  Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
  Wire.write(0);                                // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 7);                    // Request 7 bytes from DS3231 and release I2C bus at end of reading
  second = Wire.read();                         // Read seconds from register 0
  minute = Wire.read();                         // Read minuts from register 1
  hour   = Wire.read();                         // Read hour from register 2
  Wire.read();                                  // Read day from register 3 (not used)
  date   = Wire.read();                         // Read date from register 4
  month  = Wire.read();                         // Read month from register 5
  year   = Wire.read();                         // Read year from register 6
  
  DS3231_display();                             // Diaplay time & calendar
  
  if(previous_second != second){
    previous_second = second;
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
  }
  delay(50);                                          // Wait 50ms
}
