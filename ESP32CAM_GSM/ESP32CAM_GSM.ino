#include <TinyGPS++.h>
#include <HardwareSerial.h>

// GPS on UART1 using GPIO16 (RX), GPIO2 (TX)
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

#define GPS_RX 16  // GPS TX connects here
#define GPS_TX 2   // Optional: GPS RX (not required)

void setup() {
  Serial.begin(115200);  // Serial Monitor
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX); // GPS Serial

  Serial.println("ESP32-CAM + GPS6MV2 (NEO-6M) Test");
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
    Serial.print("Speed (km/h): ");
    Serial.println(gps.speed.kmph());
    Serial.println("-------------------------");
  }

  delay(1000);
}
