//Nabinho's Teensy 3.2 (Master) code for Solar Tracker Project

//Libraries
#include <Wire.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <Arduino.h>
#include <Adafruit_INA219.h>

//OpenLog File Variables
char buff[50];
const int log_memory = 0;
int fileNumber;

//Reset Pins
const int resetOpenLog = 2;
const int resetAStar = 11;

//Times Variables
unsigned long time_before = 0;
const int SECOND = 1000;
unsigned long before_read = 0;

//Message Received Variable
String reading;

//DIP Pins
const int LOG_DIP = 3;
const int IHM_DIP = 4;
const int INA_DIP = 5;
const int SUN_DIP = 6;
int DIP[4] = {LOG_DIP, IHM_DIP, INA_DIP, SUN_DIP};
int DIPArraySize = sizeof(DIP)/sizeof(DIP[0]);

//DIP Booleans
bool OpenLog;
bool IHM;
bool SUN_read;
bool INA_read;
bool * bool_values[4] = {&OpenLog, &IHM, &SUN_read, &INA_read};
bool ihm_read = false;

//Light Sensors Global Variables
//Update with new pins and new resistors values
int TEMT_sensors[4] = {A0, A1, A2, A3};
float TEMT_resistors[4] = {9940.00, 9990.00, 1000.00, 9920.00};
float lux_TEMT[4];
int TEMTArraysSize = sizeof(TEMT_sensors)/sizeof(TEMT_sensors[0]);
float BOTTOMAVERAGE;
float RIGHTAVERAGE;
float LEFTAVERAGE;
float TOPAVERAGE;
const float ACCEPTABLE_DIFFERENCE = 50.0;

//UV Light Sensor Variables
const int UV_sensor = A6;
int UV_index;

//LM35 Variables
const int LM35 = A14;
float temperature;

//Solar panel voltage reading variables
const int Solar_pin = A7;
//Update with real values
const float R1 = 14860.00;
const float R2 = 11880.00;
float solar_panel_voltage;
float effiency;

//INA219 Object
Adafruit_INA219 ina219;

//INA219 Variables
float shuntvoltage;
float busvoltage ;
float loadvoltage;
float current_mA;
float power_mW;

//Diode constant dropout voltage
//Update with real values if necessary
const float diode_voltage = 0.50;

//-------------------- DIP Selector Handler ----------------------
void check_dip_selections (){

  for (int i = 0; i < DIPArraySize; i++){
    if(digitalRead(DIP[i]) == LOW){
      delay(50);
      if(digitalRead(DIP[i]) == LOW){
        *bool_values[i] = true;
      }
    } else {
      delay(50);
      if(digitalRead(DIP[i]) == HIGH){
        *bool_values[i] = false;
      }
    }
  }

}

//-------------------- Sensors Handler ----------------------
void read_sensors(){

  //Reads LM35
  analogRead(LM35);
  temperature = (3.3 * analogRead(LM35) * 100.00) / 1024.00;
  if(OpenLog){
    Serial1.print("TEMPERATURE = "); Serial1.print(temperature); Serial1.println(" C");
  }

  //Reads TEMT sensors and converts to LUX
  for (int i = 0; i < TEMTArraysSize; i++){
    analogRead(TEMT_sensors[i]);
    float volts = analogRead(TEMT_sensors[i]) * 3.3 / 1024.00;
    float amps = volts / TEMT_resistors[i];
    float microamps = amps * 1000000.00;
    lux_TEMT[i] = microamps * 2.00;
    if(OpenLog){
      Serial1.print("TEMT");
      Serial1.print(i);
      Serial1.print(" = ");
      Serial1.print(lux_TEMT[i]);
      Serial1.println(" LUX");
    }
  }

  //Reads UV Sensor
  analogRead(UV_sensor);
  int UV_reading = analogRead(UV_sensor);
  if (UV_reading <= 10) {
    UV_index = 0;
  } else if (UV_reading > 10 && UV_reading <= 46) {
    UV_index = 1;
  } else if (UV_reading > 46 && UV_reading <= 65) {
    UV_index = 2;
  } else if (UV_reading > 65 && UV_reading <= 83) {
    UV_index = 3;
  } else if (UV_reading > 83 && UV_reading <= 103) {
    UV_index = 4;
  } else if (UV_reading > 103 && UV_reading <= 124) {
    UV_index = 5;
  } else if (UV_reading > 124 && UV_reading <= 142) {
    UV_index = 6;
  } else if (UV_reading > 142 && UV_reading <= 162) {
    UV_index = 7;
  } else if (UV_reading > 162 && UV_reading <= 180) {
    UV_index = 8;
  } else if (UV_reading > 180 && UV_reading <= 200) {
    UV_index = 9;
  } else if (UV_reading > 200 && UV_reading <= 221) {
    UV_index = 10;
  } else {
    UV_index = 11;
  } if(OpenLog){
    Serial1.print("UV INDEX = "); Serial1.println(UV_index);
  }

  if(SUN_read){
    analogRead(Solar_pin);
    solar_panel_voltage =  (analogRead(Solar_pin) * (3.3/1024.00) * ((R1 + R2)/R2)) + diode_voltage;
    effiency = (solar_panel_voltage / 7.4) * 100.00;
    if(OpenLog){
      Serial1.print("Solar Panel Voltage: ");
      Serial1.print(solar_panel_voltage);
      Serial1.println(" V");
      Serial1.print("Effiency: ");
      Serial1.print(effiency);
      Serial1.println(" %");
    }
  }

  //Reads INA Sensor
  if(INA_read){
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = (ina219.getBusVoltage_V()) + (diode_voltage * 2.00);
    loadvoltage = busvoltage + (shuntvoltage / 1000.00);
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    if(OpenLog){
      Serial1.print("Bus Voltage: "); Serial1.print(busvoltage); Serial1.println(" V");
      Serial1.print("Shunt Voltage: "); Serial1.print(shuntvoltage); Serial1.println(" mV");
      Serial1.print("Load Voltage: "); Serial1.print(loadvoltage); Serial1.println(" V");
      Serial1.print("Current: "); Serial1.print(current_mA); Serial1.println(" mA");
      Serial1.print("Power: "); Serial1.print(power_mW); Serial1.println(" mW");
    }
  }

}

//-------------------- IHM Handler ----------------------
void check_message () {

  if (IHM && ihm_read){
    //Serial.println(reading);
    if (reading == "TEMP"){
      String response = String(temperature,2);
      Serial2.print(response);
      //Serial.println(response);
    } else if (reading == "TEMT1"){
      String response = String(lux_TEMT[0],2);
      Serial2.print(response);
    } else if (reading == "TEMT2"){
      String response = String(lux_TEMT[1],2);
      Serial2.print(response);
    } else if (reading == "TEMT3"){
      String response = String(lux_TEMT[2],2);
      Serial2.print(response);
    } else if (reading == "TEMT4"){
      String response = String(lux_TEMT[3],2);
      Serial2.print(response);
    } else if (reading == "UV"){
      String response = String(UV_index, DEC);
      Serial2.print(response);
    } else if (reading == "SOLARV"){
      String response = String(solar_panel_voltage, DEC);
      Serial2.print(response);
    } else if (reading == "VBAT" && INA_read){
      String response = String(busvoltage, 2);
      Serial2.print(response);
    } else if (reading == "SAMP" && INA_read){
      String response = String(current_mA, 2);
      Serial2.print(response);
    } else if (reading == "VBAT" && !INA_read){
      Serial2.print("INAOFF");
    } else if (reading == "SAMP" && !INA_read){
      Serial2.print("INAOFF");
    } else if (reading == "LOGN" && OpenLog){
      String response = String(fileNumber, DEC);
      Serial2.print(response);
    } else if (reading == "LOGN" && !OpenLog){
      Serial2.print("LOGOFF");
    } else {
      Serial2.print("ERROR");
    }
  } else {
    Serial2.print("IHMOFF");
  }

}

//-------------------- RTC Read ----------------------
time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

//-------------------- Log Time Digits Handler ----------------------
void printLogDigits(int digits) {
  Serial1.print(":");
  if (digits < 10) {
    Serial1.print('0');
  }
  Serial1.print(digits);
}

//-------------------- Log Time Digits Handler ----------------------
void printLogDigits2(int digits) {
  if (digits < 10) {
    Serial1.print('0');
  }
  Serial1.print(digits);
  Serial1.print("/");
}

//-------------------- LOG RTC Time ----------------------
void digitalClockDisplay() {

  //Saves reagins in log file created
  Serial1.print(hour());
  printLogDigits(minute());
  printLogDigits(second());
  Serial1.print("-");
  printLogDigits2(day());
  printLogDigits2(month());
  Serial1.print(year());
  Serial1.print("|");
  //Serial1.println();

}

//-------------------- RTC PC Sync ----------------------
#define TIME_HEADER  "T"
unsigned long processSyncMessage() {

  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600;

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    return pctime;
    if ( pctime < DEFAULT_TIME) {
      pctime = 0L;
    }
  }

  return pctime;

}

//-------------------- Configuration ----------------------
void setup() {

  //DIP Pins Configuration
  for (int i = 0; i < DIPArraySize; i++){
    pinMode(DIP[i], INPUT_PULLUP);
  }
  delay(10);

  //Check DIP Selections
  for(int i = 0; i < 5; i++){
    check_dip_selections();
  }
  delay(10);

  //Sensors Configuration
  pinMode(LM35, INPUT);
  pinMode(UV_sensor, INPUT);
  for (int i = 0; i < TEMTArraysSize; i++){
    pinMode(TEMT_sensors[i], INPUT);
  }
  if(SUN_read){
    pinMode(Solar_pin, INPUT);
  }
  if(INA_read){
    ina219.begin();
  }
  delay(10);

  //Pins Configuration
  pinMode(resetOpenLog, OUTPUT);
  pinMode(resetAStar, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(10);

  //AStar Micro Reset
  digitalWrite(resetAStar, LOW);
  delay(100);
  digitalWrite(resetAStar, HIGH);
  delay(100);
  Serial2.begin(9600);

  if(OpenLog){

    //Teensy RTC Syncornization  Startup
    setSyncProvider(getTeensy3Time);
    //Alternative Time Set Method
    //setTime(21,20,30,10,1,2022);
    Serial.begin(9600);

    //OpenLog Reset
    digitalWrite(resetOpenLog, LOW);
    delay(100);
    digitalWrite(resetOpenLog, HIGH);
    delay(100);
    Serial1.begin(9600);
    delay(100);

    //OpenLog Command Mode Stratup
    Serial1.write(26);
    Serial1.write(26);
    Serial1.write(26);
    delay(100);

    //New Log File Creation
    fileNumber = EEPROM.read(log_memory);
    delay(50);
    fileNumber++;
    if(fileNumber > 100){
      fileNumber = 0;
    }
    delay(50);
    EEPROM.write(log_memory, fileNumber);
    delay(50);
    sprintf(buff, "new data%03d.txt", fileNumber);
    Serial1.println(buff);
    delay(100);

    //Entering New File
    sprintf(buff, "append data%03d.txt\r", fileNumber);
    Serial1.print(buff);
    delay(100);

    //RTC Syncronization Verification
    if (Serial) {
      delay(100);
      if (timeStatus() != timeSet) {
        Serial.println("Sistema e módulo OpenLog inicializados!");
        Serial.println("Nao foi possível sincronizar RTC com PC...");
        Serial2.println("Sistema e módulo OpenLog inicializados!");
        Serial2.println("Nao foi possível sincronizar RTC com PC...");
      } else {
        Serial.println("Sistema e módulo OpenLog inicializados!");
        Serial.println("RTC sincronizado com o PC!");
        Serial2.println("Sistema e módulo OpenLog inicializados!");
        Serial2.println("RTC sincronizado com o PC!");
      }
    }
  }

}

//-------------------- Repetition -------------------------
void loop() {

  //Every Minute
  if ((millis() - time_before) > SECOND){
    //Checks PC Connection
    if (Serial.available()) {
      //Syncs RTC Tith PC's Time
      time_t t = processSyncMessage();
      if (t != 0) {
        Teensy3Clock.set(t);
        setTime(t);
      }
    }
    //Checks Logging Selection
    if(OpenLog){
      //Reads RTC Time
      digitalClockDisplay();
    }
    read_sensors();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    time_before = millis();

    //Read Messages from AStar
    if (Serial2.available() > 0) {
      reading = Serial2.readString();
      if(reading == "IHMREADOFF"){
        ihm_read = false;
      } else {
        ihm_read = true;
        check_message();
      }
      Serial.print("IHM_READ: ");
      Serial.println(ihm_read);
    }

    if(!ihm_read){

      TOPAVERAGE = (lux_TEMT[1] + lux_TEMT[2]) / 2.00;
      BOTTOMAVERAGE = (lux_TEMT[0] + lux_TEMT[3]) / 2.00;
      LEFTAVERAGE = (lux_TEMT[0] + lux_TEMT[1]) / 2.00;
      RIGHTAVERAGE = (lux_TEMT[3] + lux_TEMT[2]) / 2.00;

      if((TOPAVERAGE - BOTTOMAVERAGE) > ACCEPTABLE_DIFFERENCE){
        Serial2.print("STILTM");
        //Serial.println("STILTM");
      } else if((BOTTOMAVERAGE - TOPAVERAGE) > ACCEPTABLE_DIFFERENCE){
        Serial2.print("STILTP");
        //Serial.println("STILTP");
      } else if((RIGHTAVERAGE - LEFTAVERAGE) > ACCEPTABLE_DIFFERENCE){
        Serial2.print("SPANP");
        //Serial.println("SPANP");
      } else if((LEFTAVERAGE - RIGHTAVERAGE) > ACCEPTABLE_DIFFERENCE){
        Serial2.print("SPANM");
        //Serial.println("SPANM");
      }
    }
  }

}
