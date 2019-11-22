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

#define STASSID "GT_TeamKairos"
#define STAPSK  "$kair0s2019"

const char* ssid     = STASSID;
const char* password = STAPSK;
////////////////////////PIN//PARA//EL//SENSOR//DE//PMSA003////////////////////////////////////////
#include "PMS.h"
PMS pms(Serial);
PMS::DATA data;

float medidaPM_2_5 = 0;
unsigned long previousMillis_2 = 0;
////////////////////PARA//EL//SERVER/NTP////////////////////////////////////////////////////
const long utcOffsetInSeconds = -18000;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "south-america.pool.ntp.org", utcOffsetInSeconds);
time_t t;
String fecha;
////////////////////////////////////////////////////////////////////////////////////////////


void setup()
{
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  String chip_id = String(ESP.getChipId());
  Serial.println(chip_id);
  Serial.println("Iniciando PMSA 003");
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

  timeClient.begin();
  setTime(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), 21, 11, 2019);
  timeClient.update();
}

void loop() {
  t = now();//Declaramos la variable time_t t
  if (pms.read(data))
  {
    unsigned long currentMillis_2 = millis();
    if (currentMillis_2 - previousMillis_2 >= 4000) { //Definimos el tiempo de envio
      previousMillis_2 = currentMillis_2;

      medidaPM_2_5 = data.PM_AE_UG_2_5;
      Serial.println("------------------------------------------------------------------------------");
      Serial.print("PM 2.5 (ug/m3): ");
      Serial.print(medidaPM_2_5);
      Serial.print("\t");
      Serial.print("AQI PM 2.5:  ");
      Serial.print(calcularAQIPM25(medidaPM_2_5));
      Serial.print("\t");
      Serial.println(getRealTime());
      Serial.println("------------------------------------------------------------------------------");

    }
  }
}

static double calcularAQIPM25(double concentration) {

  double conc = concentration;
  double c;
  double AQI;
  c = floor(10 * conc) / 10;

  if (c >= 0 && c < 12.1) {
    AQI = Linear(50, 0, 12, 0, c);
  } else if (c >= 12.1 && c < 35.5) {
    AQI = Linear(100, 51, 35.4, 12.1, c);
  } else if (c >= 35.5 && c < 55.5) {
    AQI = Linear(150, 101, 55.4, 35.5, c);
  } else if (c >= 55.5 && c < 150.5) {
    AQI = Linear(200, 151, 150.4, 55.5, c);
  } else if (c >= 150.5 && c < 250.5) {
    AQI = Linear(300, 201, 250.4, 150.5, c);
  } else if (c >= 250.5 && c < 350.5) {
    AQI = Linear(400, 301, 350.4, 250.5, c);
  } else if (c >= 350.5 && c < 500.5) {
    AQI = Linear(500, 401, 500.4, 350.5, c);
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
