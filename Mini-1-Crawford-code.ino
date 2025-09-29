/* Mini Project 1 Code
    Due Oct 21, 2024

    by Tristyn Crawford

*/

#include <LiquidCrystal.h>   // library for the LCD display

LiquidCrystal lcd(2,3,4,5,6,7);  //declaring the library and which pins are being used (RS, E, D4, D5, D6, D7)

// Define pins for the tens and ones 7-segment display
const int tensPins[] = {53,51,49,47,45,43,41};  // Pins connected to the segments of the tens display
const int onesPins[] = {52,50,48,46,44,42,40};  // Pins connected to the segments of the ones display
const int dP1 = 35;   //pin for decimal point

//Defining pins for Buttons and Potentiometers
const int potA= A0;   //For Req 3 - A value
const int potB = A1;  //For Req 3 - B value

const int bA = 9;     //pin for Button U (in code A)
const int bB = 10;     //pin for Button T (in code B)
const int bMode = 11;  //pin for Button M (in code Mode)
const int bSel = 12;  //pin for Button S/R (in code as Reset or Select Counter)
const int bReq = 13;  //pin for Button Req (controls which Requirement is being used)

//setting integer Variables
int A = 0;     //used for 1st digit   
int B = 0;     //used for 2nd digit
int valA = 0;  //used for value in steps array
int mode = 1;  //used to change which mode
int counter = 0;  //used to determine which value in steps array
int req = 0;   //used to change which requirement
int dis = 2000;  //used to set delay times for buttons

// Used to store the ones and tens value for each requirement to display on 7 segment
int tens = 0;    
int ones = 0;

//setting boolean variables
bool Opress = false;   //to determine if requirement button was pressed
bool apress = false;   //to determine if A button was pressed
bool bpress = false;   //to determine if B button was pressed
bool mpress = false;   //to determine if Mode button was pressed
bool spress = false;   //to determine if Select/Reset button was pressed
bool change = false;   //to determine if there was a change with the potentiometers

//Array for number lookup and for requirement 4
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

int steps[] = {1,2,4,5,10};

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);      //initializing the LCD to start
  lcd.setCursor(0,0);   //setting the location of Text to 0,0
  lcd.print("Press Req Button");  //printing on LCD at 0,0

  //setting up all the buttons to be able to read
  pinMode(bA, INPUT);
  pinMode(bB, INPUT);
  pinMode(bMode, INPUT);
  pinMode(bSel, INPUT);
  pinMode(bReq, INPUT);

  pinMode(dP1, OUTPUT);

  // Set segment pins as output for tens and ones display
  for (int i = 53; i >= 41; i = i - 2) {
    pinMode(tensPins[i], OUTPUT);
  }

  for (int i = 52; i <= 40; i = i - 2) {
    pinMode(onesPins[i], OUTPUT);
  }

  //begins the program
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Loop cycles through the 4 requirements
  if (digitalRead(bReq) == HIGH){    //when Req button is pressed
    delay(500);
    lcd.clear();
    switch (req) {    //chose case depending on value of req
      case 0:         //req = 0
        Req1();      //run requirement 1 code then break out of it when req changes
        break;  
      case 1:        //req = 1
        Req2();      //run requirement 2 code then break out of it when req changes
        break;
      case 2:      //req = 2
        Req3();    //run requirement 3 code then break out of it when req changes
        break;
      case 3:     //req = 3
        Req4();   //run requirement 4 code then break out of it when req changes
        break;
    }
    //loop increases req by 1 every time req button is pressed
    if (req < 3) {
      req++;
    } else { //once reached req = 3, it resets back to 0
      req = 0;
    }

  } else {                         //Displays on LCD screen to start the program loop
    lcd.setCursor(0,0);
    lcd.print("Press Req Button");
  }

}

 //Funtion for Requirement 1
void Req1() {
  //sets LCD to display Req 1 before starting loop (notifies which requirement is currently in use)
  lcd.setCursor(0,0);
  lcd.print("Req: 1");
  delay(dis);
  //resetting the LCD display
  lcd.clear();

  //sets requirement to true
  Opress = true;

  //Will perform until Req button is pressed
  while (digitalRead(bReq) == LOW){
    //Determining if A button was pressed
    if(digitalRead(bA) == HIGH){
      apress = true;
      delay(500);
    }
    //Determining if B button was pressed
    if (digitalRead(bB) == HIGH) {
      bpress = true;
      delay(500);
    }
    //Determining if reset button was pressed
    if(digitalRead(bSel) == HIGH) {
      spress = true;
      delay(500);
    }
    //based on which button is pressed by or statement (only 1 needs to be true to run)
    if (Opress == true || apress == true || bpress == true || spress == true) {
      //when reset button is pressed
      if (spress == true){
        //when A or B doesnt equal zero, they reset to zero
        if (A != 0 || B != 0){
          A = 0;
          B = 0;
        }
        spress = false; //resets button
      }
      //when A is pressed
      if (apress == true){
        //when A is less than 9, add 1 to A
        if (A < 9){
            A++;
        } else{      //A resets to zero
            A = 0;
        }
        apress = false;  //resets button
        }
      //when B is pressed
      if (bpress == true){
        //when B is less than 9, add 1 to B
        if (B < 9) {
            B++;
        //when B = 9 and A < 9, add 1 to A and b resets to 0 (when b is pressed 10 times it will display A = 1, B = 0)
        } else if (B == 9 && A < 9) {
            A++;
            B = 0;
        //Once A = 10, resets to zero, max number displayed is 99
        } else {
            A = 0;
            B = 0;
        }
        bpress = false;  //resets button
      }
      //using function to display code
      displayValue(A,B);
      //Display on 7 segment Displays
      displayDigit(A, tensPins);
      displayDigit(B, onesPins);
      Opress = false;  //resets button
    }
  }  //end while and resetting values
  A = 0;
  B = 0;
}

//Function to display A as tens and B as ones
void displayValue(int a, int b) {
  int total = (a * 10) + b;   //Total of A(tens) + B(ones)
  //using strings to make printing easier
  String a1 = "A: " + String(a) + '\n';
  String b1 = "B: " + String(b) + '\n';
  String t1 = "Total: " + String(total) + '\n';
  //printing to serial monitor
  Serial.print(a1);
  Serial.print(b1);
  Serial.print(t1);
  Serial.print("Binary: ");
  Serial.print(total, BIN);
  Serial.print('\n');
  //Setting the LCD to display, A, B and total
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A: ");
  lcd.print(A);
  lcd.print(" B: ");
  lcd.print(B);
  lcd.setCursor(0,1);
  lcd.print("Total: ");
  lcd.print(total);
}

// Function to display a digit on a 7-segment display
void displayDigit(byte digit, const int segmentPins[]) {
  byte pattern = digitPattern[digit];  // Get the pattern for the digit
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (pattern >> (7 - i)) & 0x01);  // Write each segment state
  }
}

//Function for Requirement 2
void Req2() {
  //sets LCD to display Req 2 before starting loop (notifies which requirement is currently in use)
  lcd.setCursor(0,0);
  lcd.print("Req: 2");
  delay(5000);
  //resetting LCD display
  lcd.clear();
  Opress = true;  //sets requirement to true
  //Sets 7 segment to 0 0
  displayDigit(0, onesPins);
  displayDigit(0, tensPins);
  //checking if the calculated value has a decimal point
  bool dpcheck = false;

  //Loop will run until Req button is pressed
  while (digitalRead(bReq) == LOW) {
    //Determining if A was pressed
    if (digitalRead(bA) == HIGH){
      apress = true;
      delay(500);
    }
    //Determining if B was pressed
    if (digitalRead(bB) == HIGH) {
      bpress = true;
      delay(500);
    }
    //Determining if Mode was pressed
    if (digitalRead(bMode) == HIGH) {
      mpress = true;
      delay(500);
    }
    //Determining if Reset was pressed
    if (digitalRead(bSel) == HIGH) {
      spress = true;
      delay(500);
    }
    //Run loop when 1 button is pressed (uses or statement)
    if (Opress == true || apress == true || bpress == true || mpress == true || spress == true) {
      //when Reset is pressed
      if (spress == true) {
        //A or B is not 0, then everything resets
        if (A != 0 || B != 0) {
          A = 0;
          B = 0;
          mode = 1;
        }
        spress = false;  //resets button
      }
      //when mode is pressed
      if (mpress == true) {
        //mode < 4, add 1 to mode
        if (mode < 4) {
          mode++;
          //mode = 4, resets to 1
        } else{
          mode = 1;
        }
        mpress = false;  //resets button
      }
      //when A is pressed
      if (apress == true) {
        //A < 9, add 1 to A
        if (A < 9){
          A++;
          // A = 10, resets to 0
        } else {
          A = 0;
        }
        apress = false;  //resets button
      }
      //when B is pressed
      if (bpress == true) {
        //B < 9, add 1 to B
        if (B < 9) {
          B++;
          //B = 10 and A < 9, add 1 to A and reset B
        } else if (B == 9 && A < 9) {
          A++;
          B = 0;
          //Max number is 99, then resets
        } else {
          A = 0;
          B = 0;
        }
        bpress = false;  //resets button
      }
      Opress = false;  //resets button
      float result = Mode(A, B, mode);  //Calculating answer using function
      //Checking if the result has a decimal point (real number)
      float num = (result * 10);    //multiplies result by 10 to create whole number
      int dec = (int)num % 10;      //finding the modulus of that number
      //if there is a modulus
      if (dec > 0 ) {   //when there is a decimal 
        dpcheck = true;
      } 
      //Print either ones or decimal value
      if (result >= 0 && result <= 9) {
        //when there is a decimal point
        if (dpcheck == true) {
          tens = (int)result;  //tens is equal tot he first digit
          ones = dec;          //ones is equal to the remainder digit
        } else {               //when not a decimal (whole number)
          tens = 0;               //ten digit is set to 0
          ones = (int)result;     //ones digit is the calulated result
        }
      } else if (result >= 10) {     //when the result is greater than 9
        tens = (int)result / 10;     //delaring the tens digit
        ones = (int)result % 10;     //declaring the ones digit
      } else {                     //everything else is 0
        tens = 0;
        ones = 0;
      }
      //display answer on 7 segment display
      displayDigit(ones, onesPins);
      displayDigit(tens, tensPins);
      //only turn on decimal point when there is a decimal
      if (dpcheck == true) {
        digitalWrite(dP1, HIGH);
        dpcheck = false;        //resets checked value
      } else {                  //everything else the decimal is off
        digitalWrite(dP1, LOW);
      }
      //Function to Display the values of A, B, mode, and result on LCD and Serial monitor
      displayValue2(A, B, mode, result);
    }
  }  //end while  && resets value before next req.
  A = 0;
  B = 0;
  mode = 1;
}
//Function to display A, B, Mode, and result on LCD and Serial Monitor
void displayValue2(int a, int b, int md, float r) {
      //display on serial monitor
      String m1 = "Mode: " + String(md) + '\n';
      Serial.print(m1);
      Serial.print("A: ");
      Serial.print(a, 1);
      Serial.print('\n');
      Serial.print("B: ");
      Serial.print(b, 1);
      Serial.print('\n');
      Serial.print("Total: ");
      Serial.print(r, 1);
      Serial.print('\n');
      //displays Mode, A, B, and Answer on LCD
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Mode:");
      lcd.print(md);
      lcd.print("A:");
      lcd.print(a);
      lcd.print("B:");
      lcd.print(b);
      lcd.setCursor(0,1);
      lcd.print("Total: ");
      lcd.print(r);
}

//Function to determine which calulation
float Mode(int a, int b, int md) {    //(A, B, and Mode)
  //number of mode
  switch (md) {
    case 1:  //mode = 1
      return a + b;  //Addition
      break;
    case 2:  //mode = 2
      return (a >= b) ? (a - b) : 0;  //Subtraction and displays 0 if negative number
      break;
    case 3:  //mode = 3
      return a * b; //multiplication (9*9 is the highest value = 81)
      break;
    case 4:  //mode = 4
      return (b != 0) ? ((float)a / (float)b) : 0;  //Division and displays 0 if B = 0
      break;
  }
}
//Function for Requirement 3
void Req3() {
  //sets LCD to display Req 1 before starting loop (notifies which requirement is currently in use)
  lcd.setCursor(0,0);
  lcd.print("Req: 3");
  delay(5000);
  //resets LCD display
  lcd.clear();
  Opress = true;  //req button is pressed

  int prevA = 100;  //set integer to 100
  int prevB = 100;  //set integer to 100

  //will run until Req button is pressed
  while (digitalRead(bReq) == LOW) {
    //Initializing which pins for each potentiometer
    int potValueA = analogRead(potA);
    int potValueB = analogRead(potB);
    //to read the value from potentiometer, limiting the value to go from 0-9
    int valueA = map(potValueA, 0, 1023, 0, 9);
    int valueB = map(potValueB, 0, 1023, 0, 9);
    //check if there is a decimal point
    bool dpcheck = false;

    //To determine if the potentiometers where moved
    //when pot value doesnt equal set value
    if (valueA != prevA || valueB != prevB) {
      //setting the values to be equal
      prevA = valueA;
      prevB = valueB;
      change = true;  //declares that there was a change
    }
    //Determine if Mode was pressed
    if (digitalRead(bMode) == HIGH) {
      mpress = true;
      delay(500);
    }
    //Determine if Reset was pressed
    if (digitalRead(bSel) == HIGH) {
      spress = true;
      delay(500);
    }
    //runs when a button is pressed (using or statement)
    if (Opress == true || mpress == true || spress == true) {
      //when Reset is pressed
      if (spress == true) {
        //mode is not 1, resets to 1
        if (mode != 1) {
          mode = 1;
      	}
        spress = false;  //resets button
        change = true;   //states something changed
      }
      //when Mode is pressed
      if (mpress == true) {
        //mode < 4, add 1 to mode
        if (mode < 4) {
          mode++;
        //mode = 4, resets mode to 1
        } else{
          mode = 1;
        }
        mpress = false;  //resets button
        change = true;   //states something changed
      }
    }
    Opress = false;  //resets button
    //when something changed this loop will run
    if (change == true) {
      //calculate answer from Mode function (A,B, mode)
      float result = Mode(valueA, valueB, mode);
      //Checking if the result has a decimal point (real number)
      float num = (result * 10);    //multiplies result by 10 to create whole number
      int dec = (int)num % 10;      //finding the modulus of that number
      //if there is a modulus
      if (dec > 0 ) {   //check if there is a decimal
        dpcheck = true;  
      } 
      //when calculated result is between 0 and 9
      if (result >= 0 && result <= 9) {
        //when its a real number
        if (dpcheck == true) {
          tens = (int)result;  //tens is equal to the first digit
          ones = dec;          //is equal to the second digit
        } else {       //when its a whole number
          tens = 0;                //tens is automatically 0
          ones = (int)result;      //ones is just the answer
        }
      } else if (result >= 10) {   //when the answer is 10 or larger
        tens = (int)result / 10;   //tens is the first digit
        ones = (int)result % 10;   //ones is the second digit
      } else {       //anything else is automatically zero
        tens = 0;
        ones = 0;
      }
      //uses Function to print to serial monitor and LCd
      displayValue2(valueA, valueB, mode, result);
      //Display on 7 segment display
      displayDigit(tens, tensPins);
      displayDigit(ones, onesPins);
      //check if decimal point needs to be turned on
      if (dpcheck == true) {
        digitalWrite(dP1, HIGH);
        dpcheck = false;
      } else {
        digitalWrite(dP1, LOW);
      }
      change = false;  //resets value
    }
  } // end while and resets mode to 1
  mode = 1;
  digitalWrite(dP1, LOW);
}

//Function for Requirement 4
void Req4() {
  //sets LCD to display Req 4 before starting loop (notifies which requirement is currently in use)
  lcd.setCursor(0,0);
  lcd.print("Req: 4");
  delay(dis);
  //resets LCD display
  lcd.clear();
  //Prompts what is needed to start the loop
  lcd.setCursor(0,0);
  lcd.print("Press A or SR");
  //resets the 7 seg display to 0
  displayDigit(0, tensPins);
  displayDigit(0, onesPins);

  //runs until Req button is pressed
  while (digitalRead(bReq) == LOW) {
    //determine if A was pressed
    if (digitalRead(bA) == HIGH) {
      apress = true;
      delay(500);
    }
    //determine if Select Counter was pressed
    if (digitalRead(bSel) == HIGH) {
      spress = true;
      delay(500);
    }
    //when one of the buttons are pressed
    if (apress == true || spress == true) {
      //sets valA to the value in the steps array, based on the counter value
      valA = steps[counter];
      //when A is pressed
      if (apress == true) {
        //setting A to A + valA
        //makes sure that if the calculated value is greater than 99 it is set to 0
        A = A + valA;
        //A < 99, add valA to A (valA is the preset step)
        if (A > 99) {
          A = 0;
        }
      }
      //when SR(reset button) is pressed
      if (spress == true) {
        //Counter < 4, add 1 to counter 
        //Determine which integer in steps array is used
        if (counter < 4) {
          counter++;
          //Counter = 4, resets to 0
        } else {
          counter = 0;
        }
      }
      //when either button is pressed
      if (apress == true || spress == true) {
        //Prints to Serial monitor
        Serial.print("Counter: ");
        Serial.print(valA);
        Serial.print('\n');
        Serial.print("Total: ");
        Serial.print(A);
        Serial.print('\n');
        //Resets LCD display
        lcd.clear();
        //Displays on LCD, the Counter Step and the Total
        lcd.setCursor(0,0);
        lcd.print("Counter: ");
        lcd.print(valA);
        lcd.setCursor(0, 1);
        lcd.print("Total: ");
        lcd.print(A);
        //Displays the total on 7 seg dislpay
        tens = A / 10;  //setting the first digit of answer for tens display
        ones = A % 10;  //setting the second digit of answer for ones display
        //Sending to 7 segment display
        displayDigit(tens, tensPins);
        displayDigit(ones, onesPins);
        apress = false;  //resets button
        spress = false;  //resets button
      }
    }
  }  //end while and resets used values
  A = 0;
  counter = 0;
}