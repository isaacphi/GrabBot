 
#include <SoftwareSerial.h>
#include <Servo.h>

#define pinSonar 4
#define pinNoise 0
#define pinServo 13

SoftwareSerial btSerial(6, 7);
int DEBUG_FLAG = 1;

Servo myservo;
int posServo=90;

/*Constants for Motor Code*/
//motor A connected between A01 and A02
//motor B connected between B01 and B02
//Constants

#define TURN_DELAY 725

int STBY = 10; //standby
//Motor A
int PWMA = 3; //Speed control 
int AIN1 = 9; //Direction
int AIN2 = 8; //Direction

//Motor B
int PWMB = 5; //Speed control
int BIN1 = 11; //Direction
int BIN2 = 12; //Direction

int FWD_TURN_PARAM = 100;
int BKW_TURN_PARAM = 50;

/*-----------------------*/

/* Sensor Code */
#define SOUNDOUT_PIN 2
#define IRDetect 0

unsigned long time;

/*-----------------------*/

/* Path Exploration Constants */
//Probability of choosing a direction
int dir_prob[8] = {32,32,32,32,32,32,32,32};

//Cum. prob of choosing directions
int dir_cum[8]  = {32,64,96,128,160,192,224,256};

//maximum cumulative probability of any direction
int MAX_DIR_PROB = 256;

//Number of directions we consider
int NUM_DIR = 8;


//constants that determine how much to move and how quickly to do so
#define MOVE_UNIT 100
#define MOVE_INCREMENT 100
#define MOVE_SPEED 150
#define MOVE_SPEED_FAST 230

int cur_move_dir = 0;
int last_move_dir = 0;
int last_bt_signal  = 0;

boolean first_time_bt_use = HIGH;
int turn_by_amount = 0;
int move_by_amount = 0;

int cm_sonar_read = 1000;
/*
Direction Metric relative to robot front
0 - N  1 - NE 2 - E 3 - SE 
4 - S  5 - SW 6 - W 7 - NW
*/


//Global state of the code 
int state = 0;
// state 0 is reading from sensor
// state 1 is turning
// state 2 is moving



/*-----------------------*/

void setup(){
  
  state = 0;
  
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
 myservo.attach(pinServo);
 myservo.write(posServo);
 pinMode(pinNoise, OUTPUT);
  
  pinMode(SOUNDOUT_PIN,OUTPUT);
  //pinMode(FREQ,INPUT);
  Serial.begin(9600);
  setupBluetooth(); 
}



int print_state = 0;
int servo_ctr = 90;
int dir = 1;
void make_some_sound()
{

  Serial.print("sonar ");
  Serial.println(cm_sonar_read);
  
  int cm = cm_sonar_read;
 // tail wag and eyes lit up
 if (cm < 50)
 {
   
    
    if(servo_ctr==180)
    {
      dir = 0;
    }
    if(servo_ctr==0)
    {
      dir=1;
    }
    
    
    if(dir==0)
    {
      servo_ctr-=15;
     // dir = 1;
    }
    if(dir==1)
    {
      servo_ctr+=15;
     // dir =0;
    }
    
      
    myservo.write(servo_ctr);
    delay(60);
 }

}
    /*  
    servo_ctr = (servo+1)%360;
    
    if((servo_ctr>=0 && servo_ctr<90) )
    {
       servo.write(servo_ctr+90);
    }
    if((servo_ctr>=90 && servo_ctr<270) )
    {
       servo.write(270-servo_ctr);
    }
    int delayVal=15;
    if (cm >40)
    {
     delayVal=10;
    }
    if (cm <40 )
    {
     delayVal=5;
    }
   
    if (cm < 30)
    {
     
     for (int i=0; i<3; i++)
     {  
       digitalWrite(pinNoise,HIGH);
       delay(150);
       digitalWrite(pinNoise,LOW);
       delay(200);  
     }
     
    }
    
    for(posServo = 90; posServo < 180; posServo += 1)  // goes from 0 degrees to 180 degrees
    {                                  // in steps of 1 degree
     myservo.write(posServo);              // tell servo to go to position in variable 'pos'
     //delay(delayVal);                       // waits 15ms for the servo to reach the position
    }
    for(posServo = 180; posServo>=1; posServo-=1)     // goes from 180 degrees to 0 degrees
    {                                
     myservo.write(posServo);              // tell servo to go to position in variable 'pos'
     ///delay(delayVal);                       // waits 15ms for the servo to reach the position
    }
  for(posServo = 0; posServo < 90; posServo += 1)  // goes from 0 degrees to 180 degrees
  {                                  // in steps of 1 degree
     myservo.write(posServo);              // tell servo to go to position in variable 'pos'
     delay(delayVal);                       // waits 15ms for the servo to reach the position
  }
  
  
 }
  
}*/

//choice fo robot functionality
#define CHOICE 2
void loop()
{
  
   // establish variables for duration of the ping,
   // and the distance result in inches and centimeters:
   long pulse;

   // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
   // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
   pinMode(pinSonar, INPUT);
   pulse = pulseIn(pinSonar, HIGH);
   cm_sonar_read = pulse*(2.54/147);
   
   Serial.println(cm_sonar_read);
  
  
  if(CHOICE == 1)
  {
    demo1();
  }
  
  if(CHOICE ==2)
  {
    demo2();
  }
    
}


void demo1()
{
  
   make_some_sound();
  
  
  String cmd = readBluetoothCommand();
  
  if(cmd == "fwd")
  {
    move_forward(MOVE_SPEED_FAST);
  }
  
  if(cmd =="rev")
  {
    move_backward(MOVE_SPEED_FAST);
  }
  
  if(cmd == "left")
  {
    turn_left();
    delay(TURN_DELAY);
    stop_motors();
  }
  
  if(cmd == "right")
  {
    turn_right();
    delay(TURN_DELAY);
    stop_motors();
  }
  
  if(cmd == "stop")
  {
    stop_motors();
  }
  
}

//Demo the strength seeking algorithm 
void demo2()
{
  
  //  make_some_sound();
 // if (cm_sonar_read < 50)
 // {
   //  move_backward(MOVE_SPEED);
     
    //delay(1000);
    //stop_motors();  
    //state=0;    
   
  //}
   
  
  
  if(DEBUG_FLAG)
  {
    Serial.print("state=");
    Serial.println(state); 
    print_state = 0;
  }
  
  print_state += 1;
  
  //Evaluate state
  if (state == 0)
  {
    //Read a value from BT
    int bt_signal = sample_bluetooth_signal(last_move_dir);
    delay(1000);
    
    if(DEBUG_FLAG)
    {
      Serial.print("Signal = ");  
      Serial.println(bt_signal); 
      Serial.println();  
    }
    
    //If this is the first time,
    if(first_time_bt_use == HIGH)
    {
      first_time_bt_use = LOW;
    }
     else
    {
      //Compare it to previous read - determine if it was better or worse update prob vector
      reeval_dist(last_move_dir, bt_signal>last_bt_signal);
      reeval_dist(last_move_dir, bt_signal>last_bt_signal);
    }
    last_bt_signal = bt_signal;
    
    //sample move direction
    cur_move_dir = choose_dir();
    //find out how much we can turn by
    turn_by_amount = compute_turn_amount(cur_move_dir, last_move_dir);
    
    if(DEBUG_FLAG)
    {
      
      Serial.print("cur_move_dir = ");  
      Serial.println(cur_move_dir); 
      Serial.print("last_move_dir = ");  
      Serial.println(last_move_dir); 
      Serial.print("turn_by_amount = ");  
      Serial.println(turn_by_amount); 
      Serial.println();  
    }
    state=1;
  }
  
  //do the turn
  if (state==1)
  {
    
    //turn by some amount
    if (turn_by_amount != 0 )
    {
      if(turn_by_amount < 0)
      {
        if(DEBUG_FLAG)
        {
          Serial.println("Turning left by 45 degrees");  
        }
        turn_left();
        delay(TURN_DELAY);
        stop_motors();
        turn_by_amount+=45;
      }
      else
      {
        if(DEBUG_FLAG)
        {
          Serial.println("Turning right by 45 degrees");  
        }
        turn_right();
        delay(TURN_DELAY);
        stop_motors();
        turn_by_amount-=45;
        
      }
      
    }
    else
    {
      //now we are looking in the direction of the cur_direction
    
      last_move_dir = cur_move_dir;
      move_by_amount = MOVE_UNIT;
      state=2;
    }
    
  }
  
  // do the move
  //check for hits/objects/avoidance etc
  if (state == 2)
  {
    if (move_by_amount != 0 )
    {
      if(DEBUG_FLAG)
      {
        Serial.print("Moving forward.....move_by_amount=");  
        Serial.println(move_by_amount);
      }
      move_forward(MOVE_SPEED);
      delay(1000);
      stop_motors();
      move_by_amount -= MOVE_INCREMENT;
    }
    else
    {
      state=0;
    }
  }
}


/* Bluetooth Code */
String readBluetoothCommand() {
  if (!btSerial.available())
    return "";
  
  String cmd = "";
  char c;
  while ((c = btSerial.read()) != '\n') {
    if (c != '\r')
      cmd += c;
  }
  
  return cmd;
}
 
void setupBluetooth()
{
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
  btSerial.begin(38400);
  btSerial.print("\r\n+STWMOD=0\r\n");
  btSerial.print("\r\n+STNA=BTBot\r\n");
  btSerial.print("\r\n+STPIN=4377\r\n");
  btSerial.print("\r\n+STOAUT=1\r\n");
  btSerial.print("\r\n+STAUTO=0\r\n");
  delay(2000);
  btSerial.print("\r\n+INQ=1\r\n");
  Serial.println("bluetooth ready");
  delay(2000);
  btSerial.flush();
}

/*-----------------*/

/* Path Exploration Code */
//returns turn by degrees
//if +ve clockwise (right)
//if -ve counterclockwise (left)
int compute_turn_amount(int current,int last)
{
  int diff = current-last;
  int turn = diff*45;
  
  //turn represents the difference in degrees between current and last direction as a multiple of 45 degrees
  
  //if turn is +ve and greater than 180 -> we can get there faster by switching the direction
  if(turn > 180)
  {
    turn = 360-turn; //shorten angle
    turn = -1 *turn; //reverse direction
  }
  
  //if turn is -ve and less than -180 -> we can get there faster by switching the direciont
  if(turn < -180)
  {
    turn = 360+turn; //direction gets switched with the addition in this case
  }
  return turn;
}


int choose_dir()
{
  int randNumber = random(1,256);
  if(DEBUG_FLAG)
  {
    Serial.println("PROB & CUMULATIVE");
    for(int i=0;i<NUM_DIR;i++)
    {
      
      Serial.print(dir_prob[i]);
      Serial.print(',');
    }
    Serial.println(); 
    for(int i=0;i<NUM_DIR;i++)
    {
      
      Serial.print(dir_cum[i]);
      Serial.print(',');
    }
    Serial.println(); 
  }
  for(int i=0;i<(NUM_DIR);i++)
  {
    if(randNumber <= dir_cum[i])
    {
      if(DEBUG_FLAG)
      {
       Serial.print("randnumber = ");
       Serial.println(randNumber);
       Serial.print("BIN CHOSEN=");
       Serial.println(i); 
      }
      return i;
    }
  }
  
}

//TODO: Unit test this using a static loop
//update the distributions
void reeval_dist(int index,boolean did_improve)
{
  if(did_improve==HIGH)
  {//If the signal did improve
  
    //How much dir_prob[index] can take
    int max_increase = (MAX_DIR_PROB)-dir_prob[index];
    if(max_increase>(NUM_DIR-1))
    {
      max_increase = (NUM_DIR-1);
    }
    
    //How much the others can contribute  
    int increased_by = 0;
    //Increase the probability of the current direction by as much can be taken
    //from the other directions. Upto the maximum accomodatable in the current prob
    for (int i=0; i<NUM_DIR && increased_by<max_increase; i++)
    {
      if(i!=index && dir_prob[i]!=0)
      {//Take a value from the i'th bin and give it to the index bin
       increased_by++;
       dir_prob[i]--;
       dir_prob[index]++;
      }
    }
    
  }
  else
  {
    //How much dir_prob[index] can take
    int max_decrease = dir_prob[index];
    if(max_decrease > (NUM_DIR-1))
      max_decrease = (NUM_DIR-1);
      
    //How much the others can contribute  
    int decreased_by = 0;
    //Decreases the probability of the current direction by as much can be taken
    //away from it and added to the other directions. Upto the maximum accomodatable in the other prob
    
    for (int i=0; i<NUM_DIR && decreased_by<max_decrease; i++)
    {
      if(i!=index && dir_prob[i]!=(MAX_DIR_PROB-1))
      {//Add a value to the i'th bin from the index bin
       decreased_by++;
       dir_prob[i]++;
       dir_prob[index]--;
      }
    }
  }
  
  
  //Recompute the cumulative distribution
  dir_cum[0] = dir_prob[0];
  for(int i = 1;i<NUM_DIR;i++)
  {
    dir_cum[i] = dir_cum[i-1] + dir_prob[i];
  }
}

/*-------------------------*/

/*
Simulating the Bluetooth Serial Interface
*/
int fake_bt_val = 127;
int fake_ctr    = 0;
int sample_bluetooth_signal(int last_dir)
{
  /*
  if(fake_ctr==1)
  {
    fake_ctr = 0;
    
    Serial.println("0 - Decrease 1- Increase");
    Serial.println(fake_bt_val);  
    Serial.println();
    while(!Serial.available())
    {
      
    }
    if (Serial.available() > 0) {    
      int update = Serial.read();
      if (update==48)//Decrease - 0 for user
      {
        if(fake_bt_val < 247)
          fake_bt_val -= 8;
        
      }
      if (update==49)//Increase - 1 for user
      {
        if(fake_bt_val >= 8)
          fake_bt_val += 8;
      }
      
      //break;
    }
  }
  fake_ctr++; 
  Serial.print("Fake_ctr=");
  Serial.println(fake_ctr);  
  Serial.println();
  */
  if(last_dir==2)
  {
    fake_bt_val+=8;
  }
  else if(last_dir==1)
  {
    fake_bt_val+=3;
  }
  else if(last_dir==3)
  {
    fake_bt_val+=3;
  }
  else
  {
    fake_bt_val-=4;
  }
    
  return fake_bt_val;
}

/*--------------------------*/



/*
Motor Controls
turn_right() -> turns the robot right
turn_left()  -> turns the robot left
For both these functions, we need to call stop motos to halt turning
Modulate
FWD_TURN_PARAM & BKW_TURN_PARAM to change turning speed

move_forward(speed) -> moves both motors forward with some speed
move_backward(speed)-> moves both motors backward with some speed

stop_motors() -> place motors on standby, use this function to halt the motion of motors
        
*/
void turn_right(){
  disable_standby();

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  digitalWrite(AIN1, inPin1);
  digitalWrite(AIN2, inPin2);
  analogWrite(PWMA, FWD_TURN_PARAM);
  
  digitalWrite(BIN1, inPin2);
  digitalWrite(BIN2, inPin1);
  analogWrite(PWMB, FWD_TURN_PARAM); // used to be bk
}

void turn_left(){
  disable_standby();

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  digitalWrite(AIN1, inPin2);
  digitalWrite(AIN2, inPin1);
  analogWrite(PWMA, FWD_TURN_PARAM); // used to be bk
  
  digitalWrite(BIN1, inPin1);
  digitalWrite(BIN2, inPin2);
  analogWrite(PWMB, FWD_TURN_PARAM);
}

//move both motors forward
void move_forward(int speed){
  disable_standby();

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  digitalWrite(AIN1, inPin1);
  digitalWrite(AIN2, inPin2);
  analogWrite(PWMA, speed);
  
  digitalWrite(BIN1, inPin1);
  digitalWrite(BIN2, inPin2);
  analogWrite(PWMB, speed);
  
}

//move both motors backward
void move_backward(int speed){
  disable_standby();

  boolean inPin1 = HIGH;
  boolean inPin2 = LOW;
  
  digitalWrite(AIN1, inPin1);
  digitalWrite(AIN2, inPin2);
  analogWrite(PWMA, speed);
  
  digitalWrite(BIN1, inPin1);
  digitalWrite(BIN2, inPin2);
  analogWrite(PWMB, speed);
  
}

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  disable_standby();

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

//Disable Standby
void disable_standby(){
  digitalWrite(STBY, HIGH); //disable standby
}

//Enable standby for the motor
void stop_motors(){
  digitalWrite(STBY, LOW);
}


