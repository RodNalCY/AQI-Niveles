#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <math.h>
/*sdafasd
   double x
   x = round(2.5)
   x = floor(10 * 7)/10
*/
//////////////////////////WIFI//CREDENCIALES///////////////////////////////////////////////////
#define STASSID "GT_TeamKairos"
#define STAPSK  "$kair0s2019"
const char* ssid     = STASSID;
const char* password = STAPSK;
////////////////////////PIN//PARA//EL//SENSOR//DE//CO////////////////////////////////////////
#include "MQ7.h"
MQ7 pinMQ(A0, 5.0);
float medidaCO = 0;
unsigned long previousMillis_2 = 0;
////////////////////PARA//EL//SERVER/NTP////////////////////////////////////////////////////
const long utcOffsetInSeconds = -18000;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "south-america.pool.ntp.org", utcOffsetInSeconds);
time_t t;
String fecha;
////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  /////////////////////////////////////////////////////////////////////////////////////////////
  Serial.begin(9600);
  Serial.print("Node MCU Chip Id : ");
  String chip_id = String(ESP.getChipId());
  Serial.println(chip_id);
  Serial.print("'Conectando Modulo' a la RED WiFi espere por favor (...) : ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("Mi direccion IP: ");
  Serial.println(WiFi.localIP());
  /////////////////////////////////////////////////////////////////////////////////////////////

  Serial.println("PPM CO to AQI");

  timeClient.begin();
  setTime(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), 21, 11, 2019);
  timeClient.update();

}

void loop() {
  t = now();//Declaramos la variable time_t t
  unsigned long currentMillis_2 = millis();
  if (currentMillis_2 - previousMillis_2 >= 2000) {
    previousMillis_2 = currentMillis_2;
    medidaCO = pinMQ.getPPM(); //Lemos la salida analógica del MQ
    
    Serial.println("------------------------------------------------------------------------------");
    Serial.print("PPM CO:  ");
    Serial.print(medidaCO);
    Serial.print("\t");
    Serial.print("AQI CO:  ");
    Serial.print(calcularCO(medidaCO));
    Serial.print("\t");
    Serial.println(getRealTime());
    Serial.println("------------------------------------------------------------------------------");


  }
}
String getRealTime() {
  //2019-07-23 21:48:01
  String anio = String(year(t));
  String mes  = String(month(t));
  String dia  = String(day(t));
  String hora = timeClient.getFormattedTime();
  String fecha =  anio + "/" + mes + "/" + dia + " " + hora;
  fecha.trim();
  //delay(1000);
  return fecha;
}
static double calcularCO(double concentration) {

  double conc = concentration;
  double c;
  double AQI;
  c = floor(10 * conc) / 10;

  if (c >= 0 && c < 4.5) {
    AQI = Linear(50, 0, 4.4, 0, c);
  } else if (c >= 4.5 && c < 9.5) {
    AQI = Linear(100, 51, 9.4, 4.5, c);
  } else if (c >= 9.5 && c < 12.5) {
    AQI = Linear(150, 101, 12.4, 9.5, c);
  } else if (c >= 12.5 && c < 15.5) {
    AQI = Linear(200, 151, 15.4, 12.5, c);
  } else if (c >= 15.5 && c < 30.5) {
    AQI = Linear(300, 201, 30.4, 15.5, c);
  } else if (c >= 30.5 && c < 40.5) {
    AQI = Linear(400, 301, 40.4, 30.5, c);
  } else if (c >= 40.5 && c < 50.5) {
    AQI = Linear(500, 401, 50.4, 40.5, c);
  } else {
    AQI = 1000;
  }
  return AQI;

}
static double Linear(double AQIhigh, double AQIlow, double Conchigh, double Conclow, double concentration) {
  double linear;
  double conc = concentration;
  double a;
  a = (conc - Conclow) / (Conchigh - Conclow) * (AQIhigh - AQIlow) + AQIlow;
  linear = round(a);
  return linear;
}
