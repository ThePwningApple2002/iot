#include <Arduino.h>
#include <WiFi.h>
#include <WiFiSSLClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

const char* ssid = "Stanimirovic Household 2.4Ghz";
const char* password = "mgmivela";
const char* openaiApiKey = "api";

WiFiSSLClient sslClient;
HttpClient httpClient = HttpClient(sslClient, "api.openai.com", 443);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi network");
}

void loop() {
  Serial.println("Ask your question:");
  String question;
  while (!Serial.available()) {
    delay(100);
  }
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      continue;
    }
    question += c;
  }

  if (!question.isEmpty()) {
    Serial.println("Sending question to OpenAI: " + question);

    DynamicJsonDocument jsonDoc(1024);
    jsonDoc["model"] = "gpt-3.5-turbo";
    jsonDoc["temperature"] = 0.7;
    JsonArray messages = jsonDoc.createNestedArray("messages");
    JsonObject messageObj = messages.createNestedObject();
    messageObj["role"] = "user";
    messageObj["content"] = question;

    String requestBody;
    serializeJson(jsonDoc, requestBody);

    httpClient.beginRequest();
    httpClient.post("/v1/chat/completions");
    httpClient.sendHeader("Content-Type", "application/json");
    httpClient.sendHeader("Authorization", "Bearer " + String(openaiApiKey));
    httpClient.sendHeader("Content-Length", requestBody.length());
    httpClient.beginBody();
    httpClient.print(requestBody);
    httpClient.endRequest();

    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();
    
    DynamicJsonDocument jsonDoci(2048); 
    deserializeJson(jsonDoci, response);

    JsonArray choices = jsonDoci["choices"].as<JsonArray>();
    for (JsonObject choice : choices) {
        
        JsonObject message = choice["message"].as<JsonObject>();
        if (message["role"].as<String>() == "assistant") {
            answer = message["content"].as<String>();
            break; 
        }
    }

    
    Serial.println("Answer from OpenAI: " + answer);

    
    delay(10000);   
  }

}
