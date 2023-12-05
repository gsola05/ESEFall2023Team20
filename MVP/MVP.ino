#include <Servo.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <WiFiS3.h>  // http://10.103.201.189/

#define DHTPIN 11

// Ultrasonic Constants
const int trigPin = 6;
const int echoPin = 5;

const float winThresh = 10.0;  // How dar the window is when fully closed

long duration;

// Temp constants
DHT dht(DHTPIN, DHT11);
float h;
float t;

// LCD constants
const int rs = 12, en = 10, d4 = 9, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Stepper Constants
// 2048 steps == 1 Revolution
Stepper myStepper = Stepper(2048, 7, 11, 8, 1);

bool open = false;

//WiFi Constants
const char ssid[] = "AirPennNet-Device";
const char pass[] = "penn1740wifi";

int status = WL_IDLE_STATUS;

WiFiServer server(80);


void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myStepper.setSpeed(5);
}

void loop() {

  float temp = getTemp();
  float humid = getHum();
  Serial.println(String(temp) + "," + String(humid));

  // "Physcial" Things We Do
  moveWindow(temp, humid);
  delay(1000);
}

float getTemp() {
  return dht.readTemperature();
}

float getHum() {
  return dht.readHumidity();
}

float getDist() {
  // Use ping sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Distance Readings
  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;  // Returns in cm
}

void moveWindow(float t, float h) {
  if (t > 28.0 || h > 50.0) {     // Close the Window
    if (getDist() > winThresh) {  // Window too far come back
      //Serial.println("Too Far!");
      myStepper.step(256);
    }
  } else {                        // Open the Window
    if (getDist() < winThresh) {  // Window too close go away
      //Serial.println("Too Close!");
      myStepper.step(256);
    }
  }
}
