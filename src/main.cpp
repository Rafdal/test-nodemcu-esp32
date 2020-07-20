#include <WiFi.h>
#include <HTTPClient.h>
#include <private_data.h> // Aqui tengo passwords de red

#define USE_SERIAL Serial

const char *host = "192.168.1.114"; 
const int port = 3000;

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

// #include <google-get-request.h>


#include <SocketIoClient.h>
SocketIoClient webSocket;

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

	webSocket.on("chat:escribiendo", [](const char * payload, size_t length){
		// ! Expresiones lambda en C++
		Serial.printf("Recibido payload: %s\n", payload);
	});
    webSocket.begin(host, port);
}

void loop()
{
	webSocket.loop();
	//Send an HTTP POST request every 10 minutes
	if ((millis() - lastTime) > timerDelay)
	{
		// GetGoogle();
	}
}