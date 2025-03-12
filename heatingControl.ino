void UsePID() {
  // dt = PIDCheckInterval;
  er = lineVal(eL, millis()) - currentTemp;
  if ((lastErr > 0 && er < 0) || (lastErr < 0 && er > 0)) {
    erSum = 0;
  }
  erSum += er * (PIDCheckInterval / 1000);
  integ = constrain(((double)ki / 100000) * erSum, -1, 1);
  der = kd * (er - lastErr) / (PIDCheckInterval / 100);
  pro = kp * er / 10000;
  float pidCorr = (pro + der + integ);
  float HeatCoolCorr = 1;
  float hCCFactor = 0.5;
  float hCCTemp = 60000;
  //HeatCoolCorr = 1 +  ((hCCTemp - currentTemp)/hCCTemp*2) * hCCFactor;
  /*if (currentTemp < 60000 && pidCorr > 0) {
    HeatCoolCorr = 0.001 * currentTemp / 100 + 0.4;
  } else if (currentTemp > 60000 && pidCorr < 0) {
    HeatCoolCorr = -0.001 * currentTemp / 100 + 1.6;
  }
  */
  //Serial.println(String(currentTemp / 100) + " hcc>" + String(HeatCoolCorr) + " " + String(er / 100));
  ratio = constrain(ratio, 0, 1) + HeatCoolCorr * pidCorr;
  //Serial.println("rat"+String(ratio));
  //Serial.println("pid>r_" + String(ratio, 3) + "_er_" + String((float)er / 100) + "_pr_" + String(pro) + "_d_" + String(der, 4) + "_i_" + String(integ, 4) + "_s_" + String(erSum));
  lastErr = er;
}


void UseElement() {
  startTime = millis();
  nextPIDCheck = millis() + PIDCheckInterval;
  endTime = startTime + (unsigned long)cur.elems[currElemIndex].hTime;
  startTemp = currElemIndex == 0 ? currentTemp : cur.elems[currElemIndex - 1].endTemp;  //wyznaczyć prostą wg danych z krzywej a potem wyznaczyć miejsce na prostej wg temperatury obecnej
  //point q1 = { startTime, (currElemIndex==0)?currentTemp:startTemp };

  point q1 = { startTime, startTemp };
  point q2 = { endTime, cur.elems[currElemIndex].endTemp };
  float formerElA = eL.a;
  eL = GenLine(q1, q2);
  endTime = (endTime - lineXfromY(eL, currentTemp)) + millis();
  point q1 = { startTime, currentTemp };
  point q2 = { endTime, cur.elems[currElemIndex].endTemp };
  eL = GenLine(q1, q2);
  if (eL.a < formerElA) ratio = 0;


  maxSkipTemp = 0;
}


void NextEl() {
  if (((eL.a > 0.0002 || cur.elems[currElemIndex].hTime == 60000) && cur.elems[currElemIndex].endTemp < currentTemp) || (eL.a < -0.0002 && cur.elems[currElemIndex].endTemp > currentTemp) || (abs(eL.a) <= 0.0002 && endTime < millis())) {


    if (GetMaxTemp(cur) == cur.elems[currElemIndex].endTemp) {
      uint8_t nn = 'c';
      EEPROM.put(EEEMERG, nn);
    }
    if (currElemIndex + 1 >= curveElemsNo || cur.elems[currElemIndex + 1].hTime == 0) {
      ProgramEnded();
      return;
    }

    if (currElemIndex == editElIndex) editElIndex++;

    currElemIndex++;
    //Serial.println("NE>" + String(currElemIndex));
    tone(BUZZERPIN, 1500, 220);
    UseElement();
  }
}

void TurnHeatingOn() {
  //unsigned long hOnInCycle = ((ratio < 0) ? 0 : ratio) * hCycle;
  unsigned long hOnInCycle = constrain(ratio, 0.0, 1.0) * hCycle;
  unsigned long timeFromCycleStart = (millis() - hCycleStartTime);

  if (ON && hOnInCycle > timeFromCycleStart) {
    hOn = 1;
    digitalWrite(SSR, HIGH);
    //lcd.setCursor(0, 0);
    //lcd.print("^");
    //Serial.println("on");

  } else {
    hOn = 0;
    digitalWrite(SSR, LOW);
    //lcd.setCursor(0, 0);
    //lcd.print(" ");
    //Serial.println("of");
  }
  if (timeFromCycleStart >= hCycle) {
    hCycleStartTime = millis();
  }
}

uint8_t CheckTempOnCurve(Curve cu) {
  uint8_t elemWithCurrenTemp = 0;
  for (int i = 0; i < curveElemsNo; i++) {
    if (cu.elems[i].hTime == 0) break;
    if (cu.elems[i].endTemp > currentTemp) {
      elemWithCurrenTemp = i;
      break;
    }
  }
  return elemWithCurrenTemp;
}


void SumHeatingTime() {
  if (hWasOn != 1 && hOn == 1) {
    hStartTime = millis();
  }
  if (hOn != 1 && hWasOn == 1) {
    totalHeatingTime += ((millis() - hStartTime) / 1000);
  }
  hWasOn = hOn;
}

void StartProgram() {
  if (cur.elems[0].hTime == 0) {
    HeatingOff();
    return;
  }
  currElemIndex = CheckTempOnCurve(cur);
  maxTemp = GetMaxTemp(cur);
  ON = true;
  //digitalWrite(LEDPin, 255);
  UseElement();
  //maxTemp = GetMaxTemp(cur);
  totalHeatingTime = 0;
  ratio = 0.4;
  mLevel = 8;
  editElIndex = currElemIndex;
  EEPROM.put(EEEMERG, curveIndex);
}

void ProgramEnded() {
  tone(BUZZERPIN, 1728, 150);
  delay(100);
  tone(BUZZERPIN, 2592, 150);
  delay(100);
  tone(BUZZERPIN, 3456, 1200);
  HeatingOff();
  //MenuShow("End");
  toShow = "End! " + String(costCal(), 2) + " zl";
  mLevel = 11;
  PrintInfo(" ", " ");
}

void EmergencyStart() {
  uint8_t emerg;
  emerg = EEPROM.read(EEEMERG);
  //Serial.println("EE.g:" + String(emerg));

  if (emerg != 99 && thermocouple.readThermocoupleTemperature() != 0 && !isnan(thermocouple.readThermocoupleTemperature())) {

    MenuShow("R" + String(emerg + 1));
    delay(1000);
    curveIndex = emerg;
    ReadCurveInd();
    StartProgram();
  }
}
void HeatingOff() {
  ON = false;
  hOn = false;
  lcd.setCursor(8, 0);
  ClearRest();
  uint8_t nn = 'c';
  EEPROM.put(EEEMERG, nn);
  //EEPROM.put(1020, 99);
  //-----------
  //uint8_t emerg = 5;
  //EEPROM.get(EEEMERG, emerg);
  //Serial.println("OFF");
}

bool SkipMax() {
  if (cur.elems[currElemIndex].hTime == 60000) {
    //Serial.println(String(millis()) + " " + String(millis() - maxSkipTime));
    if (currentTemp > maxSkipTemp) {
      //Serial.println("!" + String(millis()));
      maxSkipTemp = currentTemp;
      maxSkipTime = millis();
    }
    if (millis() - maxSkipTime > 200000) {
      return true;
    }
  }
  return false;
}