#include "DFRobot_RainfallSensor.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ---------------- OLED Settings ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------------- Rainfall Sensor Settings ----------------
#define MODE_UART
#ifdef MODE_UART
#include "SoftwareSerial.h"
SoftwareSerial mySerial(0, 1);  // RX, TX (⚠️ avoid if using Serial Monitor on same pins)
DFRobot_RainfallSensor_UART Sensor(&mySerial);
#else
DFRobot_RainfallSensor_I2C Sensor(&Wire);
#endif

// ---------------- DHT11 Settings ----------------
#define DHTPIN 2       // Connect DHT11 data pin to D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
#ifdef MODE_UART
  mySerial.begin(9600);
#endif
  Serial.begin(115200);

  // OLED initialize
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println("AR RoboTics");
  display.setCursor(10, 20);
  display.println("Rain + DHT11 Init");
  display.display();
  delay(3000);

  // Rainfall Sensor init
  while (!Sensor.begin()) {
    Serial.println("Rainfall Sensor init err!!!");
    delay(1000);
  }
  Serial.println("Rainfall Sensor Ready!");

  // DHT11 init
  dht.begin();
}

void loop() {
  // Read Rainfall
  float rainfall = Sensor.getRainfall();

  // Read DHT11
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  display.clearDisplay();

  // Rainfall Data
  if (rainfall <= 0.01) {
    Serial.println("No Rain Detected");
    display.setTextSize(1);
    display.setCursor(4, 0);
    display.println("Rain Detector");
    display.setTextSize(2);
    display.setCursor(20, 17);
    display.println("NO RAIN");
  } else {
    Serial.print("Rainfall: ");
    Serial.print(rainfall);
    Serial.println(" mm");

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Rainfall Detected:");
    display.setCursor(0, 17);
    display.setTextSize(2);
    display.print(rainfall, 2);
    display.println("mm");
  }

  // Temperature and Humidity Data
  display.setTextSize(1);
  display.setCursor(0, 45);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.setCursor(0, 56);
  display.print("Hum: ");
  display.print(hum);
  display.println(" %");

  display.display();
  delay(3000);
}
