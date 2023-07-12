#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>

const char* ssid = "project";
const char* password = "";
const char* serverName = "http://137.184.191.52:3000/";

void setup() {
  Serial.begin(9600);

  // Peripheral Connections
  pinMode(D2, INPUT); // Flame Sensor
  pinMode(D4, OUTPUT);// Buzzer
  pinMode(D7, INPUT); // IR Sensor
  pinMode(A0, INPUT); // Air Quality Sensor

  // Establish WiFi Connection
  WiFi.begin(ssid,password);
	// WiFi.status() returns 7 when not connected and 3 when connected
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.println("..");
	}
	Serial.println("");
	Serial.print("Connected to WiFi network with IP Address: ");
	Serial.println(WiFi.localIP());
}

void loop() {
  // Reading the Sensor Values
  int flame = digitalRead(D2);
  int ir = digitalRead(D7);
  float airQuality = analogRead(A0)/1023*100;


  // Condition Check
  if (flame == 0 || airQuality != 100 || ir == 0){
    // Flame is Detected *OR* Air Quality has Decreased *OR* IR Sensor detected Movement 

    // Play sounds on the Buzzer
    tone(D4, 523);
    delay(1000);
    tone(D4, 587);
    delay(1000);
    tone(D4, 659);
    delay(1000);
    tone(D4, 783);
    delay(1000);
    digitalWrite(D4, LOW);

    // POST Request to the Server
    if(WiFi.status()== WL_CONNECTED){

		  WiFiClient client;
	    HTTPClient http;

		  http.begin(client, serverName);
		  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

		  String data = "flame=";
      data.concat(flame);
      data.concat("&ir=");
      data.concat(ir);
      data.concat("&airQuality=");
      data.concat(airQuality);

		  int httpResponseCode = http.POST(data);
		  Serial.print("HTTP Response code: ");
	    Serial.println(httpResponseCode);
	    http.end();

	  } else {
		Serial.println("WiFi Disconnected");
	  }
    

  // No Change in Sensor Values
  } else {
    Serial.println("No Change");
  }

  delay(2000);
}
