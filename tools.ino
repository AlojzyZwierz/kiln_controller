
//-----------------------------------------------------------------------tools
String pF2s(pointF p, int prec) {
  return ("{" + String(p.x, prec) + ";" + String(p.y, prec) + "}");
}
String pF2s(pointF p) {
  return (pF2s(p, 4));
}

long clampBeep(long const value, long const lhs, long const rhs) {
  if (value < lhs) {
    //tone(BUZZERPIN, 500, 120);
    menuMoveOk = false;
    return lhs;

  } else if (value > rhs) {
    //tone(BUZZERPIN, 800, 250);
    menuMoveOk = false;
    return rhs;
  } else {
    return value;
  }
}
/*
void ErrorBeep(byte n) {
  for (int i = 0; i < 8; i++) {
    
    tone(BUZZERPIN, i==(n-1)?864: 432, 150);
    delay((i+1)%4==0?300:150);
    if(n%4==0) delay(150);
  }
  delay (300);
}
*/
/*
void Beep(int v) {
tone(BUZZERPIN, 10*v, 16);  
  /*
  for (int i = 0; i < v; i++) {
    analogWrite(BUZZERPIN, (float(i) / float(v)) * 400);
    //delay((float(i) / float(v)) * 2 * (micros() % 2));
    delay(i/25);
    analogWrite(BUZZERPIN, i * 2.5);
    delay(i/25);
  }
  analogWrite(BUZZERPIN, 0);

}
*/
/*
uint8_t constrain(uint8_t value, uint8_t lhs, uint8_t rhs) {
  return ((value < lhs) ? lhs : ((value > rhs) ? rhs : value));
}
int constrain(int value, int lhs, int rhs) {
  return ((value < lhs) ? lhs : ((value > rhs) ? rhs : value));
}
*/
String leadingZero(long i, int noOfDigits) {
  return leadingZero(i, noOfDigits, '0');
}

String leadingZero(long i, long noOfDigits, char ch) {
  String iStr = (String(i));
  int noOfDigInI = iStr.length();
  if (noOfDigInI >= noOfDigits) return iStr;
  String toRet = "";
  for (int j = 0; j < (noOfDigits - noOfDigInI); j++) {
    toRet += ch;
  }
  toRet += iStr;
  return toRet;
}


long HMStoMillis(int h, int m, int s) {
  return (h * 3600000 + m * 60000 + s * 1000);
}


String millisToHMS(long t) {
  String toReturn = "";
  if (t < 0) {
    toReturn = "-";
    t = -t;
  }
  long h = t / 3600000;
  long temp = t % 3600000;
  long m = (temp) / 60000;
  long s = (temp % 60000) / 1000;
  toReturn = toReturn + String(leadingZero(h, 2) + ":" + leadingZero(m, 2) + ":" + leadingZero(s, 2));
  return toReturn;
}
String millisToHM(long t) {
  String toReturn = "";
  if (t < 0) {
    toReturn = "-";
    t = -t;
  }
  long h = t / 3600000;
  long temp = t % 3600000;
  long m = (temp) / 60000;
  toReturn = toReturn + String(leadingZero(h, 2) + ":" + leadingZero(m, 2));
  return toReturn;
}
long GetMaxTemp(Curve c) {
  long maxT = 0;
  for (int i = 0; i < curveElemsNo; i++) {
    if (c.elems[i].hTime == 0) break;
    if (c.elems[i].endTemp > maxT) {
      maxT = c.elems[i].endTemp;
    }
  }
  return maxT;
}
/*
int GetMaxTempEl(Curve c) {
  long maxTe = 0;
  int maxI = 0;
  for (int i = 0; i < curveElemsNo; i++) {
    if (c.elems[i].hTime == 0) break;
    if (c.elems[i].endTemp > maxTe) {
      maxI = i;
    }
  }
  return maxI;
}
*/
line GenLine(pointF p1, pointF p2) {
  float a = (p2.y - p1.y) / (p2.x - p1.x);
  float b = p2.y - a * p2.x;

  line l = { a, b };
  //Serial.println("GLf: a:" + String(a, 4) + " b:" + String(b, 4) + " p1:" + pF2s(p1) + " p2:" + pF2s(p2));
  return l;
}

line GenLine(point p1, point p2) {
  float a = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);
  float b = (float)p2.y - a * p2.x;

  line l = { a, b };
  //Serial.println("GL:a:" + String(a, 4) + " b:" + String(b, 4) + " p1:" + PrintPoint(p1) + " p2:" + PrintPoint(p2));
  return l;
}


String PrintPoint(point p) {
  return "{" + String(p.x) + ";" + String(p.y) + "}";
}
float lineVal(line l, long x) {
  return ((l.a) * (float)x + l.b);
}

float lineXfromY(line l, long y) {
  return ((float)y - l.b) / l.a;
}

void LedColor(int rL, int gL, int bL) {
  analogWrite(RED, constrain(rL, 0, 255));
  analogWrite(GREEN, constrain(gL, 0, 255));
  analogWrite(BLUE, constrain(bL, 0, 255));
}

void SetLed() {
  if (hOn) {
    LedColor(255, 0, 0);
  } else if (ON) {
    LedColor(0, 255, 0);
  } else {
    LedColor(0, 0, 255);
  }
}

int sign(int val) {
  return (val < 0 ? -1 : 1);
}

float costCal() {
  return (((float)totalHeatingTime / 3600) * ((float)power / 10) * ((float)cost / 100));
}

void Beep() {
  tone(BUZZERPIN, 864, 1000);
  tone(BUZZERPIN, 432, 500);
}