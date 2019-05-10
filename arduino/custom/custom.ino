/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(&Wire, 0x42);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(&Wire, 0x41);

uint16_t motor_angles1[14] = {SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN};
uint16_t motor_angles2[14] = {SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN};

void setup() {
  Serial.begin(9600);
  
  pwm1.begin();
  pwm1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  pwm2.begin();
  pwm2.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  delay(10);
}

void loop() {
    Serial.println("IN LOOP");
    int range = SERVOMAX / 2 - SERVOMIN;
    
    for (int i =  0; i < range; i++) {
      for (int motor_idx = 0; motor_idx < 14; motor_idx++) {
        if (motor_idx % 2 == 0) { // Case for even motors
          int pulselen = SERVOMAX / 2 - i;
          pwm1.setPWM(motor_idx, 0, pulselen);
          pwm2.setPWM(motor_idx, 0, pulselen);
        }
        else { // Case for odd motors
          int pulselen = SERVOMIN + i;
          pwm1.setPWM(motor_idx, 0, pulselen);
          pwm2.setPWM(motor_idx, 0, pulselen);
        }
      }
    }
    Serial.println("REVERSE");
    delay(500);
    for (int i =  0; i < range; i++) {
      for (int motor_idx = 0; motor_idx < 14; motor_idx++) {
        if (motor_idx % 2 == 0) { // Case for even motors
          int pulselen = SERVOMIN + i;
          pwm1.setPWM(motor_idx, 0, pulselen);
          pwm2.setPWM(motor_idx, 0, pulselen);
//          delay(1);
        }
        else { // Case for odd motors
          int pulselen = SERVOMAX / 2 - i;
          pwm1.setPWM(motor_idx, 0, pulselen);
          pwm2.setPWM(motor_idx, 0, pulselen);
//          delay(1);
        }
      }
    }
    delay(500);
}

