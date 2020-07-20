#include <WiFi.h>
#include <HTTPClient.h>
#include <private_data.h>

//Your Domain name with URL path or IP address with path
const char *serverName = "http://192.168.1.114:3000/";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 15000;

// #include <google-get-request.h>

void setup()
{
	Serial.begin(576000);

	 WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.println(F("Connecting"));
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.print(F("\nConnected to WiFi network with IP Address: "));
	Serial.println(WiFi.localIP());

	Serial.println(F("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading."));
	lastTime = millis();
}

void loop()
{
	//Send an HTTP POST request every 10 minutes
	if ((millis() - lastTime) > timerDelay)
	{
		// GetGoogle();
	}
}