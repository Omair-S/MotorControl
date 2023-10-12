//Stepper motor (Using A4988 Driver)
/*
Overview of A4988 Driver pins:
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

Power state control pins include ENABLE, SLEEP and RESET pins.
ENABLE: Used to enable the driver. Pulling it high disables the driver and pulling it low enables the
driver. By default, it is set to low, so the driver is always enabled.
SLEEP : Pulling this pin low puts the driver in sleep mode, thus minimizing the power consumption. It is
used when motor is not in use.
RESET : Sets the translator to a predefined home state (intial state). If set to low, all input steps
will be ignored.
*/

int s=3; //step pin
int d=4; //direction pin
int X; //analog value on X-axis
int S; //switch state
int x;
int D;
void setup() {
  pinMode(s,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(2,INPUT);
  Serial.begin(9600);
  digitalWrite(2,HIGH);
}

void loop() {
  X=analogRead(A0);
  S=digitalRead(2);
  if(X>512)
  digitalWrite(d,HIGH); //sets motor direction clockwise
  if(X<512)
  digitalWrite(d,LOW); //sets motor direction anti-clockwise
  else {
    digitalWrite(s,LOW);
  }
  x=abs(X-512);
  D=(-2125.*x/256.)+5000.;
  if(S==1)
    for(int i=0;i<10;i++)
    {
    digitalWrite(s,HIGH);
    delayMicroseconds(D);
    digitalWrite(s,LOW);
    delayMicroseconds(D);
    }
  Serial.println(x);
}
