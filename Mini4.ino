/*
Mini Project 4
December 2, 2024
By, Tristyn Crawford
341544
*/

//Pin Definitions
//US sensor
const int USR_T = 10;
const int USR_E = 11;
const int USL_T = 12;
const int USL_E = 13;
//IR sensor
const int IR_F = A0;

//DC Motor
const int spin1 = 2;
const int DCL_F = 3;
const int DCL_B = 4;
const int spin2 = 7;
const int DCR_F = 5;
const int DCR_B = 6;

//Constants
const int obstacle_dist = 17;
const int wall_threshold = 8;
const int min_dist = 12;
const int hum_min = 20;
const int hum_max = 30;
const int speedmax = 250;
const int speedmin = 100;


//Variables
int front_dist = 0;
int left_dist = 0;
int right_dist = 0;

//Switch between modes automactically
enum RobotMode {Avoid_Obstacle, Follow_Wall, Human_Follow};
RobotMode currentMode;

//Boolean Variables
bool change = false;
bool wallL = false;
bool wallR = false;
bool human = false;


void setup() {
  //delcares all pins
  Serial.begin(9600);
  pinMode(USR_T, OUTPUT);
  pinMode(USR_E, INPUT);
  pinMode(USL_T, OUTPUT);
  pinMode(USL_E, INPUT);
  pinMode(spin1, OUTPUT);
  pinMode(DCL_F, OUTPUT);
  pinMode(DCL_B, OUTPUT);
  pinMode(spin2, OUTPUT);
  pinMode(DCR_F, OUTPUT);
  pinMode(DCR_B, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Read the sensor values
  right_dist = getUS(USR_T, USR_E);
  left_dist = getUS(USL_T, USL_E);
  front_dist = ReadIR(IR_F);


  // Debugging
  Serial.print("Front: ");
  Serial.print(front_dist);
  Serial.print(" || Left: ");
  Serial.print(left_dist);
  Serial.print(" || Right: ");
  Serial.println(right_dist);

  //determine which mode the robot is in
  if (front_dist < obstacle_dist){
    //for Avoiding obstacles
    currentMode = Avoid_Obstacle;
  } else if (left_dist < wall_threshold || right_dist < wall_threshold){
    //for following a wall
    currentMode = Follow_Wall;
  } else if (front_dist >= hum_min && front_dist <= hum_max) {
    //to follow a human
    currentMode = Human_Follow;
  } else {
    //if nothing the robot will just move forward
    moveForward(speedmax);
    return;
  }

  //switch case based on the mode
  switch (currentMode) {
    //for avoiding obstacles the corresponding function is run
    case Avoid_Obstacle:
      AvoidObj(front_dist, left_dist, right_dist);
      break;
    //for following a wall the corresponding function is run
    case Follow_Wall:
      wallfollowing(front_dist, left_dist, right_dist);
      break;
    //for followin a human the corresponding function is run
    case Human_Follow:
      Human(front_dist);
      break;

  }
  delay(50);
}

//Function to avoid obstacles
void AvoidObj(int front, int left, int right) {
  //for debugging purposes
  Serial.println("Avoiding |||||");
  change = true;
  //while there is a change the following loop will run
  while (change == true){
    //if the front distance is less than the obstacle distance and greater than the minimum distance
    if (front < obstacle_dist && front > min_dist) {
      //if left is less than right
      if (left > right) {
        //turn left
        turnLeft(speedmax);
      } else {
        //turn right
        turnRight(speedmax);
      }
    //if the front distance is less than the minimum distance the robot will move backwards
    } else if (front < min_dist){
      moveBackward(speedmax);
    //else it will move forward
    } else {
      moveForward(speedmax);
    }
    change = false;  //resetting the variable
  }
}

//Function to follow a wall
void wallfollowing(int front, int left, int right) {
  //for debugging
  Serial.println("Wall |||||");
  //if left is less than right 
  if (left < right) {
    //set to true
    wallL = true;
  } else {
    //set to true
    wallR = true;
  }
  //when wall is on the left
  while (wallL == true){
    //if left distance is less than the wall distance
    if (left < wall_threshold) {
      //turn right
      turnRight(speedmin);
    //if left distance is greater than the wall distance 
    } else if (left > wall_threshold) {
      //turn left
      turnLeft(speedmin);
    //if the front distance is less than the obstacle distance
    } else if (front < obstacle_dist) {
      //runs avoid obstacle function
      AvoidObj(front, left, right);
    //else the robot moves forward
    } else {
      moveForward(speedmin);
    }
    wallL = false;
  }
  //same as previous but for right wall
  while (wallR == true) {
    if (right < wall_threshold){
      turnLeft(speedmin);
    } else if (right > wall_threshold){
      turnRight(speedmin);
    } else if (front < obstacle_dist){
      AvoidObj(front, left, right);
    } else {
      moveForward(speedmin);
    }
    wallR = false;
  }
}

//Function to follow a Human/obstacle
void Human(int front) {
  //for debugging
  Serial.println("Human |||||");
  human = true;

  while (human == true){
    //to maintain a specific distance with the object
    if (front > (hum_max - 2)) {
      moveBackward(speedmax);
    } else if (front < (hum_min + 2)) {
      moveForward(speedmax);
    } else {
      moveForward((speedmin+50));
    }
    human = false;
  }
}

//Behaviour Functions
void moveForward(int speed){
  analogWrite(spin1, speed);
  analogWrite(spin2, speed);
  //pins are set the same for left and right
  digitalWrite(DCL_F, LOW);
  digitalWrite(DCL_B, HIGH);
  digitalWrite(DCR_F, LOW);
  digitalWrite(DCR_B, HIGH);
}

void moveBackward(int speed){
  analogWrite(spin1, speed);
  analogWrite(spin2, speed);
  //reversed of moveForward
  digitalWrite(DCL_F, HIGH);
  digitalWrite(DCL_B, LOW);
  digitalWrite(DCR_F, HIGH);
  digitalWrite(DCR_B, LOW);

}

void turnLeft(int speed){
  analogWrite(spin1, speed);
  analogWrite(spin2, speed);
  //pins are set to opposite on left and right
  digitalWrite(DCL_F, LOW);
  digitalWrite(DCL_B, HIGH);
  digitalWrite(DCR_F, HIGH);
  digitalWrite(DCR_B, LOW);
}

void turnRight(int speed){
  analogWrite(spin1, speed);
  analogWrite(spin2, speed);
  //reversed of turnLeft
  digitalWrite(DCL_F, HIGH);
  digitalWrite(DCL_B, LOW);
  digitalWrite(DCR_F, LOW);
  digitalWrite(DCR_B, HIGH);  

}

void stopMotors(){
  //stops the motor
  analogWrite(spin1, 0);
  analogWrite(spin2, 0);
}


// Function to measure distance from US sensor in CM
int getUS(int tpin, int epin) {
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

//Function to read IR value in CM
int ReadIR(int pin) {
  int sensorValue = analogRead(pin);
  // Convert the sensor value to distance (approximation based on datasheet)
  float voltage = sensorValue * (5.0 / 1023.0); // Convert ADC value to voltage
  int distance = 27.86 / (voltage - 0.1);       // Empirical formula for distance in cm
  distance = constrain(distance, 10, 80);       // Limit to sensor's valid range
  return distance;
}
