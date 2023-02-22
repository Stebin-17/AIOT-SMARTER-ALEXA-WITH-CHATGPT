/*
  Basic MQTT example

  This sketch demonstrates the basic capabilities of the library.
  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>




//We always have to include the library
# include "LedControl.h"

const static byte tablaCaracteresMayuscula [] PROGMEM  = {
  B00000000, B11111100, B11111110, B00010010, B00010010, B11111110, B11111100, B00000000,
  B00000000, B11111110, B11111110, B10010010, B10010010, B11111110, B01101100, B00000000,
  B00000000, B01111100, B11111110, B10000010, B10000010, B11000110, B01000100, B00000000,
  B00000000, B11111110, B11111110, B10000010, B10000010, B11111110, B01111100, B00000000,
  B00000000, B11111110, B11111110, B10010010, B10010010, B10010010, B10000010, B00000000,
  B00000000, B11111110, B11111110, B00010010, B00010010, B00010010, B00000010, B00000000,
  B00000000, B01111100, B11111110, B10000010, B10100010, B11100110, B01100100, B00000000,
  B00000000, B11111110, B11111110, B00010000, B00010000, B11111110, B11111110, B00000000,
  B00000000, B00000000, B10000010, B11111110, B11111110, B10000010, B00000000, B00000000,
  B00000000, B01100000, B11100000, B10000010, B11111110, B01111110, B00000010, B00000000,
  B00000000, B11111110, B11111110, B00111000, B01101100, B11000110, B10000010, B00000000,
  B00000000, B11111110, B11111110, B10000000, B10000000, B10000000, B10000000, B00000000,
  B00000000, B11111110, B11111110, B00001100, B00011000, B00001100, B11111110, B11111110,
  B00000000, B11111110, B11111110, B00001100, B00011000, B00110000, B11111110, B11111110,
  B00000000, B01111100, B11111110, B10000010, B10000010, B11111110, B01111100, B00000000,
  B00000000, B11111110, B11111110, B00100010, B00100010, B00111110, B00011100, B00000000,
  B00000000, B00111100, B01111110, B01000010, B01100010, B11111110, B10111100, B00000000,
  B00000000, B11111110, B11111110, B00110010, B01110010, B11011110, B10001100, B00000000,
  B00000000, B01001100, B11011110, B10010010, B10010010, B11110110, B01100100, B00000000,
  B00000000, B00000110, B00000010, B11111110, B11111110, B00000010, B00000110, B00000000,
  B00000000, B01111110, B11111110, B10000000, B10000000, B11111110, B11111110, B00000000,
  B00000000, B00111110, B01111110, B11000000, B11000000, B01111110, B00111110, B00000000,
  B00000000, B11111110, B11111110, B01100000, B00110000, B01100000, B11111110, B11111110,
  B00000000, B11000110, B11101110, B00111000, B00010000, B00111000, B11101110, B11000110,
  B00000000, B00001110, B00011110, B11110000, B11110000, B00011110, B00001110, B00000000,
  B00000000, B11000010, B11100010, B10110010, B10011010, B10001110, B10000110, B00000000
};

const static byte tablaCaracteresMinuscula [] PROGMEM  = {
  B00000000, B01000000, B11101000, B10101000, B10101000, B11111000, B11110000, B00000000,
  B00000000, B11111110, B11111110, B10010000, B10010000, B11110000, B01100000, B00000000,
  B00000000, B01110000, B11111000, B10001000, B10001000, B11011000, B01010000, B00000000,
  B00000000, B01100000, B11110000, B10010000, B10010000, B11111110, B11111110, B00000000,
  B00000000, B01110000, B11111000, B10101000, B10101000, B10111000, B00110000, B00000000,
  B00000000, B00100000, B11111100, B11111110, B00100010, B00100110, B00000100, B00000000,
  B00000000, B00011000, B10111100, B10100100, B10100100, B11111100, B01111100, B00000000,
  B00000000, B11111110, B11111110, B00010000, B00010000, B11110000, B11100000, B00000000,
  B00000000, B00000000, B10000000, B11110100, B11110100, B10000000, B00000000, B00000000,
  B00000000, B01100000, B11100000, B10000000, B11111010, B01111010, B00000000, B00000000,
  B00000000, B11111110, B11111110, B00100000, B01110000, B11011000, B10001000, B00000000,
  B00000000, B00000000, B00000000, B11111110, B11111110, B00000000, B00000000, B00000000,
  B00000000, B11111000, B11111000, B00110000, B11100000, B00110000, B11111000, B11111000,
  B00000000, B11111000, B11111000, B00011000, B00011000, B11111000, B11110000, B00000000,
  B00000000, B01110000, B11111000, B10001000, B10001000, B11111000, B01110000, B00000000,
  B00000000, B11111100, B11111100, B00100100, B00100100, B00111100, B00011000, B00000000,
  B00000000, B00011000, B00111100, B00100100, B11111100, B11111100, B10000000, B11000000,
  B00000000, B11111000, B11111000, B00001000, B00001000, B00111000, B00110000, B00000000,
  B00000000, B10010000, B10101000, B10101000, B10101000, B10101000, B01001000, B00000000,
  B00000000, B00010000, B00010000, B11111100, B11111100, B00010000, B00010000, B00000000,
  B00000000, B01111000, B11111000, B10000000, B10000000, B11111000, B11111000, B00000000,
  B00000000, B00110000, B01110000, B11000000, B11000000, B01110000, B00110000, B00000000,
  B00000000, B01111000, B11111000, B10000000, B11110000, B10000000, B11111000, B01111000,
  B00000000, B10001000, B11011000, B01110000, B01110000, B11011000, B10001000, B00000000,
  B00000000, B00011000, B10111000, B10100000, B10100000, B11111000, B01111000, B00000000,
  B00000000, B00000000, B11001000, B11101000, B10111000, B10011000, B00000000, B00000000
};

const static byte tablaNumeros [] PROGMEM  = {
  B00000000, B01111100, B11111110, B10010010, B10001010, B11111110, B01111100, B00000000,
  B00000000, B10000000, B10001000, B11111110, B11111110, B10000000, B10000000, B00000000,
  B00000000, B11000100, B11100110, B10100010, B10010010, B10011110, B10001100, B00000000,
  B00000000, B01000100, B11000110, B10010010, B10010010, B11111110, B01101100, B00000000,
  B00000000, B00110000, B00101000, B00100100, B11111110, B11111110, B00100000, B00000000,
  B00000000, B01001110, B11001110, B10001010, B10001010, B11111010, B01110010, B00000000,
  B00000000, B01111100, B11111110, B10010010, B10010010, B11110110, B01100100, B00000000,
  B00000000, B00000110, B00000110, B11100010, B11111010, B00011110, B00000110, B00000000,
  B00000000, B01101100, B11111110, B10010010, B10010010, B11111110, B01101100, B00000000,
  B00000000, B01001100, B11011110, B10010010, B10010010, B11111110, B01111100, B00000000
};


const int ledPin = 14;




// variable for reading the pushbutton status

/*
  Now we need a LedControl to work with.
 ** These pin numbers will probably not work with your hardware **
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
  We have only a single MAX72XX.
*/
// LedControl lc=LedControl(12,11,10,1);
LedControl lc = LedControl(11, 10, 13, 4);

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;

void writeChar(char caracter, int posicion)
{
  int posicion_caracter;
  // por defecto un asterisco
  //uint8_t codigocaracter[]= {B00000000, B00001000, B00101010, B00011100, B01110111, B00011100, B00101010, B00001000};
  uint8_t codigocaracter[] = {0, 0, 28, 62, 124, 62, 28, 0};

  if (caracter >= 'A' && caracter <= 'Z') {
    posicion_caracter = ((int)caracter - 'A') * 8;
    for (int i = 0; i < 8; i++) {
      codigocaracter[i] = pgm_read_byte_near(tablaCaracteresMayuscula + i + posicion_caracter);
    }
  }
  else if (caracter >= 'a' && caracter <= 'z') {
    posicion_caracter = ((int)caracter - 'a') * 8;
    for (int i = 0; i < 8; i++) {
      codigocaracter[i] = pgm_read_byte_near(tablaCaracteresMinuscula + i + posicion_caracter);
    }
  }
  else if (caracter >= '0' && caracter <= '9') {
    posicion_caracter = ((int)caracter - '0') * 8;
    for (int i = 0; i < 8; i++) {
      codigocaracter[i] = pgm_read_byte_near(tablaNumeros + i + posicion_caracter);
    }
  }
  else if (caracter == ' ') {
    for (int i = 0; i < 8; i++) {
      codigocaracter[i] = 0;
    }
  }

  int maxdev = lc.getDeviceCount();
  for (int i = 0; i < 8; i++) {
    int address = 0;
    int posendisplay = posicion + i;
    while (posendisplay > 7) {
      address++;
      posendisplay -= 8;
    }
    if (address > lc.getDeviceCount() - 1 )
      return;
    lc.setColumn((maxdev - address - 1), posendisplay, codigocaracter[(7 - i)]);
  }
}

void writeStr(const char* frase, int posicion) {
  size_t len = strlen(frase);
  for (size_t i = 0; i < len; i++) {
    writeChar(frase[len - i - 1], (i * 8) + posicion);
  }
}

void writeStrScroll(char* frase, unsigned long pausa) {
  int inicio = lc.getDeviceCount() * 8;
  int npasos = strlen(frase) * 8;
  for (int i = -npasos + 1; i <= inicio; i++) {
    writeStr(frase, i);
    delay(pausa);
  }
}


// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0x00, 0xED };
IPAddress ip(192, 168, 0, 102);
IPAddress server(54, 87, 92, 106);
IPAddress myDns(192, 168, 0, 1);

//void callback(char* topic, byte* payload, unsigned int length) {
//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("] ");
//  for (int i = 0; i < length; i++) {
//    char receivedChar = (char)payload[i];
//    Serial.print(receivedChar);
//    writeStrScroll(payload[i], 150);
//    if (receivedChar == '0')
//      digitalWrite(chartsPin, HIGH);
//    if (receivedChar == '1')
//      digitalWrite(chartsPin, LOW);
//  }
//  Serial.println();
//}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char incomingMessage[length + 1];
  for (int i = 0; i < length; i++) {
    incomingMessage[i] = (char)payload[i];
  }
  incomingMessage[length] = '\0';

  Serial.println(incomingMessage);

  if (strcmp(incomingMessage, "TURN ON LIGHTS") == 0) {
    digitalWrite(ledPin, HIGH);
    Serial.println("POWER ON");

  }
  else {
    digitalWrite(ledPin, LOW);
    Serial.println("POWER OFF");
  }

  writeStrScroll(incomingMessage, 25);
}



//
//
//  if(incomingMessage.indexOf("TURN ON LIGHTS") > 0 {
//    Client.publish("home/alexa-reply","turning on the lights")
//  }
//  if(incomingMessage.indexOf("TURN OFF LIGHTS") > 0 {
//    Client.publish("home/alexa-reply","turning off the lights")
//  }
//


EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("home/alexa");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  Ethernet.init(17);  // WIZnet W5100S-EVB-Pico W5500-EVB-Pico W6100-EVB-Pico
  //we have to init all devices in a loop
  for (int address = 0; address < lc.getDeviceCount(); address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address, 2);
    /* and clear the display */
    lc.clearDisplay(address);
  }
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  client.setServer(server, 1883);
  client.setCallback(callback);

  // give the Ethernet shield a second to initialize:
  delay(1500);
}

void loop()
{


  {
    if (!client.connected()) {
      reconnect();
    }

    client.loop();
  }
}
