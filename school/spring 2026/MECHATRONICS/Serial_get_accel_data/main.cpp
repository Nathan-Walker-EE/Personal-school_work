#include "data_logger.h"
#include "read_accel_data.h"

void setup() {

    Serial.begin(9600);
    delay(1000);   // give USB time to enumerate

    Serial.print("Serial ready");

    ExternalSensors sensors;
    sensors.startupTasks();
}


void loop(void){

}