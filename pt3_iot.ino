/*************************************************************

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - ESP8266 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6QNod7iYY"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "mYG3PqyNj5pxlycGX-UHxZKBjmL6eX4P"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define LEDMERAH D1
#define LEDKUNING D2
#define LEDHIJAU D3
#define DHTPIN D7
#define DHTTYPE DHT11
#define BUZZERPIN D5

DHT dht(DHTPIN, DHTTYPE);

int kondisi;
BlynkTimer timer;

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "Universitas Mulawarman";
const char* pass = "";

BLYNK_WRITE(V0) {
  kondisi = param.asInt();
}

void blynkControl() {
  if (kondisi == 1) {
    float c = dht.readTemperature();

    if (isnan(c)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    float f = (c * 9 / 5) + 32;
    float k = c + 273;

    Blynk.virtualWrite(V3, c);
    Blynk.virtualWrite(V4, f);
    Blynk.virtualWrite(V5, k);

    Serial.print("Suhu: ");
    Serial.println(c);

    if (c < 30) {
      digitalWrite(LEDMERAH, LOW);
      digitalWrite(LEDKUNING, LOW);
      digitalWrite(LEDHIJAU, HIGH);
    } else if (c > 36) {
      digitalWrite(LEDMERAH, HIGH);
      digitalWrite(LEDKUNING, LOW);
      digitalWrite(LEDHIJAU, LOW);
      tone(BUZZERPIN, 150);
    } else if (c > 30 && c < 36 ) {
      digitalWrite(LEDMERAH, LOW);
      digitalWrite(LEDKUNING, HIGH);
      digitalWrite(LEDHIJAU, LOW);
      noTone(BUZZERPIN);                                                                                                            
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LEDMERAH, OUTPUT);
  pinMode(LEDKUNING, OUTPUT);
  pinMode(LEDHIJAU, OUTPUT);
  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, blynkControl);
}

void loop()
{
  Blynk.run();
  timer.run();
}
