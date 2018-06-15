/*  
A TCP server that send 4 ToF sensors data
*/

#include <Wire.h>
#include <VL53L0X.h>
#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h" 
#include <Packetizer.h>

#include "Sodaq_wdt.h"
#ifdef ARDUINO_ARCH_AVR
#define DIAG_STREAM Serial
#elif ARDUINO_ARCH_SAMD
#define DIAG_STREAM SerialUSB
#endif

#define FSHUT 5 
#define RSHUT 2 
#define LSHUT 3
#define BSHUT 4

VL53L0X sensorF;
VL53L0X sensorR;
VL53L0X sensorL;
VL53L0X sensorB;

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2391;    // local port to listen on

char packetBuffer[255];             //buffer to hold incoming packet

int status = WL_IDLE_STATUS;
WiFiServer server(localPort);
boolean alreadyConnected = false;   // whether or not the client was connected previously

void setupWiFi() {
  //Initialize serial and wait for port to open:
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  server.begin();
  printWiFiStatus();
}

void setupToFSensor() {
  pinMode(FSHUT, OUTPUT);
  pinMode(RSHUT, OUTPUT);
  pinMode(LSHUT, OUTPUT);
  pinMode(BSHUT, OUTPUT);
  digitalWrite(FSHUT, LOW);
  digitalWrite(RSHUT, LOW);
  digitalWrite(LSHUT, LOW);
  digitalWrite(BSHUT, LOW);
 
  delay(100);
  Wire.begin();

  pinMode(FSHUT, INPUT);
  delay(150);
  sensorF.init(true);
  delay(100);
  sensorF.setAddress((uint8_t)24);
  sensorF.setTimeout(500);

  pinMode(RSHUT, INPUT);
  delay(150);
  sensorR.init(true);
  delay(100);
  sensorR.setAddress((uint8_t)23);
  sensorR.setTimeout(500);
 
  pinMode(LSHUT, INPUT);
  delay(150);
  sensorL.init(true);
  delay(100);
  sensorL.setAddress((uint8_t)22);
  sensorL.setTimeout(500);
 
  pinMode(BSHUT, INPUT);
  delay(150);
  sensorB.init(true);
  delay(100);
  sensorB.setAddress((uint8_t)25);
  sensorB.setTimeout(500);

  sensorF.startContinuous();
  sensorR.startContinuous();
  sensorL.startContinuous();
  sensorB.startContinuous();
  digitalWrite(13,LOW);
}

void measureDistance() {
  Serial.print("F: ");
  Serial.print(sensorF.readRangeContinuousMillimeters());
  Serial.print("R: ");
  Serial.print(sensorR.readRangeContinuousMillimeters());
  Serial.print(" L: ");
  Serial.print(sensorL.readRangeContinuousMillimeters());
  Serial.print(" B: ");
  Serial.println(sensorB.readRangeContinuousMillimeters());
}
void sendSensorData() {
  // wait for a new client:
  WiFiClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      //client.println("Hello, client!");
      alreadyConnected = true;
    }

    if (client.available() > 0) {
      uint16_t fRange = sensorF.readRangeContinuousMillimeters();
      uint16_t rRange = sensorR.readRangeContinuousMillimeters();
      uint16_t lRange = sensorL.readRangeContinuousMillimeters();
      uint16_t bRange = sensorB.readRangeContinuousMillimeters();
      if (fRange == 65535) {
        Serial.println("Resetting sensor");
        setupToFSensor();
      }
      Packetizer::Packer packer; // default packet checker is crc8
      uint8_t dataArray[8] = { 
                                (uint8_t)((fRange >> 8) & 0xFF),
                                (uint8_t)((fRange >> 0) & 0xFF),
                                (uint8_t)((rRange >> 8) & 0xFF),
                                (uint8_t)((rRange >> 0) & 0xFF),
                                (uint8_t)((lRange >> 8) & 0xFF),
                                (uint8_t)((lRange >> 0) & 0xFF),
                                (uint8_t)((bRange >> 8) & 0xFF),
                                (uint8_t)((bRange >> 0) & 0xFF)
                              };
      packer.pack(dataArray, sizeof(dataArray));
      server.write(packer.data(), packer.size());
    }
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  //Serial.begin(19200);
  setupWiFi();
  setupToFSensor();
  
  // Enable WDT
  sodaq_wdt_enable(WDT_PERIOD_8X);
  // Startup message (also seen after a reset)
  DIAG_STREAM.begin(57600);
  DIAG_STREAM.println("Start up");
}

void loop() {
  // If the WDT interrupt has been triggered
  if (sodaq_wdt_flag) {
    sodaq_wdt_flag = false;
    sodaq_wdt_reset();
    
    DIAG_STREAM.println("WDT interrupt has been triggered");
  }
  
  sendSensorData();
  measureDistance();
}


