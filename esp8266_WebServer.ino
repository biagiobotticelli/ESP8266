#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "Telecom-52227471";
const char* password = "biagiobotticelliaureliacasalini";

int pinRELAY = 12;

WiFiServer server(80);

void setup(void){
  Serial.begin(115200);
  delay(10);
  
  pinMode(pinRELAY, OUTPUT);

  // Connect to WiFi network
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("HTTP server started!");

  // Print the IP address
  Serial.println("");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.println("");
}
 
void loop(void){
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("New client connection...");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.print("Request: ");
  Serial.println(request);
  client.flush();
   
  // Match the request
  int value = LOW;
  if (request.indexOf("/LIGHT=ON") != -1) {
    digitalWrite(pinRELAY, HIGH);
    value = HIGH;
  } 
  if (request.indexOf("/LIGHT=OFF") != -1){
    digitalWrite(pinRELAY, LOW);
    value = LOW;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
   
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<FONT face=Helvetica>");
  client.println("<h1>ESP8266 Web Client</h1>");
  client.println("<h4> Relay <a href=\"/LIGHT=ON\"><button>ON</button></a>&nbsp;<a href=\"/LIGHT=OFF\"><button>OFF</button></a></h4>");
  client.println("");
  client.print("The light is now: ");
  if(value == HIGH) {
    client.print("<strong>On</strong>");  
  } else {
    client.print("<strong>Off</strong>");
  }
  client.println("</FONT></html>");
 
  delay(1);
  Serial.println("DONE!");
  Serial.println("");
}
