#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int TC74_address = 0b1001000;

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x3F for

Servo myservo0; 
Servo myservo1;
Servo myservo2;
Servo myservo3;

Servo servos[4] = {myservo0, myservo1, myservo2, myservo3};

// Servos Position Tracking
// 0: fully extended
// 1: middle
// 2: fully retracted
int servosPosition[4] = {0,0,0,0};

void setup() {
  
    // LCD
    lcd.begin(23, 22);// initialize the lcd with SDA and SCL pins
    lcd.backlight();// Turn on backlight
    lcd.setCursor(0,0);//set the cursor position to the first line

    // SERVOS
    servos[0].attach(13);  // attaches the servo on pin 13 to the servo object
    servos[1].attach(12);
    servos[2].attach(27);
    servos[3].attach(33);
    
    Serial.begin(9600);
}

void loop() {

    String incoming_string;

    if (Serial.available() > 0) {
        lcd.clear();
        incoming_string = Serial.readString();
//        lcd.print(incoming_string);
        char up_left = incoming_string.charAt(1);
        char up_right = incoming_string.charAt(3);
        char down_left = incoming_string.charAt(5);
        char down_right = incoming_string.charAt(7);
        lcd.print(up_left);
        lcd.print(up_right);
        lcd.print(down_left);
        lcd.print(down_right);
        
        updatePosition((int) up_left, 0);
    }
    delay(300);
}

void updatePosition(int target_pos, int servoNum) {
  int current_pos = servosPosition[servoNum];
  if (current_pos < target_pos) {
    int num_stages = target_pos - current_pos;
    backward(num_stages, servoNum);
  } else if (current_pos > target_pos) {
    int num_stages = current_pos - target_pos;
    forward(num_stages, servoNum);
  }
}

void forward(int numStages, int servoNum) {
  int current_degrees = servosPosition[servoNum]*30;
  int target_degrees = 30*numStages;
  for (int pos = current_degrees; pos <= target_degrees; pos += 1) { 
    servos[servoNum].write(pos); // in steps of 1 degree       
    delay(5);
  }
}

void backward(int numStages, int servoNum) {
  int current_degrees = servosPosition[servoNum]*30;
  int target_degrees = 30*numStages;
  for (int pos = current_degrees; pos >= target_degrees; pos -= 1) { 
    servos[servoNum].write(pos); // in steps of 1 degree       
    delay(5);
  }
}



