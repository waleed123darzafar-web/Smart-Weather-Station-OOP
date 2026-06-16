#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>


// ENUM DEFINITIONS
enum AirQualityLevel 
{

    AQ_GOOD,
    AQ_MID,
    AQ_POOR

};

enum RainLevel 
{

    RAIN_HEAVY,
    RAIN_AVG,
    RAIN_NONE

};

// LCD (I2C 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27

// DHT11 Setup 
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// 1️⃣ Temperature & Humidity Sensor Class
class TempHumiditySensor {
private:

    DHT* sensor;

public:
    TempHumiditySensor(DHT* sensor) {

        this->sensor = sensor;

    }

    float readTemperature() {

        return sensor->readTemperature();

    }

    float readHumidity() {

        return sensor->readHumidity();

    }
};

// 2️⃣ MQ-135 Air Quality Sensor Class
class AirQualitySensor {
private:

    int pin;

public:
    AirQualitySensor(int pin) 
    {

        this->pin = pin;

    }

    int readAirQuality() 
    {

        return analogRead(pin); // Returns 0-1023

    }

    AirQualityLevel getAirQualityLevel() 
    {

        int value = analogRead(pin);

        if (value < 200) 
        
        return AQ_GOOD;

        else if (value < 500) 
        
        return AQ_MID;

        else 
        
        return AQ_POOR;
    }

    String airQualityToString(AirQualityLevel level) 
    {

    switch (level) 
    {
        case AQ_GOOD: return "Good";
        case AQ_MID:  return "Mid";
        case AQ_POOR: return "Poor";
        default: return "Unknown";
    }
    }

};

// 3️⃣ MH-RD Rain Sensor Class
class RainSensor {
private:
    int pin;

public:
    RainSensor(int pin) 
    {
        this->pin = pin;
    }

    int readRainValue() 
    {
        return analogRead(pin);
    }

        RainLevel getRainStatus() 
        {
        
        int value = analogRead(pin);

        if (value < 500) 
        
        return RAIN_HEAVY;

        else if (value < 900)
        
         return RAIN_AVG;

        else 

        return RAIN_NONE;
        }

        String rainToString(RainLevel level) 
        {

        switch (level)
        {
        case RAIN_HEAVY: return "Heavy";
        case RAIN_AVG:   return "Avg";
        case RAIN_NONE:  return "None";
        default: return "Unknown";
        }

        }
};

// SENSOR OBJECTS 
TempHumiditySensor thSensor(&dht);
AirQualitySensor airSensor(A0);  // MQ-135 connected to A0
RainSensor rainSensor(A1);       // Rain sensor connected to A1

// SETUP 
void setup() 
{

    Serial.begin(9600);
    Serial.println(" Temp, Humidity, AQ, AQ_Level, Rain, Rain_Status");
    dht.begin();

    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0,0);
    delay(2000);
    lcd.print("      SMART      ");
    lcd.setCursor(0,1);
    lcd.print("WEATHER STATION");
    delay(2000);
    lcd.clear();

}


void loop() 
{

    // ----------- Read Sensor Values ------------
    float temperature = thSensor.readTemperature();
    float humidity    = thSensor.readHumidity();

    int airValue = airSensor.readAirQuality();
    AirQualityLevel airLevelEnum = airSensor.getAirQualityLevel();
    String airLevel = airSensor.airQualityToString(airLevelEnum);

    int rainValue = rainSensor.readRainValue();
    RainLevel rainLevelEnum = rainSensor.getRainStatus();
    String rainStatus = rainSensor.rainToString(rainLevelEnum);


    // ----------- Serial Output ----------------
    unsigned long t = millis() / 1000; // seconds since start (time since startup)
    Serial.print("  "); Serial.print(t); Serial.print("s, ");
    Serial.print(temperature); Serial.print(", ");
    Serial.print(humidity); Serial.print(", ");
    Serial.print(airValue); Serial.print(", ");
    Serial.print(airLevel); Serial.print(", "); 
    Serial.print(rainValue); Serial.print(", "); 
    Serial.println(rainStatus);


     // ----------- LCD Display ------------------
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("T:"); 
    lcd.print(temperature);
    lcd.print("C H:"); 
    lcd.print(humidity);


    lcd.setCursor(0,1);
    lcd.print("AQ:"); 
    lcd.print(airLevel);
    lcd.print("  R:"); 
    lcd.print(rainStatus);


    delay(1500); // Update every 1.5 sec
}
