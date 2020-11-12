/*-------------LIBERIAS-------------*/
#include <ArduinoJson.h> //Libería para codificacion y decodificacion de mensajes JSON
#include <WiFi.h> //Libería para conectarse a red WiFi
#include <HTTPClient.h> //Libería que permite enviar peticiones POST y GET
#define baudrate 115200 //definimos un baudrate
int pinLED = 2;

/*-----------VARIABLES DE RED----------*/
//const char* ssid = "Huawei-HG8245-A721"; //Nombre de la red a la que se conectará la ESP32
//const char* password = "KV95MA97CE01"; //Password de la red a la que nos conectaremos
const char* ssid = "INFINITUME096_2.4";
const char* password = "4509527228";

/*-----------VARIABLES DE JSON Y SERVIDOR-----------*/
String NIP = ""; //String donde guardamos los datos recibidos y enviamos al server
StaticJsonBuffer<400> jsonBuffer; //Buffer asignado al mensaje JSON recibido
/*-----VARIABLES PARA ENVIO DE DATOS A QT---------*/
int nip;
const char* NOMBRE;
float A_MENOS, A_MAS, B_MENOS, B_MAS, C_MENOS, C_MAS;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciamos puertos seriales");

  while (!Serial) {
    ; //Esperamos a un puerto serial para conectar
  }
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
  Serial2.begin(baudrate);

  /*--------CONEXION A WIFI--------------*/
  WiFi.begin(ssid,password); //Conectamos a la red
  Serial.println("Conectando....");
  while(WL_CONNECTED != WiFi.status()){
    Serial.println("Esperando conexion");
    delay(300);
  }
}

void loop() {
  if(WL_CONNECTED == WiFi.status()){
    SerialNIP();
  }
}
