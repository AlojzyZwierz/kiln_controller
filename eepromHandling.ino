

//-------------------------------------------------------------------------------------eeprom
/*
void WriteEl(eeElement el, int index) {
  for (int i = 0; i < 4; i++) {
    EEPROM.put(index + i, el.data[i]);
  }
}

eeElement ReadEl(int index) {
  eeElement toRet;
  for (int i = 0; i < 4; i++) {
    uint8_t data;
    EEPROM.get(i + index, data);
    toRet.data[i] = data;
  }
  return toRet;
}
*/

eeElement cToEE(CElement cEl) {
  int a = cEl.endTemp / 100;
  int b = cEl.hTime / 60000;
  return { highByte(a), lowByte(a), highByte(b), lowByte(b) };
}

CElement EEtoC(eeElement el) {
  long a = el.data[0];
  a = a << 8;
  a |= el.data[1];
  long b = el.data[2];
  b = b << 8;
  b |= el.data[3];
  CElement toReturn;
  toReturn.endTemp = a * 100;
  toReturn.hTime = b * 60000;
  return toReturn;
}
/*
void Int2Eeprom(int n, int i) {

  EEPROM.put(i, highByte(n));
  EEPROM.put(i + 1, lowByte(n));
//serial.println("Int2Eeprom: " + String(n) + " at " + String(i)+ "&" + String(i+1));
}

int IntFromEeprom(int index) {
  uint8_t data;  
  EEPROM.get(index, data);  
  int a = data;
  a = a << 8;
  EEPROM.get(index+1, data);  
  a |= data;
//serial.println("IntFromEeprom " + String(index) +" " + String(a));
  return a;
}

void Float2ee(float rla,int index ){
  Int2Eeprom((int)(rla*100), index);
}
float FloatFromEe(int index)
{
  return ((float)IntFromEeprom(index))/100;
}
*/
void WriteCurve(Curve cu, int mem) {
  for (int i = 0; i < curveElemsNo; i++) {
    eeElement ee = cToEE(cu.elems[i]);
    //WriteEl(ee, (mem + i * 4) + 1);
    EEPROM.put((mem + i * 4) + 1,ee)   ;  
  }
}

Curve ReadCurve(int mem) {
  Curve c;
  for (int i = 0; i < curveElemsNo; i++) {
    //c.elems[i] = EEtoC(ReadEl((mem + i * 4) + 1));
    eeElement tempElem;
    EEtoC(EEPROM.get((mem + i * 4) + 1,tempElem));
    c.elems[i] =EEtoC(tempElem);
  }
  return c;
}

void WriteCurveInd() {
  int memIndex = curveIndex * curveElemsNo * 4;
  WriteCurve(cur, memIndex);
//serial.println("WriteCurveInd:" + String(curveIndex) + " " + String(memIndex));
}

void ReadCurveInd() {
  int memIndex = curveIndex * curveElemsNo * 4;
  cur = ReadCurve(memIndex);
//serial.println("ReadCurveInd:" + String(curveIndex) + " " + String(memIndex));
}