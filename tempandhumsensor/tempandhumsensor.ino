#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 18
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LED_PIN 4
#define TEMP_THRESHOLD 28.0

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Starting...");
    display.display();
  }
}

void loop() {
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // °C
  bool valid = !(isnan(hum) || isnan(temp)); //checks whether the sensor readings read a floating point number

  if (!valid) {
    Serial.println("DHT read FAILED — turning LED OFF and retrying");
    digitalWrite(LED_PIN, LOW);   //If no floating point number reading from the sensors it'll output fail screen on OLED display
    delay(2000);
    return;
  }

  

  if (temp > TEMP_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temp: "); display.print(temp,1); display.println("C");
  display.setTextSize(1);
  display.setCursor(0,36);
  display.print("Humidity: "); display.print(hum,1); display.println("%");
  display.display();

  delay(2000);
}
