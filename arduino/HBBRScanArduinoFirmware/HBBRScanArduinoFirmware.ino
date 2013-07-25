// HBBRScan - http://combinatorialdesign.com/designs/HBBR_Scanner
//
// compatible wih
//
// FabScan - http://hci.rwth-aachen.de/fabscan
// 
//  by Francis Engelmann
//  Copyright 2011 Media Computing Group, RWTH Aachen University. All rights reserved.
//  Copyright 2013 Pawel Wodnicki. All rights reserved.


// 4 Phase Stepper PINS
#define  STEPPER_PIN1 2
#define  STEPPER_PIN2 3
#define  STEPPER_PIN3 4
#define  STEPPER_PIN4 5
#define LASER_PIN0 12
#define LASER_PIN1 13
#define MS_PIN    19

  
#define TURN_LASER_OFF      200
#define TURN_LASER_ON       201
#define PERFORM_STEP        202
#define SET_DIRECTION_CW    203
#define SET_DIRECTION_CCW   204
#define TURN_STEPPER_ON     205
#define TURN_STEPPER_OFF    206
#define TURN_LIGHT_ON       207
#define TURN_LIGHT_OFF      208
#define ROTATE_LASER        209
#define FABSCAN_PING        210
#define FABSCAN_PONG        211
#define SELECT_STEPPER      212
#define LASER_STEPPER       11
#define TURNTABLE_STEPPER   10
//the protocol: we send one byte to define the action what to do.
//If the action is unary (like turnung off the light) we only need one byte so we are fine.
//If we want to tell the stepper to turn, a second byte is used to specify the number of steps.
//These second bytes are defined here below.

#define ACTION_BYTE         1    //normal byte, first of new action
#define LIGHT_INTENSITY     2
#define TURN_TABLE_STEPS    3
#define LASER1_STEPS        4
#define LASER2_STEPS        5
#define LASER_ROTATION      6
#define STEPPER_ID          7

int incomingByte = 0;
int byteType = 1;
int currStepper;




int step_sequence[][4] =   {
                          {1,0,0,0},
                          {0,1,0,0},
                          {0,0,1,0},
                          {0,0,0,1}
                          };



int step_position  = 0;
int step_direction = 0;


void rotate_platform(){
  if ( step_direction == 0 ) { //CCW
     if (step_position >= 4)
       step_position = 0;
     digitalWrite(STEPPER_PIN1, step_sequence[step_position][0]);
     digitalWrite(STEPPER_PIN2, step_sequence[step_position][1]);
     digitalWrite(STEPPER_PIN3, step_sequence[step_position][2]);
     digitalWrite(STEPPER_PIN4, step_sequence[step_position][3]);
     step_position++;
     if (step_position == 4){
       step_position = 0;
     }
  } else {
     if (step_position < 0)
       step_position = 3;
     digitalWrite(STEPPER_PIN4, step_sequence[step_position][3]);
     digitalWrite(STEPPER_PIN3, step_sequence[step_position][2]);
     digitalWrite(STEPPER_PIN2, step_sequence[step_position][1]);
     digitalWrite(STEPPER_PIN1, step_sequence[step_position][0]);
     step_position--;
     if (step_position < 0){
       step_position = 3;
     }
  }
   
}

void step()
{
 if(currStepper == TURNTABLE_STEPPER){
   rotate_platform();
 }
 
 if(currStepper == LASER_STEPPER){
   // Do nothing
 }
  
 delay(3);
}

void step(int count)
{
  for(int i=0; i<count; i++){
    step();
  }
}

void setup() 
{ 
  // initialize the serial port
   Serial.begin(9600);
   pinMode(LASER_PIN0, OUTPUT);
   pinMode(LASER_PIN1, OUTPUT);
 
  pinMode(STEPPER_PIN1, OUTPUT);
  pinMode(STEPPER_PIN2, OUTPUT);
  pinMode(STEPPER_PIN3, OUTPUT);
  pinMode(STEPPER_PIN4, OUTPUT);
  step_position = 0;
  
 digitalWrite(LASER_PIN0, 1); //turn laser 0 on
 digitalWrite(LASER_PIN1, 1); //turn laser 1 on
 Serial.write(FABSCAN_PONG); //send a pong back to the computer so we know setup is done and that we are actually dealing with a FabScan
 
 currStepper = TURNTABLE_STEPPER;  //turntable is default stepper
} 

void loop() 
{

  if(Serial.available() > 0){

    incomingByte = Serial.read();
    
    switch(byteType){
      case ACTION_BYTE:
      
          switch(incomingByte){    //this switch always handles the first byte
            //Laser
            case TURN_LASER_OFF:
              digitalWrite(LASER_PIN0, LOW);    // turn the LASER off
              digitalWrite(LASER_PIN1, LOW);    // turn the LASER off
              break;
            case TURN_LASER_ON:
              digitalWrite(LASER_PIN0, HIGH);   // turn the LASER on
              digitalWrite(LASER_PIN1, HIGH);   // turn the LASER on
              break;
            case ROTATE_LASER: //unused
              byteType = LASER_ROTATION;
              break;
            //TurnTable
            case PERFORM_STEP:
              byteType = TURN_TABLE_STEPS;
              break;
              
            case SET_DIRECTION_CW:
              if(currStepper == TURNTABLE_STEPPER){
                step_direction = 1;
              }
              break;
            case SET_DIRECTION_CCW:
              if(currStepper == TURNTABLE_STEPPER){
                step_direction = 0;
              }
              break;
              
            
#ifdef FALSE
            case SET_DIRECTION_CW:
              if(currStepper == TURNTABLE_STEPPER){
                digitalWrite(DIR_PIN_0, 1);
              }
              break;
            case SET_DIRECTION_CCW:
              if(currStepper == TURNTABLE_STEPPER){
                digitalWrite(DIR_PIN_0, 0);
              }
              break;
            case TURN_STEPPER_ON:
              if(currStepper == TURNTABLE_STEPPER){
                digitalWrite(ENABLE_PIN_0, 0);
              }
              break;
            case TURN_STEPPER_OFF:
              if(currStepper == TURNTABLE_STEPPER){
                digitalWrite(ENABLE_PIN_0, 1);
              }
              break;
#endif              
            case FABSCAN_PING:
              delay(1);
              Serial.write(FABSCAN_PONG);
              break;
            case SELECT_STEPPER:
              byteType = STEPPER_ID;
              break;
            }
      
          break;
        case TURN_TABLE_STEPS:
          step(incomingByte);
          byteType = ACTION_BYTE;
          break;
        case STEPPER_ID:
          Serial.write(incomingByte);
          currStepper = incomingByte;
          byteType = ACTION_BYTE;
          break;
    }
  } 
} 

