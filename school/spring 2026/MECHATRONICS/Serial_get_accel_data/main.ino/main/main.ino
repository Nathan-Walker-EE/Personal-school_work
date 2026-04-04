#include "data_logger.h"
#include "read_accel_data.h"

ExternalSensors sensors;

void setup() {

    Serial.begin(115200);
    delay(1000);   // give USB time to enumerate

    Serial.print("Serial ready");

    //ExternalSensors sensors;
    sensors.startupTasks();
}

float x,y,z;
char buf[7];

void loop(void){
  delay(1000);
  sensors.readAcceleration(x,y,z);
  dtostrf(x,-7,3,buf);
  Serial.print("X Accel = ");
  Serial.print(buf);
  dtostrf(y,-7,3,buf);
  Serial.print("  Y Accel = ");
  Serial.print(buf);
  dtostrf(z,-7,3,buf);
  Serial.print("  Z Accel = ");
  Serial.print(buf);
  Serial.print("\n");
  Serial.write('\r');
}