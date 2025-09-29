/*  Mini Project 2
    Due Nov 4

    By Tristyn Crawford

*/

// Define pins for the tens and ones 7-segment display
const int tensPins[] = {53,51,49,47,45,43,41};  // Pins connected to the segments of the tens display
const int onesPins[] = {52,50,48,46,44,42,40};  // Pins connected to the segments of the ones display
const int dP1 = 35;   //pin for decimal point

//Defining pins for Buttons and Potentiometers
const int potPin = A0;   //For Req 3 - A value

const int bInc = 10;
const int bDec = 11;
const int bRes = 12;
const int bReq = 13;
const int bStep = 6;

// Motor Pins
const int speedpin = 9;   //PWM pin to control speed
const int pindir1 = 2;    //controls 1 direction
const int pindir2 = 3;    //controls the other direction
//Servo
const int servopin = 8;   //sends signal to servo motor

//integer variables to store data
int tens = 0;
int ones = 0;
int req = 0;
int dis = 2000;
int curAngle = 0;
int gear = 1;
int speedper = 0;
int A = 0;
int counter = 0;

int steps[] = {1,2,4,5,10};

//Array for #3 for the 10 gears maximum speed
int maxspeeds[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
const int revmax = 35;    //sets reverse speed to 35
const int maxsteer = 60;  //maximum steering angle

//boolean factors
bool direction = false;
bool rpress = false;
bool dpcheck = false;
bool spress = false;
bool ipress = false;
bool dpress = false;
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

void setup() {

  Serial.print("Press Requirement Button");
  Serial.print('\n');

  //setting up outputs and inputs
  pinMode(servopin, OUTPUT);
  pinMode(speedpin, OUTPUT);
  pinMode(pindir1, OUTPUT);
  pinMode(pindir2, OUTPUT);
  pinMode(dP1, OUTPUT);
  pinMode(bInc, INPUT);
  pinMode(bDec, INPUT);
  pinMode(bRes, INPUT);
  pinMode(bReq, INPUT);

  // Set segment pins as output for tens and ones display
  for (int i = 0; i <= 6; i++) {
    pinMode(tensPins[i], OUTPUT);
  }

  for (int i = 0; i <= 6; i++) {
    pinMode(onesPins[i], OUTPUT);
  }

  //begins the program and allows to print to serial monitor
  Serial.begin(9600);

}

void loop() {
  // main loop to go through selected requirement
  // When Req Button is pressed it goes through the switch case
  if (digitalRead(bReq) == HIGH) {
    delay(500);
    switch (req) {    //dependant of the value of req
      case 0:         //when req = 0
        Req1();       //performs Req1 function
        break;
      case 1:         //when req = 1
        Req2();       //performs Req2 function
        break;
      case 2:         //when req = 2
        Req3();       //performs Req3 function
        break;
    }
    //loop increases req by 1 every time req button is pressed
    if (req < 2) {
      req++;
    } else { //once reached req = 3, it resets back to 0
      req = 0;
    }
  } else {                         //Displays on  serial monitor to start the program loop
    if (startup == false) {         //Only display once
      Serial.print("Press Requirement Button");
      Serial.print('\n');
      startup = true;
    }
  }
}

//Function for Requirement # 1
void Req1() {
  //Prints to Serial Monitor to notify that you are in this function
  Serial.print("Requirement 1");
  Serial.print('\n');
  delay(dis);
  //sets requirement to true
  rpress = true;
  //loop runs until bReg is pressed
  while (digitalRead(bReq) == LOW){
    //setting the potentiometer values
    int potVal = analogRead(potPin);    //reading the potentiometer
    int val = map(potVal, 0, 1023, 0, 255);   //scalling the value from potentiometer
  
    if (val < 128) {                   //when potentiometer on first half
      speedper = map(val, 0, 127, 99, 0);  //setting the speed percentage
      digitalWrite(pindir1, HIGH);         // Sets DC motor to Clockwise
      digitalWrite(pindir2, LOW);
      //Prints Scaled speed on Serial Monitor
      Serial.print(" | CW Scaled Speed: ");
      Serial.print(speedper);
      Serial.println("%");

    } else {                               //when potentiometer is on second half
      speedper = map(val, 128, 255, 0, 99);  //setting the speed percentage
      digitalWrite(pindir1, LOW);          //Sets DC motor to Counter Clockwise
      digitalWrite(pindir2, HIGH);
      //Prints Scaled speed to serial monitor
      Serial.print(" | CCW Scaled Speed: ");
      Serial.print(speedper);
      Serial.println("%");
    }
    int PWMval = map(speedper, 0, 99, 0, 255);  //sets the PWM value based on the speed percentage
    //sends to DC motor
    analogWrite(speedpin, PWMval);
    // Print the PWM value and speed percentage to the serial monitor
    Serial.print(" | PWM Value: ");
    Serial.print(PWMval);
    //Displays Digit
    //Sets the units fot the 7 segment display
    tens = speedper / 10;
    ones = speedper % 10;
    displayDigit(tens, tensPins);
    displayDigit(ones, onesPins);

    rpress = false;    //sets back to false
    delay(250);

  }  //end while
  //resets DC motor to off
  analogWrite(speedpin, 0);
}
// Function to display a digit on a 7-segment display
void displayDigit(byte digit, const int segmentPins[]) {
  byte pattern = digitPattern[digit];  // Get the pattern for the digit
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (pattern >> (7 - i)) & 0x01);  // Write each segment state
  }
}
//Function for Requirement #2
void Req2() {
  //Prints to Serial Monitor to notify that you are in this function
  Serial.print('\n');
  Serial.print("Requirement 2");
  Serial.print('\n');
  delay(dis);
  //sets value to true
  rpress = true;
  //Runs until bReq is pressed again
  while (digitalRead(bReq) == LOW) {
    //When angle is less than 90 add 1 to angle
    if (digitalRead(bStep) == HIGH){
      spress = true;
      delay(500);
    }
    if (digitalRead(bInc) == HIGH) {
      ipress = true;
      delay(500);
    }
    //when angle is greater than -90 minus 1 to angle
    if (digitalRead(bDec) == HIGH) {
      dpress = true;
      delay(500);
    }
    //checking if angle value is negative
    if (A < 0) {
      dpcheck = true;
    }
    //If one of the buttons is pressed
    if (spress == true || ipress == true || dpress == true){
      //determines the value int the array
      int valA = steps[counter];
      //when bInc is pressed
      if(ipress == true){
        //calculates the angle
        A = A + valA;
        //if angle is greater than 90 it resets to 0
        if (A > 90) {
          A = 0;
        }
        ipress = false; //resets the variable
      }
      //when bDec is pressed
      if (dpress == true) {
        //calculates the angle
        A = A - valA;
        //if angle is less than -90 it resets to zero
        if (A < -90) {
          A = 0;
        }
        dpress = false;  //resets the variable
      }
      //if bStep is pressed
      if (spress == true) {
        //if the counter is less than 4 add 1 to counter
        if (counter < 4) {
          counter++; 
        } else {   //resets back to zero
          counter = 0;
        }
        spress = false;  //resets the variable
      }
    }
    //resets angle to 0
    if (digitalRead(bRes) == HIGH) {
      A = 0;
      delay(100);                            
    }
    //writes angle to Servo Motor
    servoWrite1(A);
    //Prints angle value to Serial Monitor
    Serial.print("Angle: ");
    Serial.print(A);
    Serial.print('\n');
    //Sets up units for 7 segement display
    tens = abs(A) / 10;
    ones = abs(A) % 10;
    //display number on 7 segment display
    displayDigit(tens, tensPins);
    displayDigit(ones, onesPins);
    //When negative angle Decimal Point is turned on
    if (dpcheck == true) {
      digitalWrite(dP1, HIGH);
      dpcheck = false;  //resets value
    } else {   //turns it off
      digitalWrite(dP1, LOW);
    }
    rpress = false;  //resets value
    delay(250);
  } // end of while 
}
//Function to Connect to Servo Motor
void servoWrite1(int angle) {
  //Declares the pulsewidth based on the angle to the range
  int pulseWidth = map(angle, -90, 90, 1000, 2000);
  
  digitalWrite(servopin, HIGH);           // Start pulse
  delayMicroseconds(pulseWidth);          // Hold pulse for duration
  digitalWrite(servopin, LOW);            // End pulse
  
  delay(20);  // Wait for 20 milliseconds before the next signal
}
//Function for Requirement 3
void Req3() {
  //Prints to Serial Monitor to notify that you are in this function
  Serial.print('\n');
  Serial.print("Requirement 3");
  Serial.print('\n');
  delay(dis);
  //sets requirement to true
  rpress = true;
  //Runs until bReq is pressed again
  while (digitalRead(bReq) == LOW){
    //bRes changes the direction from Reverse to Forward
    if (digitalRead(bRes) == HIGH) {
      direction = !direction;
      delay(200);
    }
    //when direction is Forward
    if (direction) {
      //when bInc is pressed
      if (digitalRead(bInc) == HIGH) {
        //When gear value is less than 10 add 1 to gear
        if (gear < 10) {
          gear++;
          delay(200);
        }
      }
      //when bDec is pressed
      if (digitalRead(bDec) == HIGH) {
        //when gear value is greater than 1 minus 1 to gear
        if (gear > 1) {
          gear--;
          delay(200);
        }
      }
    }
    //reads and sets the potentiometer value
    int potValue = analogRead(potPin);
    int potVal = map(potValue, 0, 1023, 0, 255);
    //
    if (potVal <= 127){
      speedper = map(potVal, 0, 127, 0, 99);
    } else {
      speedper = map(potVal, 128, 255, 99, 0);
    }

    //sets direction and speed
    int speedMax;
    //when bRes is pressed
    if (!direction){
      //Max speed is equal to the preset Reverse Max speed
      speedMax = revmax;
      //sets the gear to display 1
      gear = 1;
      //Displays the decimal point on 7 segment
      digitalWrite(dP1, HIGH);
    } else {   //for forward direction
      //setting Max speed to value in Max Speed Array for the 10 preset values
      speedMax = maxspeeds[gear - 1];
      //turns the decimal point off
      digitalWrite(dP1, LOW);
    }

    //calculates the scaled speed based off the percentage and Max speed
    int scaledspeed = (speedper * speedMax) / 99;

    //For reverse or forward angle
    if (!direction) {
      // Reverse: for half of the potentiometer
      if (potVal <= 127) {
        //sets the angle from scaledspeed based on Max speed from 60 - 0
        curAngle = map(scaledspeed, 0, speedMax, 60, 0);
      } else {    //For the other half of the potentiometer
        //sets the angle from scaledspeed based on Max speed from -60 - 0
        curAngle = map(scaledspeed, 0, speedMax, -60, 0);
      }
      //Setting DC motor to turn Counter Clockwise
      digitalWrite(pindir1, LOW);
      digitalWrite(pindir2, HIGH);
      Serial.print("Reverse | ");
    } else {     //For forward direction
      //for half of the potentiometer
      if (potVal <= 127) {
        //sets the angle from scaledspeed based on Max speed from -60 - 0
        curAngle = map(scaledspeed, 0, speedMax, -60, 0);
      } else {   //for the other half of the potentiometer
        //sets the angle from scaledspeed based on Max speed from 60 - 0
        curAngle = map(scaledspeed, 0, speedMax, 60, 0);
      }
      //sets DC motor to Clockwise direction
      digitalWrite(pindir1, HIGH);
      digitalWrite(pindir2, LOW);
      Serial.print("Forward | ");
    }
    
    //Calculates angle based on scaled speed
    int angleAbs = abs(curAngle);   //getting absolute value - gets rid of negatives
    float refactor = (float)angleAbs / maxsteer;  //calculating the ratio between angle and max angle
    int speedadj = int(scaledspeed * (1 - refactor));  //calculates the adjusted speed
    int newspeed = map(speedadj, 0, 99, 50, 250);  //sets the new speed for the DC motor
    //setting DC motor speed to the newspeed
    analogWrite(speedpin, newspeed);
    //Displays Angle on the Servo Motor
    servoWrite2(curAngle);
    //sets units for 7 segment display
    tens = speedper / 10;
    ones = speedper % 10;
    //displays on 7 segment display
    displayDigit(tens, tensPins);
    displayDigit(ones, onesPins);
    //Prints Gear, Scaled Speed, Adjusted Speed, Speed % and Steering Angle to serial monitor
    Serial.print(" | Gear: ");
    Serial.print(gear); // Show 'R' for reverse
    Serial.print(" | Base Speed: ");
    Serial.print(scaledspeed);
    Serial.print(" | Adjusted Speed: ");
    Serial.print(speedadj);
    Serial.print(" | Speed Percent: ");
    Serial.print(speedper);
    Serial.print(" | Steering Angle: ");
    Serial.println(curAngle);

    delay(250);

    rpress = false;  //resets value
  }  // end while
  
}
//Function to connect to Servo Motor for 3rd requirement
void servoWrite2(int angle){
  //Declares the pulsewidth based on the angle to the range
  int pulseWidth = map(angle, -60, 60, 1000, 2000);
  
  digitalWrite(servopin, HIGH);           // Start pulse
  delayMicroseconds(pulseWidth);          // Hold pulse for duration
  digitalWrite(servopin, LOW);            // End pulse
  
  delay(20);  // Wait for 20 milliseconds before the next signal
}