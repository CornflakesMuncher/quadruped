/* FILE:    QuadrupedMainframe
   DATE:    05/04/2023
   VERSION: 2.5
   AUTHOR:  Tyron Wood
   
10/06/16 version 0.1: Original version by Hobby Components Ltd (HOBBYCOMPONENTS.COM)

This example demonstrates how to use the HCPCA9685 library together with the PCA9685
to control up to 16 servos. The sketch will initialise the library putting it into 
'servo mode' and then will continuously sweep one servo connected to PWM output 0 
back and forth. The example has been written particularly for the 16 Channel 12-bit 
PWM Servo Motor Driver Module (HCMODU0097) available from hobbycomponents.com 

To use the module connect it to your Arduino as follows:

PCA9685...........Uno/Nano
GND...............GND
OE................N/A
SCL...............A5
SDA...............A4
VCC...............5V

External 5V Power for the servo(s) can be supplied by the V+ and GND input of the 
screw terminal block.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/


/* Include the HCPCA9685 library */
#include "HCPCA9685.h"

/* I2C slave address for the device/module. For the HCMODU0097 the default I2C address
   is 0x40 */
#define  I2CAdd 0x40


/* Create an instance of the library */
HCPCA9685 HCPCA9685(I2CAdd);

int TibeaAngle[] = {270,180,90,180};
int FemerAngle[] = {270,180,90,180};

int j;
int k;
int l;

unsigned long previousMillis = 0;
unsigned long interval = 1500;


enum State {
    STORE,
    LEFT,
    RIGHT,
    WALK,
    REVERSE,
    NUM_STATES  // always keep this one last
};

State currentState = STORE;
unsigned long stateStartMillis;
unsigned long mainMillis = 0; 
const long mainInterval = 1000;  // interval at which to do something (milliseconds)


bool isFuncRunning = false; // a flag to check if the function is running
unsigned long funcStartMillis = 0; // timestamp when the function started
unsigned long funcDuration = 5000; // function duration, e.g., 5000 milliseconds (5 seconds)
int mode = 1;


/*

int mapValues(int input_value) {
  int output_value = 0;
  if(output_value<=30){
    output_value = 30;
  }
  if(output_value>=420){
    output_value = 420;
  }
  return output_value;

*/

double mapValues(int input_value) {
  const double input_min = 30.0;
  const double input_max = 330.0;
  const double output_min = 0.0;
  const double output_max = 470.0;
  
  double input_range = input_max - input_min;
  double output_range = output_max - output_min;
  double ratio = (input_value - input_min) / input_range;
  
  double output_value = (ratio * output_range) + output_min;
  
  if(output_value <= 30){
    output_value = 50;
  }
  if(output_value >= 470){
    output_value = 450;
  }

  return output_value;

}


void setup() 
{
  /* Initialise the library and set it to 'servo mode' */ 
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);

  /* Initialize Serial communication */
  Serial.begin(9600); // Baud rate should match the one in MATLAB
}


/* HERE IS THE START OF THE MAIN LOOP*/
/* HERE IS THE START OF THE MAIN LOOP*/
/* HERE IS THE START OF THE MAIN LOOP*/
void loop() 
{
   if (Serial.available()) {
     char newCommand = Serial.read();
    char command = newCommand;
    switch (command) {
      case 's':      
        MATLABWalk();
        break;
      case 'l':
        Left();
        break;
      case 'r':
        Right();
        break;
      case 'w':
        Walk();
        break;
      case 'u':
        ReverseWalk();
        break;
      case 'i':
        // Read the rest of the line, convert it to an integer, and set the interval speed
        String speedStr = Serial.readStringUntil('\n');
        int speed = speedStr.toInt();
        setIntervalSpeed(speed);
        Serial.println(speed);
        break;
    }
  }
}

void MATLABWalk() {
int FemerAngle[] = {181,165,146,145,129,121,121,130,140,140,142,144,145,147,149,151,152,154,155,156,157,158,159,160,161,162,163,165,166,168,170,172,174,177,181,187};
int TibeaAngle[] = {168,134,109,108,106,116,116,133,144,144,144,145,147,148,149,150,151,152,152,153,153,153,152,153,153,153,153,154,155,156,157,159,162,165,169,179};



interval = 50;

  static int i = 0, j = 0.25*sizeof(TibeaAngle)/sizeof(TibeaAngle[0]), k = 0.5*sizeof(TibeaAngle)/sizeof(TibeaAngle[0]), l = 0.75*sizeof(TibeaAngle)/sizeof(TibeaAngle[0]);
  if (i >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    i = 0;
  }
  if (j >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    j = 0;
  }
  if (k >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    k = 0;
  }
  if (l >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    l = 0;
  }
  
  /* Set the position of the servos */
  //Front right
  HCPCA9685.Servo(0, mapValues(TibeaAngle[i])-4);  
  HCPCA9685.Servo(1, mapValues(FemerAngle[i])-4);  
  //Front left
  HCPCA9685.Servo(3, mapValues(TibeaAngle[j]-4));  
  HCPCA9685.Servo(4, mapValues(FemerAngle[j]-3));  
  //Back right
  HCPCA9685.Servo(6, mapValues(TibeaAngle[k]));  
  HCPCA9685.Servo(7, mapValues(FemerAngle[k]));  
  //Back left
  HCPCA9685.Servo(9, mapValues(TibeaAngle[l]-10));
  HCPCA9685.Servo(10, mapValues(FemerAngle[l])+10);

  /* Update the loop variables */
  i++;
  j++;
  k++;
  l++;

  /* Wait for the motor to receive the command */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) {
    delay(interval - (currentMillis - previousMillis));
  }
  previousMillis = millis();
}


void Store() {
int BFemerAngle[] = {280,280,280,280};
int FFemerAngle[] = {90,90,90,90};

int BTibeaAngle[] = {300,300,300,300};
int FTibeaAngle[] = {45,45,45,45};
interval = 150;

  static int i = 0;
  if (i >= sizeof(FTibeaAngle)/sizeof(FTibeaAngle[0])) {
    i = 0;
  }
  
    /* Set the position of the servos */
  //Back Left
  HCPCA9685.Servo(0, mapValues(BTibeaAngle[i]));  
  HCPCA9685.Servo(1, mapValues(BFemerAngle[i])-14);  
  //Back Right
  HCPCA9685.Servo(3, mapValues(BTibeaAngle[i]-4));  
  HCPCA9685.Servo(4, mapValues(BFemerAngle[i]-3));  
  //Front Left
  HCPCA9685.Servo(6, mapValues(FTibeaAngle[i]));  
  HCPCA9685.Servo(7, mapValues(FFemerAngle[i]));  
  //Front Right
  HCPCA9685.Servo(9, mapValues(FTibeaAngle[i]-10));
  HCPCA9685.Servo(10, mapValues(FFemerAngle[i])+18);

  /* Update the loop variables */
  i++;

  /* Wait for the motor to receive the command */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) {
    delay(interval - (currentMillis - previousMillis));
  }
  previousMillis = millis();
}

void Left() {
// int RFemerAngle[] = {177,177,174,169,165,165,157,149,143,139,139,140,144,151,153,153,153,154,154,155,156,156,157,158,158,159,160,160,161,161,162,162,163,163,163,163,164,164,165,165,165,166,166,167,167,168,168,169,169,170,171,171,172,173,174,175,175,176,177,178};
// int RTibeaAngle[] = {158,145,123,105,98,96,97,101,109,122,122,132,149,169,167,167,167,166,166,167,167,167,167,167,167,167,167,167,167,166,166,165,164,163,162,161,159,158,158,157,156,155,154,154,154,153,153,153,153,153,154,154,155,156,156,157,158,160,161,163};

// int LFemerAngle[] = {177,177,174,169,165,165,160,156,152,148,148,147,149,152,155,155,156,157,157,158,159,159,160,161,161,162,162,163,164,164,165,165,166,166,167,167,167,168,168,169,169,169,170,170,170,171,171,172,172,172,173,174,174,175,175,176,176,177,178,178};
// int LTibeaAngle[] = {158,145,123,105,98,96,96,97,99,102,102,108,119,129,131,131,132,133,135,136,137,138,139,140,142,143,144,145,146,147,148,148,149,149,149,150,150,149,150,150,150,150,150,150,151,151,151,152,152,153,153,154,155,156,156,158,159,160,161,163};
// int RFemerAngle[] = {172,168,159,159,148,140,140,144,152,152,153,154,155,157,158,159,161,162,162,163,163,163,164,164,164,165,165,166,167,167,168,169,170,171,172,174};
// int RTibeaAngle[] = {158,121,97,96,102,119,119,144,159,159,160,162,163,165,167,169,170,171,171,170,168,166,163,162,160,159,158,157,157,157,157,157,158,159,160,162};

// int LFemerAngle[] = {172,168,159,159,151,144,144,146,152,152,153,155,156,157,158,159,160,161,162,163,164,164,164,165,166,166,166,167,168,168,169,170,171,172,173,174};
// int LTibeaAngle[] = {158,121,97,96,100,108,108,127,139,139,141,143,145,147,149,151,153,154,155,156,156,156,155,156,155,155,154,154,155,155,156,157,158,159,160,162};


int FLFemerAngle[] = {180,225,225,135,135,135,135,180,180};
int FLTibeaAngle[] =  {180,225,225,135,135,135,135,180,180};

// int BLFemerAngle[] = {180,225,225,135,135,135,135,180,180};
// int BLTibeaAngle[] =  {180,225,225,135,135,135,135,180,180};

int BLFemerAngle[] = {135,135,180,180,180,225,225,225,225};
int BLTibeaAngle[] = {135,135,180,180,180,225,225,225,225};

int BRFemerAngle[] = {180, 135, 135, 225, 225, 225, 225, 180, 180};
int BRTibeaAngle[] = {180, 135, 135, 225, 225, 225, 225, 180, 180};

int FRFemerAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 135};
int FRTibeaAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 135};




interval = 50;

  static int i = 0, j = 0.25*sizeof(BRTibeaAngle)/sizeof(BRTibeaAngle[0]), k = 0.5*sizeof(BRTibeaAngle)/sizeof(BRTibeaAngle[0]), l = 0.75*sizeof(BRTibeaAngle)/sizeof(BRTibeaAngle[0]);
  if (i >= sizeof(BRTibeaAngle)/sizeof(BRTibeaAngle[0])) {
    i = 0;
  }
  if (j >= sizeof(BRTibeaAngle)/sizeof(BRTibeaAngle[0])) {
    j = 0;
  }
  if (k >= sizeof(BRTibeaAngle)/sizeof(BRTibeaAngle[0])) {
    k = 0;
  }
  if (l >= sizeof(BRTibeaAngle)/sizeof(BRTibeaAngle[0])) {
    l = 0;
  }
  

  /* Set the position of the servos */
  //Back Left
  HCPCA9685.Servo(0, mapValues(BLTibeaAngle[i]));  
  HCPCA9685.Servo(1, mapValues(BLFemerAngle[i])-14);  
  //Back Right
  HCPCA9685.Servo(3, mapValues(BRTibeaAngle[j]-4));  
  HCPCA9685.Servo(4, mapValues(BRFemerAngle[j]-3));  
  //Front Left
  HCPCA9685.Servo(6, mapValues(FLTibeaAngle[k]));  
  HCPCA9685.Servo(7, mapValues(FLFemerAngle[k]));  
  //Front Right
  HCPCA9685.Servo(9, mapValues(FRTibeaAngle[l]-10));
  HCPCA9685.Servo(10, mapValues(FRFemerAngle[l])+18);
      /* Set the position of the servos */
  //Back Left
  HCPCA9685.Servo(0, mapValues(BLTibeaAngle[i]));  
  HCPCA9685.Servo(1, mapValues(BLFemerAngle[i])-14);  
  //Back Right
  HCPCA9685.Servo(3, mapValues(BRTibeaAngle[i]-4));  
  HCPCA9685.Servo(4, mapValues(BRFemerAngle[i]-3));  
  //Front Left
  HCPCA9685.Servo(6, mapValues(FLTibeaAngle[i]));  
  HCPCA9685.Servo(7, mapValues(FLFemerAngle[i]));  
  //Front Right
  HCPCA9685.Servo(9, mapValues(FRTibeaAngle[i]-10));
  HCPCA9685.Servo(10, mapValues(FRFemerAngle[i])+18);

  /* Update the loop variables */
  i++;
  j++;
  k++;
  l++;

  /* Wait for the motor to receive the command */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) {
    delay(interval - (currentMillis - previousMillis));
  }
  previousMillis = millis();
}


void Right() {
// int LFemerAngle[] = {174,174,170,166,161,161,154,148,143,139,139,140,144,151,153,153,153,154,155,155,156,157,158,159,159,160,161,161,162,162,163,163,163,163,163,163,163,164,164,164,164,165,165,165,166,166,167,167,168,168,169,169,170,171,171,172,173,174,174,175};
// int LTibeaAngle[] = {157,143,121,104,97,96,97,102,110,122,122,132,149,169,167,167,167,168,168,169,170,170,171,172,173,174,174,174,174,174,173,172,170,169,167,166,164,162,162,161,160,158,158,157,156,156,156,155,155,155,156,156,156,157,157,158,158,159,160,161};


// int RFemerAngle[] = {174,174,170,166,161,161,156,151,147,143,143,142,145,149,152,152,153,153,154,155,155,156,157,158,158,159,160,160,161,161,162,162,163,163,164,164,164,165,165,165,166,166,166,167,167,167,168,168,169,169,170,170,171,171,172,173,173,174,174,175};
// int RTibeaAngle[] = {157,143,121,104,97,96,97,99,104,110,110,118,130,141,142,142,143,144,145,146,147,148,149,150,151,152,153,154,155,155,156,156,156,156,156,156,155,155,156,155,154,154,154,154,153,153,154,154,154,154,155,155,155,156,157,158,158,159,160,161};
// int LFemerAngle[] = {172,168,159,159,148,140,140,144,152,152,153,154,155,157,158,159,161,162,162,163,163,163,164,164,164,165,165,166,167,167,168,169,170,171,172,174};
// int LTibeaAngle[] = {158,121,97,96,102,119,119,144,159,159,160,162,163,165,167,169,170,171,171,170,168,166,163,162,160,159,158,157,157,157,157,157,158,159,160,162};

// int RFemerAngle[] = {172,168,159,159,151,144,144,146,152,152,153,155,156,157,158,159,160,161,162,163,164,164,164,165,166,166,166,167,168,168,169,170,171,172,173,174};
// int RTibeaAngle[] = {158,121,97,96,100,108,108,127,139,139,141,143,145,147,149,151,153,154,155,156,156,156,155,156,155,155,154,154,155,155,156,157,158,159,160,162};

int RFemerAngle[] = {180,225,225,225,135,135,135,180,180};
int RTibeaAngle[] =  {180,225,225,225,135,135,135,180,180};

int BRFemerAngle[] = {180,225,225,225,135,135,135,180,180};
int BRTibeaAngle[] =  {180,225,225,225,135,135,135,180,180};

// int FLFemerAngle[] = {180,225,225,135,135,135,135,180,180};
// int FLTibeaAngle[] =  {180,225,225,135,135,135,135,180,180};


// int BLFemerAngle[] = {135,135,180,180,180,225,225,225,225};
// int BLTibeaAngle[] = {135,135,180,180,180,225,225,225,225};
int FRFemerAngle[] = {135,135,180,180,180,225,225,225,135};
int FRTibeaAngle[] = {135,135,180,180,180,225,225,225,135};


int FLFemerAngle[] = {180, 135, 135, 135, 225, 225, 225, 180, 180};
int FLTibeaAngle[] = {180, 135, 135, 135, 225, 225, 225, 180, 180};

// int BRFemerAngle[] = {180, 135, 135, 225, 225, 225, 225, 180, 180};
// int BRTibeaAngle[] = {180, 135, 135, 225, 225, 225, 225, 180, 180};

// int FRFemerAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 135};
// int FRTibeaAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 135};

int BLFemerAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 225};
int BLTibeaAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 225};


interval = 50;

  static int i = 0, j = 0.25*sizeof(RTibeaAngle)/sizeof(RTibeaAngle[0]), k = 0.5*sizeof(RTibeaAngle)/sizeof(RTibeaAngle[0]), l = 0.75*sizeof(RTibeaAngle)/sizeof(RTibeaAngle[0]);
  if (i >= sizeof(RTibeaAngle)/sizeof(RTibeaAngle[0])) {
    i = 0;
  }
  if (j >= sizeof(RTibeaAngle)/sizeof(RTibeaAngle[0])) {
    j = 0;
  }
  if (k >= sizeof(RTibeaAngle)/sizeof(RTibeaAngle[0])) {
    k = 0;
  }
  if (l >= sizeof(RTibeaAngle)/sizeof(RTibeaAngle[0])) {
    l = 0;
  }
  
  /* Set the position of the servos */
  //Back Left
  HCPCA9685.Servo(0, mapValues(BLTibeaAngle[i]));  
  HCPCA9685.Servo(1, mapValues(BLFemerAngle[i])-14);  
  //Back Right
  HCPCA9685.Servo(3, mapValues(BRTibeaAngle[j]-4));  
  HCPCA9685.Servo(4, mapValues(BRFemerAngle[j]-3));  
  //Front Left
  HCPCA9685.Servo(6, mapValues(FLTibeaAngle[k]));  
  HCPCA9685.Servo(7, mapValues(FLFemerAngle[k]));  
  //Front Right
  HCPCA9685.Servo(9, mapValues(FRTibeaAngle[l]-10));
  HCPCA9685.Servo(10, mapValues(FRFemerAngle[l])+18);

      /* Set the position of the servos */
  //Back Left
  HCPCA9685.Servo(0, mapValues(BLTibeaAngle[i]));  
  HCPCA9685.Servo(1, mapValues(BLFemerAngle[i])-14);  
  //Back Right
  HCPCA9685.Servo(3, mapValues(BRTibeaAngle[i]-4));  
  HCPCA9685.Servo(4, mapValues(BRFemerAngle[i]-3));  
  //Front Left
  HCPCA9685.Servo(6, mapValues(FLTibeaAngle[i]));  
  HCPCA9685.Servo(7, mapValues(FLFemerAngle[i]));  
  //Front Right
  HCPCA9685.Servo(9, mapValues(FRTibeaAngle[i]-10));
  HCPCA9685.Servo(10, mapValues(FRFemerAngle[i])+18);
  /* Update the loop variables */
  i++;
  j++;
  k++;
  l++;

  /* Wait for the motor to receive the command */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) {
    delay(interval - (currentMillis - previousMillis));
  }
  previousMillis = millis();
}


void Walk() {
int TibeaAngle[] = {180,225,225,225,135,135,135,180,180};

int BRFemerAngle[] = {180,225,225,225,135,135,135,180,180};
int BRTibeaAngle[] =  {180,225,225,225,135,135,135,180,180};

int FLFemerAngle[] = {180,225,225,135,135,135,135,180,180};
int FLTibeaAngle[] =  {180,225,225,135,135,135,135,180,180};


int BLFemerAngle[] = {135,135,180,180,180,225,225,225,225};
int BLTibeaAngle[] = {135,135,180,180,180,225,225,225,225};
int FRFemerAngle[] = {135,135,180,180,180,225,225,225,135};
int FRTibeaAngle[] = {135,135,180,180,180,225,225,225,135};

  static int i = 0;
  if (i >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    i = 0;
  }
  
  /* Set the position of the servos */
 HCPCA9685.Servo(0, mapValues(BLTibeaAngle[i]));  
  HCPCA9685.Servo(1, mapValues(BLFemerAngle[i])-14);  
  //Back Right
  HCPCA9685.Servo(3, mapValues(BRTibeaAngle[i]-4));  
  HCPCA9685.Servo(4, mapValues(BRFemerAngle[i]-3));  
  //Front Left
  HCPCA9685.Servo(6, mapValues(FLTibeaAngle[i]));  
  HCPCA9685.Servo(7, mapValues(FLFemerAngle[i]));  
  //Front Right
  HCPCA9685.Servo(9, mapValues(FRTibeaAngle[i]-10));
  HCPCA9685.Servo(10, mapValues(FRFemerAngle[i])+18);

  /* Update the loop variables */
  i++;

  /* Wait for the motor to receive the command */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) {
    delay(interval - (currentMillis - previousMillis));
  }
  previousMillis = millis();
}

void ReverseWalk() {
int TibeaAngle[] = {180,225,225,225,135,135,135,180,180};

int FLFemerAngle[] = {180, 135, 135, 135, 225, 225, 225, 180, 180};
int FLTibeaAngle[] = {180, 135, 135, 135, 225, 225, 225, 180, 180};

int BRFemerAngle[] = {180, 135, 135, 225, 225, 225, 225, 180, 180};
int BRTibeaAngle[] = {180, 135, 135, 225, 225, 225, 225, 180, 180};

int FRFemerAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 135};
int FRTibeaAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 135};

int BLFemerAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 225};
int BLTibeaAngle[] = {225, 225, 180, 180, 180, 135, 135, 135, 225};

  static int i = 0;
  if (i >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    i = 0;
  }
  
  /* Set the position of the servos */
 HCPCA9685.Servo(0, mapValues(BLTibeaAngle[i]));  
  HCPCA9685.Servo(1, mapValues(BLFemerAngle[i])-14);  
  //Back Right
  HCPCA9685.Servo(3, mapValues(BRTibeaAngle[i]-4));  
  HCPCA9685.Servo(4, mapValues(BRFemerAngle[i]-3));  
  //Front Left
  HCPCA9685.Servo(6, mapValues(FLTibeaAngle[i]));  
  HCPCA9685.Servo(7, mapValues(FLFemerAngle[i]));  
  //Front Right
  HCPCA9685.Servo(9, mapValues(FRTibeaAngle[i]-10));
  HCPCA9685.Servo(10, mapValues(FRFemerAngle[i])+18);

  /* Update the loop variables */
  i++;

  /* Wait for the motor to receive the command */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) {
    delay(interval - (currentMillis - previousMillis));
  }
  previousMillis = millis();
}



void setIntervalSpeed(int speed) {
  interval = speed;
}
// unsigned long McurrentMillis = millis();
//     unsigned long stateDuration = 0;

//     switch (currentState) {
//         case STORE:
//             Store();
//             stateDuration = 1000;  // run for 2 seconds
//             break;
//         case LEFT:
//             Left();
//             stateDuration = 1800;  // run for 1.8 seconds
//             break;
//         case RIGHT:
//             Right();
//             stateDuration = 1800;  // run for 1.8 seconds
//             break;
//         case WALK:
//             Walk();
//             stateDuration = 2800;  // run for 2.8 seconds
//             break;
//         case REVERSE:
//             ReverseWalk();
//             stateDuration = 2800;  // run for 2.8 seconds
//             break;
//         default:
//             break;
//     }

//     // check if current state's time is up
//     if (McurrentMillis - stateStartMillis >= stateDuration) {
//         // move to next state
//         currentState = static_cast<State>((currentState + 1) % NUM_STATES);
//         // reset timer
//         stateStartMillis = McurrentMillis;
//     }



// HEYO WALKING GAIT
void Walking() {
int FemerAngle[] = {200,160,100,300};

int TibeaAngle[] = {180,225,220,300};

// int FemerAngle[] = {180,180,180,180};
int FFemerAngle[] = {180,180,270,270};
int FTibeaAngle[] = {160,160,280,280};


int BFemerAngle[] = {270,270,270,270};
int BTibeaAngle[] = {270,270,270,270};
interval = 450;

  static int i = 0, j = 0.25*sizeof(TibeaAngle)/sizeof(TibeaAngle[0]), k = 0.5*sizeof(TibeaAngle)/sizeof(TibeaAngle[0]), l = 0.75*sizeof(TibeaAngle)/sizeof(TibeaAngle[0]);
  if (i >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    i = 0;
  }
  if (j >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    j = 0;
  }
  if (k >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    k = 0;
  }
  if (l >= sizeof(TibeaAngle)/sizeof(TibeaAngle[0])) {
    l = 0;
  }
  
  /* Set the position of the servos */
  //Back Left
  HCPCA9685.Servo(0, mapValues(BTibeaAngle[i]));  
  HCPCA9685.Servo(1, mapValues(BFemerAngle[i])-14);  
  //Back Right
  HCPCA9685.Servo(3, mapValues(BTibeaAngle[i]-4));  
  HCPCA9685.Servo(4, mapValues(BFemerAngle[i]-3));  
  //Front Left
  HCPCA9685.Servo(6, mapValues(FTibeaAngle[i]));  
  HCPCA9685.Servo(7, mapValues(FFemerAngle[i]));  
  //Front Right
  HCPCA9685.Servo(9, mapValues(FTibeaAngle[i]-10));
  HCPCA9685.Servo(10, mapValues(FFemerAngle[i])+18);


  /* Update the loop variables */
  i++;
  j++;
  k++;
  l++;

  /* Wait for the motor to receive the command */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) {
    delay(interval - (currentMillis - previousMillis));
  }
  previousMillis = millis();
}



