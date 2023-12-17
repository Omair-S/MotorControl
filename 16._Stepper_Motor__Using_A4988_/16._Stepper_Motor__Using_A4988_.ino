//Stepper motor (Using A4988 Driver)
/*
Overview of A4988 Driver pins:
This is Sufiyans version of this file
V_MOT: Supply voltage for motor. Input voltage 8V - 35V.
GND : Ground pin for VMOT.
A 100μF capacitor should be connected in parallel with the GND and VMOT to reduce noise.

VDD : Supply voltage for driving internal logic circuit. Input voltage 3V - 5.5V
GND : Ground pin for VDD.

1A and 1B pins are connected to one coil of the stepper motor, and 2A and 2B pins are connected to other
coil of stepper motor.

STEP: Controls the number of microsteps. Each high pulse sent to this pin steps the motor by number of
microsteps set by microstep selection pins (MS1,MS2,MS3). Faster the pulses, faster the motor will rotate.
DIR : Controls the spinning direction of motor. Pulling it high rotates the motor in clockwise direction
and pulling it low rotates the motor in counter-clockwise direction.
STEP and DIR pins are connected to digital pins of Arduino board.

MS pins: Microstepping pins. A4988 driver has three step size selector inputs, i.e., MS1,MS2 and MS3. By
setting appropriate logic levels to these pins, we can set the motor to one of the five step resolutions,
according to the following table:
     ________________________________________________
    |  MS1  |  MS2  |  MS3  |  Microstep Resolution  |
    |  Low  |  Low  |  Low  |       Full Step        |
    | High  |  Low  |  Low  |       Half Step        |
    |  Low  | High  |  Low  |     Quarter Step       |
    | High  | High  |  Low  |      Eighth Step       |
    | High  | High  | High  |    Sixteenth Step      |
    | High  |  Low  | High  |       1/32 Step        |
    |  low  | High  | High  |       1/64 Step        |
    |  Low  }  Low  | High  |       1/128 Step       |
     ________________________________________________

Power state control pins include ENABLE, SLEEP and RESET pins.
ENABLE: Used to enable the driver. Pulling it high disables the driver and pulling it low enables the
driver. By default, it is set to low, so the driver is always enabled.
SLEEP : Pulling this pin low puts the driver in sleep mode, thus minimizing the power consumption. It is
used when motor is not in use.
RESET : Sets the translator to a predefined home state (intial state). If set to low, all input steps
will be ignored.
*/
#define dir_pin 2
#define step_pin 3
#define L_sleep_pin 4
#define MS3_pin 5
#define MS2_pin 6
#define MS1_pin 7
#define L_en_pin 8


//JOYSTICK CONTROLL INPUTS
#define X_axis A0
#define Y_axis A1
#define SW A3

int x=0;
double D=1000;
int Direction = 1;
int joyPosVertical = 512;
int joyPosHorizontal = 512;
bool MS_config[3] = {LOW,LOW,LOW};

// Function to determine the percentage change in joystick position
int calculatePercentageChange(int currentPosition, int initialPosition) {
  int temp = ((int)(currentPosition - initialPosition) / 1023.0) * 100.0;
  if(temp < 0){
    temp = -1*temp;
  }
  return temp;
}

//determine the direction in which the stepper motor will move
bool determine_direction(int x){
  bool output = false;
  if(x > 512){
    output = true;
  }
  return output;
}

// determine how fast the motor will move in a direction
int speedControl(int percentage_change){
  int x = map(percentage_change, 0,50,0,8);
  
  digitalWrite(MS1_pin, MS_config[0]);
  digitalWrite(MS2_pin, MS_config[1]);
  digitalWrite(MS3_pin, MS_config[2]);
  switch(x){    
    case 1:
      MS_config[0] = HIGH;
      MS_config[1] = LOW;
      MS_config[2] = HIGH;
      break;
    case 3:
      MS_config[0] = HIGH;
      MS_config[1] = HIGH;
      MS_config[2] = HIGH;
      break;
    case 4:
      MS_config[0] = HIGH;
      MS_config[1] = HIGH;
      MS_config[2] = LOW;
      break;
    case 5:
      MS_config[0] = LOW;
      MS_config[1] = HIGH;
      MS_config[2] = LOW;
      break;
    case 6:
      MS_config[0] = HIGH;
      MS_config[1] = LOW;
      MS_config[2] = LOW;
      break;
    case 7:
      MS_config[0] = LOW;
      MS_config[1] = LOW;
      MS_config[2] = LOW;
      break;
    default:
      break;
  }
  return x;
}

void setup()
{
  pinMode(dir_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(L_sleep_pin, OUTPUT);
  pinMode(MS3_pin, OUTPUT);
  pinMode(MS2_pin, OUTPUT);
  pinMode(MS1_pin, OUTPUT);
  pinMode(L_en_pin, OUTPUT);
  
  //  setup the joystick for input
  pinMode(SW, INPUT);
  
  digitalWrite(L_en_pin, LOW);
  digitalWrite(L_sleep_pin, HIGH);
  
  // joystick write
  digitalWrite(SW,HIGH);     
  Serial.begin(9600);
}

void loop()
{
  // saving the verticle and horizontal positions
  joyPosHorizontal = analogRead(X_axis);
  joyPosVertical = analogRead(Y_axis); 
  Direction = determine_direction(joyPosHorizontal);
  digitalWrite(dir_pin,Direction);

  
//  calculate percentage change
  int percentageChange = calculatePercentageChange(joyPosHorizontal, 512);
  int moving  = speedControl(percentageChange);
  digitalWrite(MS1_pin, MS_config[0]);
  digitalWrite(MS2_pin, MS_config[1]);
  digitalWrite(MS3_pin, MS_config[2]); 

//  step to move the stepper motor
// move only if the joystick is moving
  if(moving){
    digitalWrite(step_pin,HIGH);
    delayMicroseconds(D);
    digitalWrite(step_pin,LOW);
    delayMicroseconds(D);  
  }
  
  
}
