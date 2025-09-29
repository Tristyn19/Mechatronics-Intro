/*  Mini Project 3
    Due Nov 18

    By Tristyn Crawford

*/
//Library for Servo motor
//created my own for Mini 2 and using library for Mini 3
#include <Servo.h> 

Servo myservo;
// Define pins for the tens and ones 7-segment display
const int tensPins[] = {53,51,49,47,45,43,41};  // Pins connected to the segments of the tens display
const int onesPins[] = {52,50,48,46,44,42,40};  // Pins connected to the segments of the ones display

//Define the 1 button
const int bReq = 10;

//Define the pins for US sensor
const int tpin = 2;
const int epin = 3;

//Define the pin for the IR sensor
const int IRpin = A0;

// Motor Pins
const int speedpin = 8;   //PWM pin to control speed
const int dir1 = 13;
const int dir2 = 12;

//Servo
const int servopin = 11;   //sends signal to servo motor

//Integer Variables
int ones = 0;
int tens = 0;
int senVal = 0;
int req = 0;
int dis = 2000;
int curAngle = 0;

//Boolean Variables
bool rpress = false;
bool startup = false;

//Array for number lookup
const byte digitPattern[10] = {
  0b11111100,  //0
  0b01100000,  //1
  0b11011010,  //2
  0b11110010,  //3
  0b01100110,  //4
  0b10110110,  //5
  0b10111110,  //6
  0b11100000,  //7
  0b11111110,  //8
  0b11110110,  //9
};

//ADC value for each location
//IR lookup table from ADC to CM
const int IRlookup[][2] = {
  {520, 10},
  {280, 20},
  {210, 30},
  {155, 40},
};

//For US Samples
//the amount of samples per set
const int sample = 20;
//declaring the array for the 20 samples
float dis_sample[sample];
//setting the distance and standard deviation variables
float US_dist = 0;
float US_SD = 0;

//Startup code
void setup() {
  Serial.begin(9600);
  pinMode(tpin, OUTPUT);
  pinMode(epin, INPUT);
  pinMode(IRpin, INPUT);
  pinMode(servopin, OUTPUT);
  pinMode(speedpin, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(bReq, INPUT);
  //Set servo pin
  myservo.attach(servopin);

  // Set segment pins as output for tens and ones display
  for (int i = 0; i <= 6; i++) {
    pinMode(tensPins[i], OUTPUT);
  }

  for (int i = 0; i <= 6; i++) {
    pinMode(onesPins[i], OUTPUT);
  }
}

void loop() {
  // When Req Button is pressed it goes through the switch case
  if (digitalRead(bReq) == HIGH) {
    delay(500);
    switch (req) {    //dependant of the value of req
      case 0:         //when req = 0
        ReqA1();       //performs ReqA1 function
        break;
      case 1:         //when req = 1
        ReqA2();       //performs ReqA2 function
        break;
      case 2:         //when req = 2
        ReqA3();       //performs ReqA3 function
        break;
      case 3:        //when req = 3
        ReqB1();     //performs ReqB1 function
        break;
      case 4:        //when req = 4
        ReqB2();     //performs ReqB2 function
        break;
    }
    //loop increases req by 1 every time req button is pressed
    if (req < 4) {
      req++;
    } else { //it resets back to 0
      req = 0;
    }
  } else {                         //Displays on  serial monitor to start the program loop
    if (startup == false) {         //Only display once
      Serial.println("Press Requirement Button");
      startup = true;
    }
  }
}
//Requirement A1 function
void ReqA1() {
  //displays on serial monitor
  Serial.print("Requirement A1");
  Serial.print('\n');
  delay(dis);

  rpress = true;
  //Will run until the button is pressed again
  while (digitalRead(bReq) == LOW) {
    //read the ADC value from the IR sensor
    senVal = analogRead(IRpin);
    //Uses the IR table to convert the ADC value to the CM value
    int IR_dist = IR_Table(senVal);
    //When value is within Range 10cm to 40cm
    if (IR_dist >= 10 && IR_dist <= 40) {
      // Print distance and angle to the Serial Monitor
      Serial.print("IR Reading: ");
      Serial.print(senVal);
      Serial.print("| | Distance: ");
      Serial.print(IR_dist);
      Serial.print('\n');
      //Determines the ones and tens digit for the 7 segment display
      tens = IR_dist / 10;
      ones = IR_dist * 10;
      //Displays the number to the 7 segment display
      displayDigit(ones, onesPins);
      displayDigit(tens, tensPins);
    } else {
      //prints out of range
      Serial.println("Out of Range");
    }

    //Small delay between samples
    delay(800);
    //resets boolean variable
    rpress = false;

  } //end while

}

//Requirement A2 function
void ReqA2() {
  //prints which requirement on Serial monitor
  Serial.print('\n');
  Serial.print("Requirement A2");
  Serial.print('\n');
  delay(dis);

  rpress = true;
  //Runs until button is pressed again
  while (digitalRead(bReq) == LOW) {
    //reads ADC value of the IR sensor
    senVal = analogRead(IRpin);
    //Uses the IR table to convert the ADC value to CM value
    int IR_dist = IR_Table(senVal);
    //When in range of 10cm to 40cm 
    if (IR_dist >= 10 && IR_dist <= 40) {
      //calculates the Servo Angle based on CM value
      curAngle = 2 * (IR_dist - 10);
      // Print distance and angle to the Serial Monitor
      Serial.print("IR Reading: ");
      Serial.print(senVal);
      Serial.print("| | Distance: ");
      Serial.print(IR_dist);
      Serial.print(" cm | | Servo Angle: ");
      Serial.print(curAngle);
      Serial.println(" degrees");
    } else {
      //Sets values to max value when out of range
      Serial.println("Out of Range");
      if (curAngle < 0) curAngle = 0;
      if (curAngle > 60) curAngle = 60;
    }
    //setting the angle to the Servo motor
    myservo.write(curAngle);
    //determining the tens and ones digit
    tens = curAngle / 10;
    ones = curAngle * 10;
    //displays the digit on the 7 segment display
    displayDigit(tens, tensPins);
    displayDigit(ones, onesPins);
    //delay between readings
    delay(800); 
    //resets value
    rpress = false;

  } //end while
  //sets servo to 0 degrees
  myservo.write(0);

}
// Function to find the distance from the lookup table
//Uses the math functino of interpolation to determine the required output
int IR_Table(int rawValue) {
  for (int i = 0; i < sizeof(IRlookup) / sizeof(IRlookup[0]) - 1; i++) {
    //determines which value in the array is used
    int val1 = IRlookup[i][0];
    int dist1 = IRlookup[i][1];
    int val2 = IRlookup[i + 1][0];
    int dist2 = IRlookup[i + 1][1];

    // Interpolate distance for rawValue between val1 and val2
    if (rawValue <= val1 && rawValue >= val2) {
      return dist1 + (rawValue - val1) * (dist2 - dist1) / (val2 - val1);
    }
  }
    return 0; // Default return if out of range
}

//Requirement A3 function
void ReqA3() {
  //Shows which function is running
  Serial.print('\n');
  Serial.print("Requirement A3");
  Serial.print('\n');
  delay(dis);
  //sets requirement to true
  rpress = true;
  //runs until the button is pressed again
  while (digitalRead(bReq) == LOW) {
    //reads the ADC value from the IR sensor
    senVal = analogRead(IRpin);
    //Determines the CM value from the predetermined equation
    int IR_dist = MathIR(senVal);
    //when in range 10cm to 40cm
    if (IR_dist >= 10 && IR_dist <= 40) {
      //calculates the Angle based on the distance 
      curAngle = 2 * (IR_dist - 10);
      // Print distance and angle to the Serial Monitor
      Serial.print("IR Reading: ");
      Serial.print(senVal);
      Serial.print("| | Distance: ");
      Serial.print(IR_dist);
      Serial.print(" cm | | Servo Angle: ");
      Serial.print(curAngle);
      Serial.println(" degrees");
    } else {
      //sets values to max when out of range
      Serial.println("Out of Range");
      if (curAngle < 0) curAngle = 0;
      if (curAngle > 60) curAngle = 60;
    }
    //Writes the angle to the servo motor
    myservo.write(curAngle);
    //determing the ones and tens digit
    tens = curAngle / 10;
    ones = curAngle * 10;
    //writes to the 7 segment display
    displayDigit(tens, tensPins);
    displayDigit(ones, onesPins);
    //small delay between samples
    delay(800);
    //resets the value
    rpress = false;

  } // end while
  //sets the servo to angle 0
  myservo.write(0);

}
//Math Estimation function
int MathIR(int val) {
  /* From the table and inputing the values into excel
    Using excel to create a scatter plot graph
    determining the best fit line to make an equation to solve for y
    Using expoential line the equation is y = 14426x^-1.163
    Where y = cm and x = ADC value
  */
  float x = pow(val, -1.163);
  float distance = (14426 * x);
  //Returing an intiger value instead of a float value
  int rdist = (int)distance;
  return rdist;
}

//Requirement B1 function
void ReqB1() {
  //Displays which function is running
  Serial.print('\n');
  Serial.print("Requirement B1");
  Serial.print('\n');
  delay(dis);
  //sets requirement to true
  rpress = true;
  //runs until the button is pressed again
  while (digitalRead(bReq) == LOW) {
    //Collect 20 samples
    Serial.println("Starting New Sample");
    for (int i = 0; i < sample; i++) {
      dis_sample[i] = getUS();
      delay(100); // Small delay between samples
    }
    // Calculate mean using the created function
    US_dist = MeanVal(dis_sample, sample);
    //When in range of 0cm to 50cm
    if (US_dist >= 0 && US_dist <= 50){
      //calculated Standard Deviation using the created function
      US_SD = SDVal(dis_sample, sample, US_dist);
      //determines the tens and ones digit 
      tens = (int)US_dist / 10;
      ones = (int)US_dist * 10;
      //writes the digits to 7 segment display
      displayDigit(tens, tensPins);
      displayDigit(ones, onesPins);
      //Prints to Serial monitor
      Serial.print("Mean Value: ");
      Serial.print(US_dist);
      Serial.print(" | | Standard Deviation: ");
      Serial.println(US_SD);
    } else {
      //when out of range prints Out of Range
      Serial.println("Out of Range");
    }
    //Small Delay between sample sets to ensure proper viewing
    Serial.print("Waiting 3sec before next sample set");
    //Delay function for 3 seconds printing a . for every second
    for (int j = 0; j < 3; j++) {
      Serial.print(".");
      delay(1000);
    }

    Serial.println(".");
    //resets the value
    rpress = true;

  } // end while

}
//Requirement B2 function
void ReqB2() {
  //Displays which requirement is running
  Serial.print('\n');
  Serial.print("Requirement B2");
  Serial.print('\n');
  delay(dis);
  //sets requirement to true
  rpress = true;
  //runs until button is pressed again
  while (digitalRead(bReq) == LOW) {
    //0-255 is 8bit PWM
    //0-1023 is 10bit PWM
    //Getting the 20 sample set
    Serial.println("Starting New Sample");
    for (int i = 0; i < sample; i++) {
      dis_sample[i] = getUS();
      delay(100); // Small delay between samples
    }
    //Calculate mean using the created function
    US_dist = MeanVal(dis_sample, sample);
    //when in range 0 to 50
    if (US_dist >= 0 && US_dist <= 50) {
      //determines the ones and tens digit
      tens = (int)US_dist / 10;
      ones = (int)US_dist * 10;
      //writes the digits to 7 segment
      displayDigit(tens, tensPins);
      displayDigit(ones, onesPins);
      //DC motor is an 8 bit as it only reads values from 0 to 255
      //Using the given equation to calculate DC speed from distance
      int DCspeed = 5 * US_dist;
      //Setting the speed to the motor
      analogWrite(speedpin, DCspeed);
      //determining the DC direction
      digitalWrite(dir1, HIGH);
      digitalWrite(dir2, LOW);
      //prints to the serial monitor
      Serial.print("Mean Value: ");
      Serial.print(US_dist);
      Serial.print(" | | DC Speed: ");
      Serial.println(DCspeed);
    } else {
      //when out of range
      Serial.println("Out of Range");
    }
    //Waits 5 seconds between each set of samples
    Serial.print("Waiting 5sec before next sample set");
    //to print . every second for 5 seconds
    for (int j = 0; j < 5; j++) {
      Serial.print(".");
      delay(1000);
    }

    Serial.println(".");

    rpress = false;

  } // end while
  //sets the DC motor to off when not in use
  analogWrite(speedpin, 0);

}

// Function to calculate the mean
float MeanVal(float data[], int size) {
  //initializing the sum of all the data
  float sum = 0;
  //calcualting the sum of all the data
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  //determining the mean from sum / sample size
  return sum / size;
}

// Function to calculate the standard deviation
float SDVal(float data[], int size, float meanValue) {
  //initializing the sum for all the data
  float sum = 0;
  /*calculating each data point accourding to the standard deviation calulation
    sd = sqrt ((x - x2^2) / n
    x = data point
    x2 = mean
    n = sample size
  */
  for (int i = 0; i < size; i++) {
    sum += pow(data[i] - meanValue, 2);
  }
  return sqrt(sum / size);
}

// Function to measure distance from US sensor
float getUS() {
  //initializes the Trigger Pin with small delays
  digitalWrite(tpin, LOW);
  delayMicroseconds(2);
  digitalWrite(tpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(tpin, LOW);
  //Duration in seconds is set to the Echo pin
  long duration = pulseIn(epin, HIGH);
  //Determining the distance from time using the given equation
  float distance = (duration * 0.0343) / 2; // Convert time to distance (cm)
  return distance;
}

// Function to display a digit on a 7-segment display
void displayDigit(byte digit, const int segmentPins[]) {
  byte pattern = digitPattern[digit];  // Get the pattern for the digit
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (pattern >> (7 - i)) & 0x01);  // Write each segment state
  }
}