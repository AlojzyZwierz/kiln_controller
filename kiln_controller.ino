#define version 241030

#define VERT_PIN A2
#define HORZ_PIN A1
#define buttonPin 1

#define SSR 2
#define MAXCS 13
#define MAXDI 12
#define MAXDO 11
#define MAXCLK 10

//#define MAXDRDY 0   CS, DI, DO, CLK

#define RED A4
#define GREEN A5
#define BLUE A3

#define BUZZERPIN 3

//#define EERLA 1018  // to 1019
#define EEEMERG 1021
#define POWEREE 1023
#define COSTEE 1022
#define KPEE 1000
#define KDEE 1004
#define KIEE 1008
#define HCEE 1012
#define CTEE 1014

#include <LiquidCrystal.h>
#include <EEPROM.h>
//#include <SPI.h>
//#include "Adafruit_MAX31855.h"
#include <Adafruit_MAX31856.h>
Adafruit_MAX31856 thermocouple(MAXCS, MAXDI, MAXDO, MAXCLK);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

struct CElement {
  long endTemp = 3000;
  long hTime = 0;
};

struct eeElement {
  uint8_t data[4];
};

const byte curveElemsNo = 13;
const byte curvesNo = 16;
struct Curve {
  CElement elems[curveElemsNo];
};
struct point {
  long x;
  long y;
};
struct pointF {
  float x;
  float y;
};
struct line {
  float a;
  float b;
};


//---------------------------------------------PID

float kp = 0.5;
float ki = 0.8;
float kd = 0.5;
float dt;
long lastErr;
long er;
long erSum;
unsigned long nextPIDCheck;

unsigned long PIDCheckInterval = 10000;

float der;
float pro;
double integ;

//-----------------------------------------------menu
bool cChanged = false;
bool mChanged = true;
String toShow = "";

byte mPos = 0;
byte mLevel = 0;

bool ON = false;
//Curve curves[5];
Curve cur;
long maxTemp;
bool menuMoveOk = true;


int error = 0;
//unsigned long lastCorrectTempReadTime;

//int elIndex;
int editElIndex;

uint8_t curveIndex;
uint8_t currElemIndex = 0;

uint8_t power;
uint8_t cost;

int secretMenuTimer;
uint8_t secretMenuCount;

int maxSkipTemp = 0;
unsigned long maxSkipTime;

//--------------------------------------------driver



unsigned long endTime;
long startTemp;
long startTime;



long Temps[3];
uint8_t tempIndex;

line eL;
//byte calib = 0;
//bool toCalib;
//unsigned int calibIndex;

uint8_t hOn = 0;
uint8_t hWasOn = 0;

//bool loop0 = true;

//-----------------------------------------

long hCycle = 7000;  //heating cycle lenght

unsigned long hCycleStartTime;
long hStartTime;
float ratio = 0.16;

unsigned long oneSCheckNext = 0;

long sumOfHeatingTime = 0;
long totalHeatingTime = 0;

long currentTemp;
//---------------------------------input
byte butt = 0;

int upV = 0;
bool upChanged = true;
int ud = 0;
int rl = 0;

bool ignore = false;
bool lastState;
bool joystickButtonPressed;
//bool currentState;
unsigned long lastCheck = 0;
int checkInterval = 6;

bool xZeroed = true;
bool yZeroed = true;

unsigned long xZTime;
unsigned long yZTime;

int joyDelay = 600;


void setup() {
  if (!thermocouple.begin()) {
    //Serial.println("ERROR.");
    while (1) delay(100);
  }
  thermocouple.setThermocoupleType(MAX31856_TCTYPE_S);
  //maxthermo.setConversionMode(MAX31856_CONTINUOUS);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(String(version));
  delay(1000);

  currentTemp = 100 * thermocouple.readThermocoupleTemperature();
  int hc;
  int ct;

  EEPROM.get(HCEE, hc);
  EEPROM.get(CTEE, ct);

  hCycle = (long)hc * 1000;
  PIDCheckInterval = (long)ct * 1000;

  EEPROM.get(KPEE, kp);
  EEPROM.get(KDEE, kd);
  EEPROM.get(KIEE, ki);
  EEPROM.get(POWEREE, power);
  EEPROM.get(COSTEE, cost);


  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  //pinMode(MAXDRDY, INPUT);

  //Serial.println("kp" + String(kp, 5) + " kd" + String(kd, 5) + " ki" + String(ki, 5));




  pinMode(VERT_PIN, INPUT);
  pinMode(HORZ_PIN, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SSR, OUTPUT);
  pinMode(A5, OUTPUT);
  //pinMode(BUZZERPIN, OUTPUT);
  SetLed();

  GetButtons();
  readXY();
  OneSecCheck();
  Menu();
  lcd.setCursor(0, 0);
  lcd.print(String(version));
  lcd.setCursor(0, 1);
  ClearRest();
  delay(1000);

  //fillTempArray();

  //currentTemp = avrT(K2N(thermocouple.readCelsius()));
  //loop0 = false;
  EmergencyStart();

  //Serial.println(String(map(44, 40, 10, 0, 100)));
}

void loop() {

  SetLed();
  if (ON) {
    SumHeatingTime();
    if (cur.elems[currElemIndex].hTime == 60000) {
      ratio = 1;
    } else {
      if (millis() > nextPIDCheck) {
        UsePID();
        nextPIDCheck = millis() + PIDCheckInterval;
      }
    }
  }

  // if (error) {
  //   hOn = false;
  // } else {
  TurnHeatingOn();
  // }
  OneSecCheck();
  GetButtons();
  readXY();
  Menu();
}

void (*resetFunc)(void) = 0;

void OneSecCheck() {

  if (ON == false && millis() > 3500000000) resetFunc();

  if (oneSCheckNext <= millis()) {
    oneSCheckNext = millis() + 1000;
    Kiln();
    if (ON) {
      NextEl();
      float wTemp = (float)lineVal(eL, millis());
      //Serial.println(String((float)(eL.a * millis() + eL.b) / 100, 1) + ", " + String((float)currentTemp / 100, 1));
      bool isEndTemp = cur.elems[currElemIndex].endTemp < wTemp && eL.a > 0 || cur.elems[currElemIndex].endTemp > wTemp && eL.a < 0 || cur.elems[currElemIndex].hTime == 60000;
      PrintInfo(cur.elems[currElemIndex].hTime == 60000 ? "Skip" : String((float)(isEndTemp ? cur.elems[currElemIndex].endTemp : wTemp) / 100.0, 1), String(curveIndex + 1) + "." + String(currElemIndex + 1) + " " + maxTemp / 100);
      //PrintInfo(String((float)(eL.a * millis() + eL.b) / 100, 1), String(ratio, 3));
      if (currentTemp > 128000) {
        HeatingOff();
        toShow = ("OH!");
        mLevel = 11;
        Beep();
      } else if (eL.a > 0 && lineVal(eL, millis()) - currentTemp > 10000 && cur.elems[currElemIndex].hTime != 60000) {
        HeatingOff();
        toShow = "HF!";
        mLevel = 11;
        Beep();
      } else if (SkipMax()) {
        HeatingOff();
        toShow = "LP!";
        mLevel = 11;
        Beep();
      } else if (error > 30) {
        HeatingOff();
        toShow = "TF! " + String(currentTemp);
        mLevel = 11;
        Beep();
      }else if (thermocouple.readCJTemperature() > 70) {
        HeatingOff();
        toShow = "CJ/O!";
        mLevel = 11;
        Beep();
      }
    }
    //if (mLevel == 1 && mPos == 0) {
    mChanged = true;
    //}
  }
}
