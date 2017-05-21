/*  Exempel på en web server man kan koppla upp sig mot och genom knappar på adressen
 *  192.168.4.1 sätta på och stänga av en LED-lampa ansluten till pin 16 på en ESP8266. 
 */
#include <ESP8266WiFi.h>

WiFiServer server(80); // Initialisera server på port 80.

const char* ssid = "YONO"; //SSID = namn på wifi:et
const char* password = ""; //Wifi:ets lösenord
int LED_PIN = 16;

void setup(){
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  WiFi.mode(WIFI_AP); //ESP8266 är en AP (Accesspunkt)
  WiFi.softAP(ssid, password); //ESP8266 accesspunkten får namnet ssid (variabel) och lösenordet password (variabel)
  server.begin(); //Start the HTTP server

  //LOOKING UNDER THE HOOD
  Serial.begin(115200); //Starta Seriell kommunikation med ESP8266. Hastighet: 115200
  IPAddress HTTPS_ServerIP = WiFi.softAPIP(); //HTTPS_ServerIP = variabel av typen IPAddress
  Serial.print("Server IP is: "); //Skriv ut ESp8266 IP (IP = datorers personnummer, unik för varje enhet)
  Serial.println(HTTPS_ServerIP);
}

void loop(){
  /* Kolla om någon enhet (klient/client (eng)) är anluten till servern
   * if ansluten - gör något
   * else - kolla igen i all oändlighet
   */
  WiFiClient client = server.available();
    if(!client) {
      return;
    }
    //Looking under the hood
    Serial.println("Browser Online!");

    /* Läs vad webläsaren har skickat som en sträng och skriv ut förfrågan till monitorn. 
     * Read what the browser has sent into a String class and print
       the request to the monitor. */
     String request = client.readStringUntil('/r');
     //Looking under the hood
     Serial.println(request); //skriv serverns förfrågan (request) i den seriella monitorn. Öppnas med Ctrl+shift+m

     //Förbered HTML-koden och lägg till knappar
     String s = "HTTP/1.1 200 OK\r\n";
     s += "Content-Type: text/html\r\n\r\n";
     s += "<!DOCTYPE HTML\r\n<html>\r\n";
     
     //En knapp med namnet b1 av typen input (skickar information). Knappen har Turn LED ON skrivet på sig. Skickar meddelandet i request (förfrågan). 
     s += "<br><input type=\"button\" name=\"b1\" value=\"Turn LED ON\" onclick=\"location.href='/meddelandet'\">";
     
     //3 st nya rader innan nästa knapp
     s += "<br><br><br>";
     
     //En knapp med namnet b1 av typen input (skickar information). Knappen har Turn LED OFF skrivet på sig. Skickar OFF i request (förfrågan). 
     s += "<input type=\"button\" name=\"b1\" value=\"Turn LED OFF\" onclick=\"location.href='/OFF'\">";
     s += "</html>\n";

     if(request.indexOf("/meddelandet") != -1){//Om någon del av request innehåller /meddelandet 
      digitalWrite(LED_PIN, HIGH);             // <-- Så görs detta.
     }
     else if(request.indexOf("/OFF") != -1){//Annars om meddelandet innehållet /OFF
      digitalWrite(LED_PIN,LOW);            // <-- Så görs detta.
     }

     
     client.flush(); //Rensa infon från innan i dataströmmen
     client.print(s); //Skicka HTML dokumentet till webläsaren.
     delay(1);
     Serial.println("Client disconnected");
}

