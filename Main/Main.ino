#include <Servo.h>

int servoPin9 = 9;
int servoPin8 = 8 ;
Servo servo8;
Servo servo9;
int angle = 0;  // servo position in degrees

void setup() {
    servo8.attach(servoPin8);
    servo9.attach(servoPin9);
}

void loop() {

    // scan from 0 to 180 degrees
    for(angle = 0; angle < 90; angle+=10) {
        servo8.write(angle);
        servo9.write(angle);
        delay(500);
    }
    
    // now scan back from 180 to 0 degrees
    for(angle = 90; angle > 0; angle-=10) {
        servo8.write(angle);
        servo9.write(angle);
        delay(500);
    }
}