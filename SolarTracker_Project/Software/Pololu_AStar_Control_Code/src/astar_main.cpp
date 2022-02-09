// Reference - https://docs.m5stack.com/#/en/unit/cardkb
// Nabinho's AStar Micro (Slave) code for Teensy 3.2 (Master)
// Solar Tracker Project

// Board Selection Verification
#if !defined(ARDUINO_AVR_A_STAR_32U4) // Pololu A Star Micro
#error Use this code with Pololu AStar Micro
#endif

// DEBUG Definition
// Comment for cancel serial monitor DEBUG
#define DEBUG

// Libraries
#include <LiquidCrystal_I2C.h>
#include <ServoEasing.hpp>
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>

// IHM Communication Variables
// LCD variables
#define LCD_ADDR 0x3F
#define N_COLUMNS 16
#define N_ROWS 2
LiquidCrystal_I2C lcd(LCD_ADDR, N_COLUMNS, N_ROWS);
// Keyboard variables
#define CARDKB_ADDR 0x5F
char leitura;
String letter;
String message;
String leituraHEX;
int index;
bool log_read = false;

// Times Variables
unsigned long time_before = 0;
const int TIME = 500;

// Servos Objects
ServoEasing servo_pan;
ServoEasing servo_tilt;
const int STDSpeed = 25;

// Servo Control Variables
int memory_pan = 0;
int memory_tilt = 1;
int pos_pan;
int pos_tilt;

// Pan servo variables
const int pin_pan = 4;
const int pan_max = 180;
const int pan_min = 0;

// Tilt servo variables
const int pin_tilt = 5;
const int tilt_max = 180;
const int tilt_min = 90;

// Joystick middle button variables
const int pin_middle = 7;
int readingM;
int buttonMState;
int lastButtonMState = HIGH;
unsigned long lastMDebounceTime = 0;
const int MdebounceDelay = 500;
bool manual_control = false;

// Joystick left-right variables
const int pin_left = A0;
const int pin_right = 8;
int readingL;
int readingR;
int buttonLState;
int buttonRState;
int lastButtonLState = HIGH;
int lastButtonRState = HIGH;
unsigned long lastLRDebounceTime = 0;
const int LRdebounceDelay = 100;

// Joystick up-down variables
const int pin_up = 6;
const int pin_down = 9;
int readingU;
int readingD;
int buttonUState;
int buttonDState;
int lastButtonUState = HIGH;
int lastButtonDState = HIGH;
unsigned long lastUDDebounceTime = 0;
const int UDdebounceDelay = 100;

//-------------------- IHM Handler ----------------------
void IHM_control()
{

  if (!manual_control)
  {
    Wire.requestFrom(CARDKB_ADDR, 1);
    while (Wire.available())
    {
      leitura = Wire.read();
      if (leitura != 0)
      {
        leituraHEX = String(leitura, HEX);
#ifdef DEBUG
        Serial.print("Leitura cardKB = ");
        Serial.println(leitura);
        Serial.print("Leitura cardKB HEX = ");
        Serial.println(leituraHEX);
#endif
      }
    }
    // Read M5Stack CardKB module

    if (leitura != 0)
    {
      if (leituraHEX == "8")
      {
        index = message.length();
        message.remove(index - 1);
        index = message.length();
        lcd.clear();
        lcd.print("Message:  " + message);
#ifdef DEBUG
        Serial.print("message - ");
        Serial.println(message);
#endif
      }
      else if (leituraHEX == "ff8b")
      {
        message = "";
#ifdef DEBUG
        Serial.print("message erased.");
        Serial.println(message);
#endif
      }
      else if (leituraHEX == "d" || leituraHEX == "a3")
      {
#ifdef DEBUG
        Serial.println("Enter");
#endif
        log_read = true;
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else if (leituraHEX == "1b" || leituraHEX == "ff80")
      {
#ifdef DEBUG
        Serial.println("Esc");
#endif
        digitalWrite(LED_BUILTIN, LOW);
        Serial1.print("IHMREADOFF");
        log_read = false;
        message = "";
      }
      else if (index > 6)
      {
#ifdef DEBUG
        Serial.print("message too long - ");
        Serial.println(message);
#endif
        message.remove(index - 1);
        lcd.clear();
        lcd.print("Message:  " + message);
        lcd.setCursor(0, 1);
        lcd.print("Message too long");
      }
      else
      {
        letter = String(leitura);
        message += letter;
        index = message.length();
        lcd.clear();
        lcd.print("Message:  " + message);
#ifdef DEBUG
        Serial.print("char - ");
        Serial.print(leitura);
        Serial.print("message - ");
        Serial.println(message);
#endif
      }
    }
  }
}

//-------------------- Servo Handler ----------------------
void servo_control()
{

  // Reads button states
  readingM = digitalRead(pin_middle);

  // Debounces time update
  if (readingM != lastButtonMState)
  {
    // reset the debouncing timer
    lastMDebounceTime = millis();
  }

  // Debounce verification
  if ((millis() - lastMDebounceTime) > MdebounceDelay)
  {
    if (readingM != buttonMState)
    {
      buttonMState = readingM;
      if (buttonMState == LOW)
      {
        // Inverts manual control funtion state
        manual_control = !manual_control;
        if (manual_control)
        {
// Attaches Servos
#ifdef DEBUG
          Serial.println("ENTERING SERVOS MANUAL MODE");
#endif
          lcd.clear();
          lcd.print(" ENTERED SERVOS ");
          lcd.setCursor(0, 1);
          lcd.print(" MANUAL CONTROL ");
        }
        else
        {
          // Detaches Servos
          // Serial.println("EXITING MANUAL MODE");
          lcd.clear();
          lcd.print(" EXITED SERVOS ");
          lcd.setCursor(0, 1);
          lcd.print(" MANUAL CONTROL ");
        }
      }
    }
  }

  // Updates button reading
  lastButtonMState = readingM;

  // Checks manual control function selection
  if (manual_control)
  {

    // updates left-right buttons reading
    readingL = digitalRead(pin_left);
    readingR = digitalRead(pin_right);

    // Debounce time update
    if (readingL != lastButtonLState || readingR != lastButtonRState)
    {
      // reset the debouncing timer
      lastLRDebounceTime = millis();
    }

    // Debounce verification
    if ((millis() - lastLRDebounceTime) > LRdebounceDelay)
    {
      if (readingL != buttonLState || readingR != buttonRState)
      {
        servo_pan.attach(pin_pan);
        buttonLState = readingL;
        buttonRState = readingR;
        if (buttonLState == LOW)
        {
          pos_pan++;
          if (pos_pan >= pan_max)
          {
            pos_pan = pan_max;
          }
        }
        if (buttonRState == LOW)
        {
          pos_pan--;
          if (pos_pan <= pan_min)
          {
            pos_pan = pan_min;
          }
        }
        servo_pan.easeTo(pos_pan, STDSpeed);
        EEPROM.write(memory_pan, pos_pan);
#ifdef DEBUG
        Serial.print("PAN POSITION: ");
        Serial.print(pos_pan);
#endif
        lcd.clear();
        lcd.print("PAN POS:    ");
        lcd.print(pos_pan);
        lcd.setCursor(0, 1);
        lcd.print("TILT POS:   ");
        lcd.print(pos_tilt);
      }
    }

    // Updates left-right buttons reading
    lastButtonRState = readingR;
    lastButtonLState = readingL;
    servo_pan.detach();

    // updates up-down buttons reading
    readingU = digitalRead(pin_up);
    readingD = digitalRead(pin_down);

    // Debounce time update
    if (readingU != lastButtonUState || readingD != lastButtonDState)
    {
      // reset the debouncing timer
      lastUDDebounceTime = millis();
    }

    // Debounce verification
    if ((millis() - lastUDDebounceTime) > UDdebounceDelay)
    {
      if (readingU != buttonUState || readingD != buttonDState)
      {
        servo_tilt.attach(pin_tilt);
        buttonUState = readingU;
        buttonDState = readingD;
        if (buttonUState == LOW)
        {
          pos_tilt++;
          if (pos_tilt >= tilt_max)
          {
            pos_tilt = tilt_max;
          }
        }
        if (buttonDState == LOW)
        {
          pos_tilt--;
          if (pos_tilt <= tilt_min)
          {
            pos_tilt = tilt_min;
          }
        }
        servo_tilt.easeTo(pos_tilt, STDSpeed);
        EEPROM.write(memory_tilt, pos_tilt);
#ifdef DEBUG
        Serial.print("TILT POSITION: ");
        Serial.print(pos_tilt);
#endif
        lcd.clear();
        lcd.print("PAN POS:    ");
        lcd.print(pos_pan);
        lcd.setCursor(0, 1);
        lcd.print("TILT POS:   ");
        lcd.print(pos_tilt);
      }
    }

    // Updates up-down buttons reading
    lastButtonUState = readingU;
    lastButtonDState = readingD;
    servo_tilt.detach();
  }
}

//-------------------- Configuration ----------------------
void setup()
{

  // System startup
  Serial.begin(115200);
  Serial1.begin(115200);

  // LED configuration
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // EEPROM stratup
  pos_pan = EEPROM.read(memory_pan);
  pos_tilt = EEPROM.read(memory_tilt);
  delay(100);

  // Pan servo con[]
  servo_pan.attach(pin_pan, pos_pan);
  delay(50);
  servo_pan.detach();

  // Tilt servo configuration
  servo_tilt.attach(pin_tilt, pos_tilt);
  delay(50);
  servo_tilt.detach();

  // Joystick pins configuration
  pinMode(pin_left, INPUT_PULLUP);
  pinMode(pin_right, INPUT_PULLUP);
  pinMode(pin_up, INPUT_PULLUP);
  pinMode(pin_down, INPUT_PULLUP);
  pinMode(pin_middle, INPUT_PULLUP);

  // IHM configuration
  lcd.init();
  Wire.begin();
  lcd.backlight();
  lcd.print(" SYSTEM    OK! ");
  delay(1000);
  lcd.clear();
}

//-------------------- Repetition -------------------------
void loop()
{

  // Handles Manual Servo Control Configuration
  servo_control();

  // Handles IHM Control Function
  IHM_control();

  // Every second
  if ((millis() - time_before) > TIME)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    if (log_read && !manual_control)
    {
// Sends message to Master
#ifdef DEBUG
      Serial.print("Mensage ");
      Serial.print(message);
      Serial.println(" sent, waiting return...");
#endif
      lcd.setCursor(0, 0);
      lcd.print("Message:  " + message);
      if (message == "SPPAN")
      {
        lcd.setCursor(0, 1);
        lcd.print("Reading:");
        lcd.setCursor(10, 1);
        lcd.print(pos_pan);
#ifdef DEBUG
        Serial.print("Return: ");
        Serial.println(pos_pan);
#endif
      }
      else if (message == "SPTILT")
      {
        lcd.setCursor(0, 1);
        lcd.print("Reading:");
        lcd.setCursor(10, 1);
        lcd.print(pos_tilt);
#ifdef DEBUG
        Serial.print("Return: ");
        Serial.println(pos_tilt);
#endif
      }
      else
      {
        Serial1.print(message);
      }
    }
    time_before = millis();
  }

  // Reads replies from Master
  if (Serial1.available() > 0)
  {
    String leitura = Serial1.readString();
#ifdef DEBUG
    Serial.print("Return: ");
    Serial.println(leitura);
#endif
    // Serial.println(leitura);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // Handles Servos Control Messages
    if (!log_read && !manual_control)
    {
      servo_tilt.attach(pin_tilt);
      servo_pan.attach(pin_pan);
      if (leitura == "STILTP")
      {
        pos_tilt += 5;
        if (pos_tilt > tilt_max)
        {
          pos_tilt = tilt_max;
        }
        servo_tilt.easeTo(pos_tilt, STDSpeed);
        EEPROM.write(memory_tilt, pos_tilt);
      }
      else if (leitura == "STILTM")
      {
        pos_tilt -= 5;
        if (pos_tilt < tilt_min)
        {
          pos_tilt = tilt_min;
        }
        servo_tilt.easeTo(pos_tilt, STDSpeed);
        EEPROM.write(memory_tilt, pos_tilt);
      }
      else if (leitura == "SPANP")
      {
        pos_pan += 5;
        if (pos_pan > pan_max)
        {
          pos_pan = pan_max;
        }
        servo_pan.easeTo(pos_pan, STDSpeed);
        EEPROM.write(memory_pan, pos_pan);
      }
      else if (leitura == "SPANM")
      {
        pos_pan -= 5;
        if (pos_pan < pan_min)
        {
          pos_pan = pan_min;
        }
        servo_pan.easeTo(pos_pan, STDSpeed);
        EEPROM.write(memory_pan, pos_pan);
      }
    }

    // Hendles IHM Messages
    if (log_read && !manual_control)
    {
      servo_tilt.detach();
      servo_pan.detach();
      if (leitura == "ERROR")
      {
        // Serial.println("Mensagem inválida!");
        lcd.setCursor(0, 1);
        lcd.print("Invalid message!");
        log_read = false;
      }
      else if (leitura == "IHMOFF")
      {
        lcd.setCursor(0, 1);
        lcd.print(" IHM  Disabled! ");
        log_read = false;
      }
      else if (leitura == "INAOFF")
      {
        lcd.setCursor(0, 1);
        lcd.print(" INA  Disabled! ");
        log_read = false;
      }
      else if (leitura == "LOGOFF")
      {
        lcd.setCursor(0, 1);
        lcd.print(" LOG  Disabled! ");
        log_read = false;
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(leitura);
      }
    }
  }
}