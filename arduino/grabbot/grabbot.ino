#include <Servo.h>
#include <SoftwareSerial.h>

/*
  Get your bot on 2014
  Reads over serial port 9600 for motor speed and grap position commands
  in the form of
  "<NE motor speed>,<NW motor speed>,<SE motor speed>,<SW motor speed>,<grabber position>"
  values range from 0 to 180
*/

Servo NE_servo, NW_servo, SE_servo, SW_servo, grab_servo;

int NE_speed = 90;
int NW_speed = 90;
int SE_speed = 90;
int SW_speed = 90;
int grab_pos = 90;

void setup()
{
  Serial.begin(9600);
  NE_servo.attach(9);  // attaches the servo on pin 9 to the servo object
  NW_servo.attach(10);
  SE_servo.attach(11);
  SW_servo.attach(12);
  grab_servo.attach(13);
}

void loop()
{
  while (Serial.available() > 0) {
    // look for the next valid integer in the incoming serial stream,
    // separated by commas
    NE_speed = Serial.parseInt();
    NW_speed = Serial.parseInt();
    SE_speed = Serial.parseInt();
    SW_speed = Serial.parseInt(); 
    grab_pos = Serial.parseInt();
    if (Serial.read() == '\n')
      { break; }
  }
  NE_servo.write(NE_speed);
  NW_servo.write(NW_speed);
  SE_servo.write(SE_speed);
  SW_servo.write(SW_speed);
  grab_servo.write(grab_pos);
}
