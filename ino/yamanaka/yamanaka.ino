#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <NewPing.h>
#include <Ticker.h>
#include <TinyGPS++.h>
#include "DHT.h"
#include "oled-imgs.h"

#define led 2

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTPIN 23
#define DHTTYPE DHT22

#define NEO_RX 17
#define NEO_TX 16
#define NEO_BAUD 115200

#define HC_TR1 14         // TOP
#define HC_EC1 12
#define HC_TR2 27         // LEFT
#define HC_EC2 13
#define HC_TR3 2          // BOTTOM
#define HC_EC3 4
#define HC_TR4 18         // RIGHT
#define HC_EC4 19
#define HC_MAX 200
#define HC_MIN 20
#define HC_PRINT 150

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

String titles[] = {
  " ",
  "PRESION         <hPa>",
  "HUM. Y TEMP.  <% | C>",
  "ACEL.     <X | Y | Z>",
  "HORA       <DD/MM/YY>",
  "LATITUD           <N>",
  "LONGITUD          <W>",
  "ALTITUD           <M>",
}, values[] = {
  "S E N S\nI A C T",
  "0000.00",
  "00.00 %\n00.00 C",
  "-00.00\n-00.00\n-00.00",
  "HH:MM",
  "-000'00\n00.00\"",
  "-000'00\n00.00\"",
  "0000",
};

long printTime = 0, cacheTime = 0;
bool printing = true;

byte v_sizes[] = { 3, 3, 3, 2, 4, 3, 3, 3, 3 };
byte i = 0;
short timezone = -6;
int dist[4] = { 0, 0, 0, 0 };

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_MPU6050 imu;
DHT dht(DHTPIN, DHTTYPE);
NewPing ultra1(HC_TR1, HC_EC1, HC_MAX);
NewPing ultra2(HC_TR2, HC_EC2, HC_MAX);
NewPing ultra3(HC_TR3, HC_EC3, HC_MAX);
NewPing ultra4(HC_TR4, HC_EC4, HC_MAX);
TinyGPSPlus gps;
HardwareSerial neo(2);
Adafruit_BMP280 bmp;

Ticker printTimer;
Ticker imuTimer;
Ticker dhtTimer;
Ticker sonarTimer;
Ticker bmpTimer;
Ticker gpsTimer;

void glitch(int dtime)
{
  display.clearDisplay();
  display.drawBitmap(0, 0, glitch1, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.display();
  delay(dtime / 6);
  display.clearDisplay();
  display.drawBitmap(0, 0, glitch2, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.display();
  delay(dtime / 6);
  display.clearDisplay();
  display.drawBitmap(0, 0, glitch3, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.display();
  delay(dtime / 6);
  display.clearDisplay();
  display.drawBitmap(0, 0, glitch4, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.display();
  delay(dtime / 6);
  display.clearDisplay();
  display.drawBitmap(0, 0, glitch5, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.display();
  delay(dtime / 6);
  display.clearDisplay();
  display.drawBitmap(0, 0, glitchA, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.display();
  delay(dtime / 6);
}

void print(String title, byte t_size, String value, byte v_size, int transition)
{
  glitch(transition);
  display.clearDisplay();

  if (title != "none") 
  {
    display.setTextSize(t_size);
    display.setCursor(0, 0);
    display.println(title);
  
    display.setTextSize(v_size);
    display.setCursor(0, 10);
    display.println(value);
  }
  else
  {
    display.setTextSize(v_size);
    display.setCursor(0, 0);
    display.println(value);
  }

  display.display();
}

void alert(bool flag)
{
  display.clearDisplay();
  display.cp437(true);
  if (flag)
    display.drawBitmap(0, 0, warning, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.setTextSize(1);
  
  for (int i = 0; i < 4; i++)
  {
    if (dist[i] < HC_MIN)
    {
      if (i == 0)
      {
        display.setCursor(0, 11);
        for (byte j = 0; j < 21; j++)
          display.print("^");
        display.setCursor(52, 16);
        display.setTextSize(2);
        display.print(String(dist[i]));
        display.setTextSize(1);
      }
      else if (i == 1)
      {
        for (byte j = 0; j < 3; j++)
        {
          display.setCursor(0, (j * 10) + 17);
          display.print("<");
        }
        display.setCursor(8, 22);
        display.setTextSize(2);
        display.print(String(dist[i]));
        display.setTextSize(1);
      }
      else if (i == 2)
      {
        display.setCursor(0, 46);
        for (byte j = 0; j < 21; j++)
          display.print("v");
        display.setCursor(52, 32);
        display.setTextSize(2);
        display.print(String(dist[i]));
        display.setTextSize(1);
      }
      else if (i == 3)
      {
        for (byte j = 0; j < 3; j++)
        {
          display.setCursor(122, (j * 10) + 17);
          display.print(">");
        }
        display.setCursor(106, 22);
        display.setTextSize(2);
        display.print(String(dist[i]));
        display.setTextSize(1);
      }
    }
  }
  display.display();
}

void setup() 
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    delay(2000);
    ESP.restart();
  }
  display.setTextColor(SSD1306_WHITE);

  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, 
    Adafruit_BMP280::SAMPLING_X2, 
    Adafruit_BMP280::SAMPLING_X16, 
    Adafruit_BMP280::FILTER_X16, 
    Adafruit_BMP280::STANDBY_MS_500);

  if (!imu.begin()) 
  {
    delay(2000);
    ESP.restart();
  }

  neo.begin(NEO_BAUD, SERIAL_8N1, NEO_RX, NEO_TX);

  dht.begin();

  print(titles[0], 1, values[0], v_sizes[0], 120);
  delay(2000);

  dhtRead();
  dhtTimer.attach_ms(5000, dhtRead);
  imuRead();
  imuTimer.attach_ms(100, imuRead);
  sonarRead();
  sonarTimer.attach_ms(150, sonarRead);
  bmpRead();
  bmpTimer.attach_ms(10000, bmpRead);
  gpsTimer.attach_ms(2500, gpsRead);
  if (printing)
    printTimer.attach_ms(3000, oprint);
}
 
void loop() 
{
  if (dist[0] < HC_MIN || dist[1] < HC_MIN || dist[2] < HC_MIN || dist[3] < HC_MIN) 
  {
    if (printing)
    {
      printTimer.detach();
      printing = false;
    }
    bool flag = false;
    while (dist[0] < HC_MIN || dist[1] < HC_MIN || dist[2] < HC_MIN || dist[3] < HC_MIN) 
    {
      flag = !flag;
      alert(flag);
      delay(HC_PRINT);
    }
    oprint();
    printing = true;
    if (printing)
      printTimer.attach_ms(3000, oprint);
  }
  
}

void oprint()
{
  i++;
  if (i >= sizeof(titles) / sizeof(titles[0]))
    i = 1;
  print(titles[i], 1, values[i], v_sizes[i], 90);
}

void dhtRead()
{
  float dhtTemp = dht.readTemperature(), dhtHum = dht.readHumidity();
  if (isnan(dhtTemp) || isnan(dhtHum)) 
  {
    values[2] = "ERR";
  }
  else
  {
    values[2] = String(dhtHum, 2) + " %\n" + String(dhtTemp, 2) + " C";
  }
}

void imuRead()
{
  sensors_event_t a, g, temp;
  imu.getEvent(&a, &g, &temp);

  values[3] = " " + String(g.gyro.x, 2) + " rps\n " + 
    String(g.gyro.y, 2) + " rps\n " +
    String(g.gyro.z, 2) + " rps";
}

void sonarRead()
{
  dist[0] = ultra1.ping_cm();
  dist[1] = ultra2.ping_cm();
  dist[2] = ultra3.ping_cm();
  dist[3] = ultra4.ping_cm();
}

void bmpRead()
{
  values[1] = String(bmp.readPressure() / 100.0F, 2);
}

void gpsRead()
{
  while (neo.available() > 0) 
  {
    gps.encode(neo.read());
    if (gps.location.isUpdated()) 
    {

      float lat = gps.location.lat(), lng = gps.location.lng();
      float minLat = abs(lat - int(lat)) * 60, minLng = abs(lng - int(lng)) * 60;
      float secLat = abs(minLat - int(minLat)) * 60, secLng = abs(minLng - int(minLng)) * 60;

      if (int(gps.time.hour() + timezone) > 23)
        values[4] = String(int(gps.time.hour() + timezone - 24)) + ":" + String(gps.time.minute());
      else if (int(gps.time.hour() + timezone) < 0)
        values[4] = String(int(gps.time.hour() + timezone + 24)) + ":" + String(gps.time.minute());
      else
        values[4] = String(int(gps.time.hour() + timezone)) + ":" + String(gps.time.minute());
      
      values[5] = String(int(lat)) + "'" + String(int(minLat)) + "\n" + String(secLat, 2) + "\"";
      values[6] = String(int(lng)) + "'" + String(int(minLng)) + "\n" + String(secLng, 2) + "\"";
      values[7] = String(gps.altitude.meters(), 2);
    }
  }
}
