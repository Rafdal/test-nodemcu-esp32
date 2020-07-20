// Usado para testing

//Check WiFi connection status
void GetGoogle()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(F("http://www.google.com/"));

        Serial.println(F("GET to google.com"));

        int httpCode = http.GET(); // Make the request

        if (httpCode > 0) { //Check for the returning code

            String payload = http.getString();
            Serial.println(httpCode);
            // Serial.println(payload);
        }
        else {
        Serial.println(F("Error on HTTP request"));
        Serial.println(httpCode);

        }

        http.end(); //Free the resources
    }
    else
    {
        Serial.println(F("WiFi Disconnected"));
    }
    lastTime = millis();
}