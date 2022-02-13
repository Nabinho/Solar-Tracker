/*******************************************************************************
  Solar Tracker Project Master Code
  (v1.5 - 12/02/2022)

  Created by: @Nabinho

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version (<https://www.gnu.org/licenses/>).
*******************************************************************************/

//------------------------------------------------------------------------------
//Code Setup

// Board Selection Verification
#if !defined(__MK20DX256__) // Teensy 3.2
#error Use this code with Teensy 3.2
#endif

// DEBUG Definition
// Comment for cancel serial monitor DEBUG
#define DEBUG

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
const int MMA_DIP = 5;
const int INA_DIP = 6;
int DIP[4] = {3, 4, 5, 6};
int DIPArraySize = sizeof(DIP) / sizeof(DIP[0]);

//DIP Booleans
bool OpenLog;
bool IHM;
bool SUN_read;
bool INA_read;
bool * bool_values[4] = {&OpenLog, &IHM, &SUN_read, &INA_read};
bool ihm_read = false;
String AuxBuffer;

//Light Sensors Global Variables
//Update with new pins and new resistors values
int TEMT_sensors[4] = {A1, A0, A2, A3};
float TEMT_resistors[4] = {9940.00, 10000.00, 9920.00, 10000.00};
float lux_TEMT[4];
int TEMTArraysSize = sizeof(TEMT_sensors) / sizeof(TEMT_sensors[0]);
float BOTTOMAVERAGE;
float RIGHTAVERAGE;
float LEFTAVERAGE;
float TOPAVERAGE;
const float ACCEPTABLE_DIFFERENCE = 50.0;

//UV Light Sensor Variables
const int UV_sensor = A14;
int UV_index;

//LM35 Variables
const int LM35 = A17;
float temperature;

//Solar panel voltage reading variables
const int Solar_pin = A7;
//Update with real values
const float R1 = 15000.00;
const float R2 = 12000.00;
float solar_panel_voltage;
float efficiency;

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

//------------------------------- DIP Selector Handler -------------------------
void check_dip_selections () {

  for (int i = 0; i < DIPArraySize; i++) {
    if (digitalRead(DIP[i]) == LOW) {
      delay(50);
      if (digitalRead(DIP[i]) == LOW) {
        *bool_values[i] = true;
      }
    } else {
      delay(50);
      if (digitalRead(DIP[i]) == HIGH) {
        *bool_values[i] = false;
      }
    }
  }
  String StrBuffer;
  StrBuffer += "OpenLog = ";
  StrBuffer += String(OpenLog);
  StrBuffer += "\nIHM = ";
  StrBuffer += String(IHM);
  StrBuffer += "\nSolar Pannel = ";
  StrBuffer += String(SUN_read);
  StrBuffer += "\nINA = ";
  StrBuffer += String(INA_read);
#ifdef DEBUG
  Serial.println(StrBuffer);
#endif
  if (OpenLog) {
    Serial1.println(StrBuffer);
  }

}

//------------------------------- Sensors Handler ------------------------------
void read_sensors() {

  //Reads LM35
  analogRead(LM35);
  analogRead(LM35);
  temperature = (3.3 * analogRead(LM35) * 100.00) / 1024.00;
  String StrBuffer;
  StrBuffer += "Temperature = ";
  StrBuffer += String(temperature);
  StrBuffer += " C";
#ifdef DEBUG
  Serial.println(StrBuffer);
#endif
  if (OpenLog) {
    Serial1.println(StrBuffer);
  }
  StrBuffer = "";

  //Reads TEMT sensors and converts to LUX
  for (int i = 0; i < TEMTArraysSize; i++) {
    analogRead(TEMT_sensors[i]);
    analogRead(TEMT_sensors[i]);
    float volts = analogRead(TEMT_sensors[i]) * 3.3 / 1024.00;
    float amps = volts / TEMT_resistors[i];
    float microamps = amps * 1000000.00;
    lux_TEMT[i] = microamps * 2.00;
    String StrBuffer;
    StrBuffer += "TEMT6000";
    StrBuffer += String(i+1);
    StrBuffer += " = ";
    StrBuffer += String(lux_TEMT[i]);
    StrBuffer += " LUX";
#ifdef DEBUG
    Serial.println(StrBuffer);
#endif
    if (OpenLog) {
      Serial1.println(StrBuffer);
    }
  }

  //Reads UV Sensor
  analogRead(UV_sensor);
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
  }
  StrBuffer += "UV Index = ";
  StrBuffer += String(UV_index);
#ifdef DEBUG
  Serial.println(StrBuffer);
#endif
  if (OpenLog) {
    Serial1.println(StrBuffer);
  }

  //Reads Solar Pannel Input
  if (SUN_read) {
    analogRead(Solar_pin);
    analogRead(Solar_pin);
    solar_panel_voltage =  (analogRead(Solar_pin) * (3.3 / 1024.00) * ((R1 + R2) / R2)) + diode_voltage;
    efficiency = (7.4 / solar_panel_voltage) * 100.00;
    String StrBuffer;
    StrBuffer += "Solar Panel Voltage = ";
    StrBuffer += String(solar_panel_voltage);
    StrBuffer += " V\nEfficiency = ";
    StrBuffer += String(efficiency);
    StrBuffer += " %";
#ifdef DEBUG
    Serial.println(StrBuffer);
#endif
    if (OpenLog) {
      Serial1.println(StrBuffer);
    }
  }

  //Reads INA Sensor
  if (INA_read) {
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = (ina219.getBusVoltage_V()) + (diode_voltage * 2.00);
    loadvoltage = busvoltage + (shuntvoltage / 1000.00);
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    String StrBuffer;
    StrBuffer += "Bus Voltage = ";
    StrBuffer += String(busvoltage);
    StrBuffer += " V\nLoad Voltage = ";
    StrBuffer += String(loadvoltage);
    StrBuffer += " V\nShunt Voltage = ";
    StrBuffer += String(shuntvoltage);
    StrBuffer += " mV\nCurrent Consuption = ";
    StrBuffer += String(current_mA);
    StrBuffer += " mA\nPower Consuption = ";
    StrBuffer += String(power_mW);
    StrBuffer += "mW";
#ifdef DEBUG
    Serial.println(StrBuffer);
#endif
    if (OpenLog) {
      Serial1.println(StrBuffer);
    }
  }

}

//------------------------------- IHM Handler ----------------------------------
void check_message () {

  if (IHM && ihm_read) {
    //Serial.println(reading);
    if (reading == "TEMP") {
      String response = String(temperature, 2);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "TEMT1") {
      String response = String(lux_TEMT[0], 2);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "TEMT2") {
      String response = String(lux_TEMT[1], 2);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "TEMT3") {
      String response = String(lux_TEMT[2], 2);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "TEMT4") {
      String response = String(lux_TEMT[3], 2);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "UV") {
      String response = String(UV_index, DEC);
      Serial2.print(response);
    } else if (reading == "SOLARV") {
      String response = String(solar_panel_voltage, DEC);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "VBAT" && INA_read) {
      String response = String(busvoltage, 2);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "SAMP" && INA_read) {
      String response = String(current_mA, 2);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "VBAT" && !INA_read) {
      Serial2.print("INAOFF");
#ifdef DEBUG
      Serial.print("Response to request = INAOFF");
#endif
    } else if (reading == "SAMP" && !INA_read) {
      Serial2.print("INAOFF");
#ifdef DEBUG
      Serial.print("Response to request = INAOFF");
#endif
    } else if (reading == "LOGN" && OpenLog) {
      String response = String(fileNumber, DEC);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else if (reading == "SUNV" && !OpenLog) {
      Serial2.print("LOGOFF");
#ifdef DEBUG
      Serial.print("Response to request = LOGOFF");
#endif
    } else if (reading == "SUNV" && !SUN_read) {
      Serial2.print("SUNOFF");
#ifdef DEBUG
      Serial.print("Response to request = SUNOFF");
#endif
    } else if (reading == "LOGN" && SUN_read) {
      String response = String(solar_panel_voltage, DEC);
      Serial2.print(response);
#ifdef DEBUG
      Serial.print("Response to request = ");
      Serial.println(response);
#endif
    } else {
      Serial2.print("ERROR");
#ifdef DEBUG
      Serial.print("Response to request = ERROR");
#endif
    }
  } else {
    Serial2.print("IHMOFF");
#ifdef DEBUG
    Serial.print("Response to request = IHMOFF");
#endif
  }

}

//------------------------------- RTC Read -------------------------------------
time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

//------------------------------- LOG RTC Time ---------------------------------
void digitalClockDisplay() {

  //Saves reagins in log file created
  String StrBuffer;
  if (day() < 10) {
    AuxBuffer = "0";
  } else {
    AuxBuffer = "";
  }
  StrBuffer += String(day());
  StrBuffer += "/";
  if (month() < 10) {
    AuxBuffer = "0";
  } else {
    AuxBuffer = "";
  }
  StrBuffer += String(month());
  StrBuffer += "/";
  StrBuffer += String(year());
  StrBuffer += "-";
  StrBuffer += String(hour());
  StrBuffer += ":";
  if (minute() < 10) {
    AuxBuffer = "0";
  } else {
    AuxBuffer = "";
  }
  StrBuffer += String(minute());
  StrBuffer += ":";
  if (second() < 10) {
    AuxBuffer = "0";
  } else {
    AuxBuffer = "";
  }
  StrBuffer += String(second());
  StrBuffer += "|";

#ifdef DEBUG
  Serial.println(StrBuffer);
#endif

  if (OpenLog) {
    Serial1.println(StrBuffer);
  }

}

//----------------------------- RTC PC Sync ------------------------------------
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

//---------------------------- Code Setup --------------------------------------
void setup() {

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Solar Tracker Project Made by Nabinho\nDEBUG enabled");
#endif

  //DIP Pins Configuration
  for (int i = 0; i < DIPArraySize; i++) {
    pinMode(DIP[i], INPUT_PULLUP);
  }
  delay(10);

  //Check DIP Selections
  check_dip_selections();
  delay(10);

  //Sensors Configuration
  pinMode(LM35, INPUT);
  pinMode(UV_sensor, INPUT);
  for (int i = 0; i < TEMTArraysSize; i++) {
    pinMode(TEMT_sensors[i], INPUT);
  }
  if (SUN_read) {
    pinMode(Solar_pin, INPUT);
  }
  if (INA_read) {
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

  if (OpenLog) {

    //Teensy RTC Syncornization  Startup
    setSyncProvider(getTeensy3Time);
    //Alternative Time Set Method
    //setTime(21,20,30,10,1,2022);

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
    if (fileNumber > 100) {
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
#ifdef DEBUG
        Serial.println("Sistema e módulo OpenLog inicializados!");
        Serial.println("Nao foi possível sincronizar RTC com PC...");
#endif
        Serial2.println("Sistema e módulo OpenLog inicializados!");
        Serial2.println("Nao foi possível sincronizar RTC com PC...");
      } else {
#ifdef DEBUG
        Serial.println("Sistema e módulo OpenLog inicializados!");
        Serial.println("RTC sincronizado com o PC!");
#endif
        Serial2.println("Sistema e módulo OpenLog inicializados!");
        Serial2.println("RTC sincronizado com o PC!");
      }
    }
  }

#ifdef DEBUG
  Serial.println("System Started");
#endif

}

//---------------------------- Code Loop ---------------------------------------
void loop() {

  //Every Minute
  if ((millis() - time_before) > SECOND) {
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
    if (OpenLog) {
      //Reads RTC Time
      digitalClockDisplay();
    }
    read_sensors();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    time_before = millis();

    //Read Messages from AStar
    if (Serial2.available() > 0) {
      reading = Serial2.readString();
#ifdef DEBUG
      Serial.print("IHM reading: ");
      Serial.println(reading);
#endif
      if (reading == "IHMREADOFF") {
        ihm_read = false;
      } else {
        ihm_read = true;
        check_message();
      }
#ifdef DEBUG
      Serial.print("IHM_READ: ");
      Serial.println(ihm_read);
#endif
    }

    if (!ihm_read) {

      TOPAVERAGE = (lux_TEMT[1] + lux_TEMT[2]) / 2.00;
      BOTTOMAVERAGE = (lux_TEMT[0] + lux_TEMT[3]) / 2.00;
      LEFTAVERAGE = (lux_TEMT[0] + lux_TEMT[1]) / 2.00;
      RIGHTAVERAGE = (lux_TEMT[3] + lux_TEMT[2]) / 2.00;

      if ((TOPAVERAGE - BOTTOMAVERAGE) > ACCEPTABLE_DIFFERENCE) {
        Serial2.print("STILTM");
#ifdef DEBUG
        Serial.println("STILTM");
#endif
      } else if ((BOTTOMAVERAGE - TOPAVERAGE) > ACCEPTABLE_DIFFERENCE) {
        Serial2.print("STILTP");
#ifdef DEBUG
        Serial.println("STILTP");
#endif
      } else if ((RIGHTAVERAGE - LEFTAVERAGE) > ACCEPTABLE_DIFFERENCE) {
        Serial2.print("SPANP");
#ifdef DEBUG
        Serial.println("SPANP");
#endif
      } else if ((LEFTAVERAGE - RIGHTAVERAGE) > ACCEPTABLE_DIFFERENCE) {
        Serial2.print("SPANM");
#ifdef DEBUG
        Serial.println("SPANM");
#endif
      }
    }
  }

}
//------------------------------------------------------------------------------
