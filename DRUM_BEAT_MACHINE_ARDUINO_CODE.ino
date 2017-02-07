#include <ESP8266WiFi.h>
#include <Servo.h>

#define LED_PIN LED_BUILTIN

Servo servo_1;
const int num_notes = 100;
bool reset = true;
char note[num_notes];
int cnt_note = -1;

WiFiServer server(80); //Initialize the server on Port 80

bool start_MSC(char* music_sheet)
{
  Serial.println("Music is starting!");
  Serial.println("Music Sheet: ");
  
  for(int x = 0; x < num_notes; x++)
  {
    Serial.print(music_sheet[x]);
    switch(music_sheet[x])
    {
      case '.':
      servo_1.attach(D5);
      servo_1.write(180);
      delay(200);
      servo_1.write(0);
      delay(150);
      servo_1.write(90);
      servo_1.detach();
      break;
      case '-':
      servo_1.attach(D5);
      servo_1.write(180);
      delay(200);
      servo_1.write(90);
      delay(500);
      servo_1.write(0);
      delay(150);
      servo_1.write(90);
      servo_1.detach();
      break;
      case '*':
      servo_1.attach(D5);
      servo_1.write(180);
      delay(200);
      servo_1.write(90);
      delay(1000);
      servo_1.write(0);
      delay(150);
      servo_1.write(90);
      servo_1.detach();
      break;
      default:
      break;
    }
  }
  Serial.println("");
  return true;
  }

void setup() {

Serial.begin(74880); //Start communication between the ESP8266-12E and the monitor window
WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
WiFi.softAP("Hello_IoT", "Laziboy6694"); // Provide the (SSID, password); . 
server.begin(); // Start the HTTP Server

Serial.write("server is beginning");

IPAddress HTTPS_ServerIP = WiFi.softAPIP(); // Obtain the IP of the Server
Serial.print("Server IP is: "); // Print the IP to the monitor window 
Serial.println(HTTPS_ServerIP);
}

void loop()
{
  if(reset)
  {
  for(int i = 0; i < num_notes; i++)
  {
    note[i] = 'O';
  }
  reset = false;
  }
  
WiFiClient client = server.available();
if (!client) { 
return; 
} 
//Looking under the hood 
Serial.println("Somebody has connected!");

//Read what the browser has sent into a String class and print the at to the monitor
String request = client.readStringUntil('\r'); 
//Looking under the hood 
Serial.println(request);

// Prepare the HTML document to respond and add buttons:

String s = "HTTP/1.1 200 OK\r\n";
s+="Content-Type: text/html\r\n\r\n";
s+="<!DOCTYPE HTML>\r\n<html>\r\n";
s+="<input type = \"button\" name =\"b1\" value = \" quarter note\" style =\"height:500px; width: 500px\"";
s+="onclick=\"location.href='/quarter'\"> ";
s+="<input type = \"button\" name =\"b1\" value = \" half note\" style =\"height:500px; width: 500px\"";
s+="onclick=\"location.href='/half'\"> ";
s+="<input type = \"button\" name =\"b1\" value = \" whole note\" style =\"height:500px; width: 500px\"";
s+="onclick=\"location.href='/whole'\"> ";
s+="<br><br><br>";
s+="<input type = \"button\" name =\"b1\" value = \" start music\" style =\"height:500px; width: 500px\"";
s+="onclick=\"location.href='/START'\">";
s+="<input type = \"button\" name =\"b1\" value = \" reset music\" style =\"height:500px; width: 500px\"";
s+="onclick=\"location.href='/RESET'\">";
s+="</html>\n";

Serial.println(s);

//Serve the HTML document to the browser.

client.flush(); //clear previous info in the stream 
client.print(s); // Send the response to the client 
delay(1); 
Serial.println("Client disconnected"); //Looking under the hood

if(request.indexOf("/quarter")!= -1)
{
  if(cnt_note != num_notes)
  {
    note[cnt_note + 1] = '.';
     Serial.println("YOU PRESSED . !");
    cnt_note++;
  }
  else
  {
    Serial.println("Over the limit of beats!");
  }
  
}
else if(request.indexOf("/half") != -1)
{
    if(cnt_note != num_notes)
  {
    note[cnt_note + 1] = '-';
     Serial.println("YOU PRESSED - !");
    cnt_note++;
  }
  else
  {
    Serial.println("Over the limit of beats!");
  }
}
else if (request.indexOf("/whole") != -1)
{
      if(cnt_note != num_notes)
  {
    note[cnt_note + 1] = '*';
    Serial.println("YOU PRESSED * !");
    cnt_note++;
  }
  else
  {
    Serial.println("Over the limit of beats!");
  }
}
else if (request.indexOf("/START") != -1)
{
  start_MSC(note);
}
else if (request.indexOf("/RESET") != -1)
{ 
  cnt_note = -1;
  for(int x = 0; x < num_notes; x++)
  {
  note[x] = 'O';
  }
  Serial.println("THE MUSIC PIECE IS RESET!");
}

}

