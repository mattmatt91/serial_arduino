
// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme;

void setup(void)
{
  Serial.begin(115200);
  while (!Serial)
    ;

  if (!lis.begin(0x19))
  { // change this to 0x19 for alternative i2c address
    Serial.println("Could not find a valid LIS3DH sensor!");
    while (1)
      yield();
  }

  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME680 sensor!");
    while (1)
      yield();
  }

  lis.setRange(LIS3DH_RANGE_2_G); // 2, 4, 8 or 16 G!
  lis.setDataRate(LIS3DH_DATARATE_200_HZ);

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  if (!bme.performReading())
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
  float Temperature = bme.temperature;
  float Pressure = bme.pressure / 100;
  float Humidity = bme.humidity;
  float Gas = bme.gas_resistance / 1000;

  String msg_bme = "{\"Temperature\":" + String(Temperature) + "," +
                   "\"Pressure\":" + String(Pressure) + "," +
                   "\"Humidity\":" + String(Humidity) + "," +
                   "\"Gas\":" + String(Gas) +

                   "}";
  Serial.println(msg_bme)
}

void loop()
{

  lis.read(); // get X Y and Z data at once
  // Then print out the raw data
  float X = lis.x;
  float Y = lis.y;
  float Z = lis.z;

  /* Or....get a new sensor event, normalized */
  sensors_event_t event;
  lis.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  float Xn = event.acceleration.x;
  float Yn = event.acceleration.y;
  float Zn = event.acceleration.z;
  String msg = "{\"X\":" + String(X) + "," +
               "\"Y\":" + String(Y) + "," +
               "\"Z\":" + String(Z) + "," +
               "\"Xn\":" + String(Xn) + "," +
               "\"Yn\":" + String(Yn) + "," +
               "\"Zn\":" + String(Zn) +
               "}";
  Serial.println(msg);

  delay(10);
}
