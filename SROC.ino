/*........................
  SROC Sliding Roof Observatory Control
  Jorma Ryske 10.8.2021
  Sfeomi Sliding gate motor
  Temperature Sensor AM2301
  ........................
*/
#include "DHT.h"
#define DHTPIN 2    // modify to the pin we connected
#define DHTTYPE DHT21   // AM2301 
DHT dht(DHTPIN, DHTTYPE);

int CurrentRoofPosition = 0;

void setup() {                
  // initialize the digital pins as an output.

  pinMode(3, OUTPUT); //Red Led On/Off, controlling camera IR Light Off/Auto
  pinMode(4, OUTPUT); //Relay04 LIGHT On/Off
  pinMode(5, OUTPUT); //Relay03 OPEN Roof
  pinMode(6, OUTPUT); //Relay02 STOP
  pinMode(7, OUTPUT); //Relay01 CLOSE Roof
 
  Serial.begin(57600);
  Serial.flush();
  dht.begin();
}

void loop() {
	String cmd;
    if (Serial.available() > 0) {
        cmd = Serial.readStringUntil('#');
        if (cmd == "ROOF_POSITION") {
        Serial.print(CurrentRoofPosition); Serial.print("#");
    }
    else if (cmd == "ROOF_STOP") RoofStop();
    else if (cmd == "ROOF_OPEN") RoofOpen();
    else if (cmd == "ROOF_CLOSE") RoofClose();
    else if (cmd == "LIGHT_ON") LightOn();
    else if (cmd == "LIGHT_OFF") LightOff();
    else if (cmd == "TEMPHUM_READ") TempHumRead();
    else if (cmd == "IRLED_OFF") IRLedOff();
    else if (cmd == "IRLED_AUTO") IRLedAuto();
        }
}

void RoofStop() {
    digitalWrite(6, HIGH);   // Turns ON Relay on digital pin 6, Relay02, STOP
    delay(250);              // wait for a 0,25 second to trigger motor (0,1 sec is too fast)
    digitalWrite(6, LOW);
    Serial.println("Roof STOP");
    delay(50);             // motor stops STOP in 0,3sec
}

void RoofOpen() {
    RoofStop();              // Always STOP before moving
    digitalWrite(5, HIGH);   // Turns ON Relay on digital pin 5, Relay03, OPEN
    delay(250);              // wait for a 0,25 second
    digitalWrite(5, LOW);
    Serial.println("Roof OPEN");
    delay(50);             // motor running Open direction, until RoofStop command or motor itself stops on limit switch
}

void RoofClose() {
    RoofStop();              // Always STOP before moving
    digitalWrite(7, HIGH);   // Turns ON Relay on digital pin 7, Relay01, CLOSE
    delay(250);              // wait for a 0,25 second
    digitalWrite(7, LOW);
    Serial.println("Roof CLOSE");
    delay(50);             // motor running Close direction, until RoofStop command or motor itself stops on limit switch
}

void LightOn() {
    digitalWrite(4, HIGH);   // Turns ON Light relay (like 12V for LED spotting light) on digital pin 4, Relay04
    Serial.println("Light relay ON");
    delay(50);             // short delay
}

void LightOff() {
    digitalWrite(4, LOW);   // Turns OFF Light relay (like 12V for LED spotting light) on digital pin 4, Relay04
    Serial.println("Light relay OFF");
    delay(50);             // short delay
}

void TempHumRead() {
    // Temperature
  float h = dht.readHumidity();
 float t = dht.readTemperature();
 // check if returns are valid, if they are NaN (not a number) then something went wrong!
 if (isnan(t) || isnan(h)) 
 {
   Serial.println("Failed to read from DHT");
 } 
 else 
 {
   Serial.print("Temp: "); 
   Serial.print(t);
   Serial.print(" C ");
   Serial.print("Hum: "); 
   Serial.print(h);
   Serial.println(" %\t");
   delay(50);             // short delay
}
}

void IRLedOff() {
    digitalWrite(3, HIGH);   // Turns ON red led, which is taped to cameras light sensor and turns off cameras IR light
    Serial.println("Camera IR Led OFF (Red Led On)");
    delay(50);             // short delay
}

void IRLedAuto() {
    digitalWrite(3, LOW);   // Turns OFF red led, which is taped to cameras light sensor and then camera IR is automatic
    Serial.println("Camera IR Led Auto (Red Led Off)");
    delay(50);             // short delay
}

void RoofPosition(int pos) {

    //do roof position algorithm
    delay(200);
    CurrentRoofPosition = pos;
    Serial.println("RoofPositionQuery TestMessage");
}
