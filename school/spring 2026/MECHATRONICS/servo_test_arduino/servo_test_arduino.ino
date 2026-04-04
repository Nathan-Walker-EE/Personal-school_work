#include <Servo.h>

Servo myServo;
int end_us = 1500;  // neutral pulse
int start_us = 1840;
int incr = 1;
int delay_time = 6;

void setup() {
  myServo.attach(3);
  myServo.writeMicroseconds(start_us);  // stop initially
  delay(4000);
}

//.9deg per us
void loop() {
  for (int pos = start_us; pos > end_us; pos -= incr) {
    myServo.writeMicroseconds(pos);
    delay(delay_time);  // controls sweep speed
  }

  for(int pos = end_us; pos < start_us; pos += incr){
    myServo.writeMicroseconds(pos);
    delay(delay_time);
  }
}
