#include <AccelStepper.h>
#include <DHT.h>
#include <WiFiS3.h> // 10.103.201.33
#include "index.h"

#define DHTPIN 11

// Ultrasonic Constants
const int trigPin = 6;
const int echoPin = 5;

const float winThresh = 10.0; // How dar the window is when fully closed

long duration;

// Temp constants
DHT dht(DHTPIN, DHT11);
float h;
float t;

// Stepper Constants
#define HALFSTEP 8

#define motorPin1  8     //  Blue   on 28BYJ-48
#define motorPin2  7     //  Pink   on 28BYJ-48
#define motorPin3  10    //  Yellow on 28BYJ-48
#define motorPin4  9     //  Orange on 28BYJ-48

int currentPos;        // Move this many steps - 2048 = 1 turn

AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
// 2048 steps == 1 Revolution

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

  stepper.setMaxSpeed(20000);
  stepper.setAcceleration(20000);
  stepper.setSpeed(14000);

  // be up to date
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION){
    Serial.println("Please upgrade the firmware");
  }

// attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  server.begin();
  printWifiStatus();

}

void loop() {

  float temp = getTemp();
  float humid = getHum();

  // "Physcial" Things We Do
  moveWindow(temp,humid);
  stepper.run();

  // Show everything on the website
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // read the HTTP request header line by line
    while (client.connected()) {
      if (client.available()) {
        String HTTP_header = client.readStringUntil('\n');  // read the header line of HTTP request

        if (HTTP_header.equals("\r")) {  // the end of HTTP request
          break;
        }

        Serial.print("<< ");
        Serial.println(HTTP_header);  // print HTTP request to Serial Monitor
      }
    }

    // send the HTTP response
    // send the HTTP response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();                     // the separator between HTTP header and body

    // send the HTTP response body
    String html = String(HTML_CONTENT);

    if (getDist() <= winThresh) { // Show how the window is
      html.replace("OPEN_HOLDER", "CLOSED");
    } else {
      html.replace("OPEN_HOLDER", "OPEN");
    }

    html.replace("TEMP_HOLDER", String(temp, 1)); // plug in the real temp
    html.replace("HUM_HOLDER", String(humid, 1)); // plug in the real humidity

    client.println(html); // send the actual website
    client.flush();

    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
  }
}

void printWifiStatus() {
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
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
  return duration*0.034/2; // Returns in cm
}


void moveWindow(float t, float h) {
  if (t > 28.0 || h > 50.0) { // Close the Window
    if (getDist() > winThresh) { // Window too far come back 
      Serial.println("Too Far!");
      stepper.move(256);
    }
  } else { // Open the Window
    if (getDist() < winThresh) { // Window too close go away 
      Serial.println("Too Close!");
      stepper.move(-256);
    }
  }
  stepper.run();
}
