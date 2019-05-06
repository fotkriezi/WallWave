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
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x42);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(&Wire, 0x40);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;
int motor_states[14] = {SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN};

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  for(int motor = 0; motor < 14; motor++) {
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX/2; pulselen++) {
            pwm.setPWM(motor, 0, pulselen);
    }
    motor_states[motor] = SERVOMAX/2;
  }

  delay(10);
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  if(Serial.available()) {
    String in = Serial.readString();
    for(int i = 0; i < 15; i++) {
      char state = in.charAt(i);
      if(state == '0') {
          for (uint16_t pulselen = motor_states[i]; pulselen < SERVOMAX/2; pulselen++) {
            pwm.setPWM(i, 0, pulselen);
          }
          motor_states[i] = SERVOMAX/2;
      } else if(state == '1') {
        for (uint16_t pulselen = motor_states[i]; pulselen > SERVOMIN; pulselen--) {
            pwm.setPWM(i, 0, pulselen);
          }
          motor_states[i] = SERVOMIN;
      }
    }
  }
}
