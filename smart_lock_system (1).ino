#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int RELAY_PIN = 7;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 12, 11, 10, 9 };
byte colPins[COLS] = { 8, 6, 5, 4 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char password[] = "4567";
int currentposition = 0;
String inputString;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  inputString.reserve(4);
  lcd.init();

  lcd.backlight();
  lcd.clear();
  displayScreen();
}

void loop() {

  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key >= '0' && key <= '9') {  // only act on numeric keys
      inputString += key;            // append new character to input string

    } else if (key == '*') {
      inputString = "";  // clear input
    }
  }


  if (key != NO_KEY) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PASSWORD:");
    lcd.print(" ");
    for (int i = 0; i <= currentposition; i++) lcd.print("*");
    if (key == password[currentposition]) {
      // lcd.print(password);
      currentposition++;
      if (currentposition == sizeof(password) - 1) {
        unlockDoor();
        currentposition = 0;
      }
    } else {
      incorrect();
      currentposition = 0;
      // lcd.print("hello");
    }
  }

  if (Serial.available() > 0) {
    int value1 = Serial.read();
    if (value1 == 1) {
      unlockDoor();
    }
  }
}

void unlockDoor() {
  delay(900);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Granted");
  lcd.setCursor(0, 1);
  lcd.print("WELCOME!!");
  digitalWrite(RELAY_PIN, LOW);

  delay(2000);
  counterBeep();
  delay(2000);
  digitalWrite(RELAY_PIN, HIGH);
  displayScreen();
}

void incorrect() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CODE INCORRECT");
  lcd.setCursor(0, 1);
  lcd.print("UNAUTHORIZED");
  delay(3000);
  displayScreen();
}

void displayScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*ENTER THE CODE*");
  lcd.setCursor(0, 1);
  lcd.print("TO _/_ (OPEN)!!");
}

void counterBeep() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GET IN WITHIN:");
  for (int i = 5; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print(i);
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RE-LOCKING...");
  delay(3000);

  digitalWrite(RELAY_PIN, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LOCKED!");
}
