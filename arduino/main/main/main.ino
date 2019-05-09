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

// called this way, it uses the default address 0x40
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x42);
//Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define INCREMENT 75
#define FORWARD 1
#define BACKWARD 0

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(&Wire, 0x42);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(&Wire, 0x41);

uint16_t motor_angles1[14] = {SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN};
uint16_t motor_angles2[14] = {SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN,SERVOMIN};

void setup() {
  Serial.begin(9600);
  // Serial.println("28 channel Servo test!");
  
  pwm1.begin();
  pwm1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  pwm2.begin();
  pwm2.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  // reset all motors to state 0 i.e. arm fully extended
  for(int motor = 0; motor < 14; motor++) {
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX/2; pulselen++) {
       pwm1.setPWM(motor, 0, pulselen);
       pwm2.setPWM(motor, 0, pulselen);
    }
    motor_angles1[motor] = SERVOMAX/2;
    motor_angles2[motor] = SERVOMAX/2;
  }
  delay(10);
  Serial.write('*');
}

void loop() {
  if(Serial.available() > 1) {
    String in = Serial.readString();
    for(int i = 0; i < 14; i++) { 
      int state1 = in.charAt(i) - '0';
      int state2 = in.charAt(i+14) - '0';
//      Serial.print(state1);
//      Serial.print(state2);
      // update_step(i, state);
      update_smooth(i, state1, motor_angles1, pwm1);
      update_smooth(i, state2, motor_angles2, pwm2);
    }
    Serial.write('*');
    Serial.println();
  }
}

void update_step(int i, int state, uint16_t* motor_angles, Adafruit_PWMServoDriver pwm) {
      if(state == FORWARD && motor_angles1[i] < SERVOMAX/2) {
        motor_angles[i] = motor_angles[i] + INCREMENT;
        pwm.setPWM(i, 0, motor_angles[i]);
      } else if (state == BACKWARD && motor_angles[i] > SERVOMIN) {
        motor_angles[i] = motor_angles[i] - INCREMENT/3;
        pwm.setPWM(i, 0, motor_angles[i]);
      }
}

void update_smooth(int i, int state, uint16_t* motor_angles, Adafruit_PWMServoDriver pwm) {
    if (state == FORWARD && motor_angles[i] < SERVOMAX/2) {
        for (uint16_t pulselen = motor_angles[i]; pulselen <= motor_angles[i] + INCREMENT; pulselen++) { // todo
            pwm.setPWM(i, 0, pulselen);
        }
        motor_angles[i] = motor_angles[i] + INCREMENT; // todo
    } else if (state == BACKWARD && motor_angles[i] > SERVOMIN) {
        for (uint16_t pulselen = motor_angles[i]; pulselen >= motor_angles[i] - INCREMENT; pulselen--) {
            pwm.setPWM(i, 0, pulselen);
        }
        motor_angles[i] = motor_angles[i] - INCREMENT;
    }
}
