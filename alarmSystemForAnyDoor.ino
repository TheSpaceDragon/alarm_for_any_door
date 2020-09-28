/* 
 *  Name: Gagan Deep Singh
 *  Date: 9/28/2020
 *  FileName: alarmSystemForAnyDoor.ino
 *  Purpose: An alarm door that can be put on top of any door handle. 
 *  Version: 1
*/
#include <Keypad.h>

const byte ROWS = 4; //Initializing pins
const byte COLS = 4;
int counter = 0;
int passLength = 6;
const int MOTION_SENSOR = 51;
const int LED_PIN = 13;
const int RED_LED = 22;
const int GREEN_LED = 24;
const int BUZZER_PIN = 49;

int motion_sensor_value = 0; //Decalring variables
int pir_sensor = 0;

String password = "1604AD";
String passStorage = "";

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //Creates the Keypad object required to read data from the keypad

void setup() {
  pinMode(MOTION_SENSOR, INPUT); //pinMode declarations. Declaring I/O
  pinMode(LED_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(RED_LED, HIGH); //Red LED indicates that the door is locked
  char key = keypad.getKey();

  if (key) {
    passStorage += key; // If a key is pressed, add the key char to the string passStorage. This runs six times (Length of the password)
    counter += 1;
  }

  if (counter == passLength) { // Checking mechanism for the password entered vs the true password.
    if (passStorage == password) {
      Serial.println("Access Granted");
      tone(BUZZER_PIN, 500); // High pitched beeping noise indicating correct password has been entered. 
      delay(100);
      noTone(BUZZER_PIN);
      delay(100);
      tone(BUZZER_PIN, 500);
      delay(100);
      noTone(BUZZER_PIN);
      Serial.println("Buzzer Ended");
      passStorage = ""; //Clear the passStorage and counter to make way for a new password to be entered.
      counter = 0;
      digitalWrite(RED_LED, LOW); // Green LED indicates that the door is open.
      digitalWrite(GREEN_LED, HIGH);
      delay(10000);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
    }

    else {
      Serial.println("Access Denied!"); //If password does not match, clear passStorage and counter to make way for a new password to be entered. 
      passStorage = "";
      counter = 0;
    }
  }
  motion_sensor_value = digitalRead(MOTION_SENSOR); //Getting value of motion sensor from the pin.

  if (motion_sensor_value == HIGH) {
    digitalWrite(LED_PIN, HIGH);

    if (pir_sensor == LOW) { // If an object that radiates IR (like humans, animals, etc.) are sensed, start the buzzer.
      Serial.println("Buzzer");
      tone(BUZZER_PIN, 150);
      pir_sensor = HIGH;
    }
  }

  else {
    digitalWrite(LED_PIN, LOW); // If not, no need to start the buzzer.

    if (pir_sensor == HIGH) {
      pir_sensor = LOW;
    }
  }
}
