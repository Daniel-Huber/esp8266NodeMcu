/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>

const char* ssid = "30cm slak"; //SSID = namnet på wifi att ansluta sig till
const char* password = "12345678";// Lösenordet wifi AP:en har. (AP = accesspunkt)

// Skapa en instans av servern.
// Specifiera porten esp:en ska ta emot instruktioner genom.
WiFiServer server(80);

void setup() {
  //Starta serial anslutning, Öppna med ctrl+shift+m
  Serial.begin(115200);
  delay(10);

  // Förbered GPIO (general pin input/output)
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);
  
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password); // Anslut till wifi nätverk
  
  while (WiFi.status() != WL_CONNECTED) { //Om ej ansluten skriv ut punkter i monitorn Ctrl+Shift+m
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); //Ansluten
  
  // Starta server
  server.begin();
  Serial.println("Server started");

  // Skriv ut ip-adressen att anluta till i webläsaren
  Serial.println(WiFi.localIP());
}

void loop() {
  //Kolla om klient har anslutit sig
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Vänta tills klient skickat data (något har hänt på hemsidan; knapp har tryckts på etc..)
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Läs första raden av förfrågan.
  //Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  // Bestäm vad som ska hända beroende på vad förfrågan säger
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;

  //Går att lägga till fler else if här...

  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  // Sätt pin nr 2 till 1 eller 0 beroende på förfrågan.
  digitalWrite(D4, val);
  
  client.flush(); //Spola bort informationen som clienten hade innan.

  // Prepare the response
  // Förbered svaret
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  // Skicka svaret (responsen) till klienten
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected when the function returns and 'client' object is detroyed
  // Klienten kommer att koppla ifrån och när funktionen returnerar kommer 'klient' objektet att förstöras.
  //      Alltid ett nytt client objekt per kommando..
}

