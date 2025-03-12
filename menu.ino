
void Menu() {
  if (ON && joystickButtonPressed && mLevel != 9) {
    mLevel = 7;
    joystickButtonPressed = 0;
    mChanged = 1;
  }

  switch (mLevel) {
      // if (mLevel == 0) {
    case 0:
      {
        String m0[3] = {
          ON ? "stop" : "start",
          ON ? "view program" : "edit programs",
          String(costCal()) + " zl"
        };
        //mPos = clampBeep(mPos + !ON?ud:2*ud, 0, 2);
        mPos = clampBeep((mPos + ud), 0, 2);
        if (rl == 1) {
          //Beep(rl != 50);
          if (mPos == 1) {
            if (ON) {
              mLevel = 8;
              editElIndex = currElemIndex;
            } else {
              mLevel = 4;
            }
            //mLevel = ON ? 8 : 4;
          } else if (mPos == 0) {
            //if (maxthermo.readThermocoupleTemperature()!= 0.0 && !isnan(maxthermo.readThermocoupleTemperature())) {
            if (!ON) {
              mLevel = 6;
            } else {
              mLevel = 7;
            }
            //}
          } else {
            menuMoveOk = false;
          }
        }
        if (rl == -1) menuMoveOk = false;
        if (rl == -1 && mPos == 2) {

          if (secretMenuTimer > millis() || secretMenuCount == 0) {
            secretMenuTimer = millis() + 1500;
            secretMenuCount++;
            //Serial.println("+ " + String(secretMenuCount));
          } else {
            secretMenuCount = 0;
            // Serial.println("- " + String(secretMenuCount));
          }
          if (secretMenuCount > 2) {
            mLevel = 9;
            mPos = 0;
          }
        }

        toShow = String(m0[mPos]);
        //} else if (mLevel == 4) {
        break;
      }
    case 4:
      {
        if (rl == -1) {

          mLevel = 0;
          mPos = 1;
        } else if (rl == 1) {
          editElIndex = 0;
          mLevel = 5;
        }
        if (cChanged) {
          cChanged = false;
          WriteCurveInd();
        }
        toShow = ChooseCurve();
        break;
        // } else if (mLevel == 5) {
        //toShow = EditCurve();
      }
    case 5:
      {
        if (rl == -1) {
          editElIndex--;
          if (editElIndex < 0) {
            mLevel = 4;
            mPos = 0;
            editElIndex = 0;
          }
        } else if (rl == 1) {
          editElIndex++;
        }
        editElIndex = clampBeep(editElIndex, 0, curveElemsNo * 3 - 1);
        EditCurve();
        //toShow = " test ";
        break;
        //} else if (mLevel == 6) {
      }
    case 6:
      {
        if (rl == -1) {

          mLevel = 0;
          mPos = 0;
        } else if (rl == 1) {

          mLevel = 7;
        }

        toShow = ChooseCurve();
        break;
        //} else if (mLevel == 7) {
      }
    case 7:
      {
        if (ud != 0) menuMoveOk = false;
        if (rl == -1) {

          mLevel = ON ? 0 : 6;
          mPos = 0;
        } else if (rl == 1) {

          if (!ON) {
            //Serial.println("ON");
            ON = true;
            StartProgram();
          } else {

            HeatingOff();
          }

          //digitalWrite(LED_BUILTIN, ON ? HIGH : LOW);
          //   if (ON) {

          //   }
        }
        toShow = ((ON ? "stop " : "start ") + String(curveIndex + 1) + "?");
        break;
        //} else if (mLevel == 8) {
      }
    case 8:
      {
        ViewCurve(cur);
        break;
        //} else if (mLevel == 9) {
      }
    case 9:
      {
        mPos = clampBeep(mPos + ud, 0, 8);
        if (mPos == 0 && ud == 1) mLevel = 0;
        if (rl != 0) {
          switch (mPos) {
            case 0:
              {
                kp += ((float)rl / 20) * kp;
                break;
              }
            case 1:
              {
                kd += ((float)rl / 20) * kd;
                break;
              }
            case 2:
              {
                ki += ((float)rl / 20) * ki;
                break;
              }
            case 3:
              {
                hCycle += (float)rl * 1000;
                break;
              }
            case 4:
              {
                PIDCheckInterval += (float)rl * 1000;
                break;
              }
            case 7:
              {
                power += rl;
                break;
              }
            case 8:
              {
                cost += rl;
                break;
              }
          }
        }

        //toShow = m9[mPos] + String(mPos == 0 ? kp : (mPos == 1 ? kd : ki), 6);
        //toShow = m9[mPos];
        if (mPos == 0) toShow = "kp:" + String(kp, 6);
        else if (mPos == 1) toShow = "kd:" + String(kd, 6);
        else if (mPos == 2) toShow = "ki:" + String(ki, 6);
        else if (mPos == 3) toShow = "hc:" + String(hCycle / 1000);
        else if (mPos == 4) toShow = "ct:" + String(PIDCheckInterval / 1000);
        else if (mPos == 5) toShow = String(pro) + " " + String(der) + " " + String(integ);
        else if (mPos == 6) toShow = String(ratio) + " " + String(thermocouple.readThermocoupleTemperature()) + " " + String(thermocouple.readCJTemperature());
        else if (mPos == 7) toShow = "pow:" + String(((float)power / 10), 1);
        else if (mPos == 8) toShow = "pri:" + String((float)cost / 100);
        if (joystickButtonPressed) {
          mLevel = 0;
          mPos = 0;
          secretMenuCount = 0;
          joystickButtonPressed = 0;
          upChanged = true;
          EEPROM.put(KPEE, kp);
          EEPROM.put(KDEE, kd);
          EEPROM.put(KIEE, ki);
          EEPROM.put(HCEE, (int)(hCycle / 1000));
          EEPROM.put(CTEE, (int)(PIDCheckInterval / 1000));
          EEPROM.put(POWEREE, power);
          EEPROM.put(COSTEE, cost);
        }
        break;
        //} else if (mLevel == 11) {
      }
    case 11:
      {
        //toShow = "Error! " + toShow ;
        if (rl != 0 || ud != 0) {
          mLevel = 0;
          mPos = 0;
          mChanged = 1;
        }
      }
  }


  if (mChanged) {

    MenuShow(toShow);
    mChanged = false;
  }
  if ((upChanged || ud != 0 || rl != 0)) {
    //(rl != 0 ? tone(BUZZERPIN, 1000, 50) : tone(BUZZERPIN, 2500, 50));
    {
      if (menuMoveOk) {
        tone(BUZZERPIN, 3000, 50);
      } else {
        tone(BUZZERPIN, 300, 90);
      }
    }
    menuMoveOk = true;
    toShow = "";
    upChanged = false;
    ud = 0;
    rl = 0;
    mChanged = true;
  }
}


String ChooseCurve() {
  String toReturn = "";

  curveIndex = clampBeep((int)curveIndex - ud, 0, curvesNo);
  /*
  if (upChanged) {
    curveIndex = clampBeep(curveIndex +sign(upV) * 3, 0, curvesNo);
  }
  */
  //Serial.println("upv " + upV);
  //Serial.println(upChanged);
  if (mChanged) {
    ReadCurveInd();
    maxTemp = GetMaxTemp(cur);
    toReturn = (String(curveIndex + 1) + "." + String(maxTemp / 100));
  }
  return toReturn;
}

void EditCurve() {

  int realElIndex = editElIndex / 3;
  //String toReturn = String(realElIndex) + ".";

  // lcd.setCursor(7, 0);
  //lcd.print(String(curveIndex) + " " + String(editElIndex) + " " + String(realElIndex));


  if (editElIndex % 3 == 0) {
    cur.elems[realElIndex].endTemp -= ud * 100;

    if (joystickButtonPressed && realElIndex != 0) {
      cur.elems[realElIndex].endTemp = cur.elems[realElIndex - 1].endTemp;
      joystickButtonPressed = 0;
      mChanged = true;
    }

    if (upChanged) {
      cur.elems[realElIndex].endTemp -= constrain(upV - 512, -500, 500) * 10;


      cur.elems[realElIndex].endTemp = clampBeep(cur.elems[realElIndex].endTemp, 2000, 128000);
      //cur.elems[realElIndex].endTemp = cur.elems[realElIndex].endTemp;
    }
    toShow = (String(realElIndex + 1) + ".T: " + String(cur.elems[realElIndex].endTemp / 100) + "'C");

    /* if (ud != 0 || upChanged) {
      tone(BUZZERPIN, map(cur.elems[realElIndex].endTemp, 2000, 130000, 500, 2400), 50);
    }*/
  } else if (editElIndex % 3 == 1) {
    cur.elems[realElIndex].hTime -= ud * 60000;

    if (upChanged) {
      cur.elems[realElIndex].hTime -= ((long)(upV - 512) * 2500);
    }

    cur.elems[realElIndex].hTime = clampBeep(cur.elems[realElIndex].hTime, 60000, 72000000);
    //Serial.println(String(cur.elems[realElIndex].endTemp )+ " " + String(realElIndex == 0?currentTemp:cur.elems[realElIndex-1].endTemp) + " "+String((float)cur.elems[realElIndex].hTime/3600000.0));
    if (cur.elems[realElIndex].hTime != 60000) {
      long tempIncrSpeed = (cur.elems[realElIndex].endTemp - (realElIndex == 0 ? currentTemp : cur.elems[realElIndex - 1].endTemp)) / ((float)cur.elems[realElIndex].hTime / 36000.0);
      //bool htime = (cur.elems[realElIndex].hTime != 60000);
      String hm = millisToHM(cur.elems[realElIndex].hTime);
      // Serial.println(">>> " + hm);
      toShow = (String(realElIndex + 1) + ".t: " + hm + " (" + String(tempIncrSpeed) + ")");
    } else {
      toShow = (String(realElIndex + 1) + ". /skip/ ");
    }
    //MenuShow ("time: " + String(cur.elems[realElIndex].hTime/60000));
    //if (ud != 0 || upChanged) { tone(BUZZERPIN, map(cur.elems[realElIndex].hTime, 2000, 13000000, 500, 2000), 50); }
  } else {
    if (ud != 0) {
      mLevel = 4;
      editElIndex = 0;
      if (realElIndex + 1 < curveElemsNo) {
        cur.elems[realElIndex + 1].hTime = 0;
      }
      tone(BUZZERPIN, 1300, 200);
    }
    toShow = ("end? ^");
  }
  if (upChanged || ud != 0) {
    cChanged = true;
  }
}

void MenuShow(String str) {
  {

    lcd.setCursor(0, 1);
    lcd.print(str);
    ClearRest();
    //Serial.print(">.>" + String(mLevel) + "." + String(mPos) + str + ">>");
  }
}

void ClearRest() {
  for (byte i = 0; i < 16; i++) {
    lcd.print(' ');
  }
}
void PrintInfo(String info1, String info2) {
  lcd.setCursor(8, 0);
  if ((millis() / 3000) % 2 == 1) {
    lcd.print(info1);

  } else {
    lcd.print(info2);
    //Serial.println(maxTemp);
  }
  ClearRest();
}
void ViewCurve(Curve curv) {
  editElIndex += rl;
  if (editElIndex < 0) {
    mPos = 1;
    mLevel = 0;
    editElIndex = 0;
  } else if (editElIndex >= curveElemsNo || curv.elems[editElIndex].hTime == 0) {
    editElIndex--;
  }

  if (curv.elems[editElIndex].hTime == 60000) {
    toShow = String(editElIndex + 1) + " " + String(curv.elems[editElIndex].endTemp / 100) + " SKIP ";
  } else {
    //Serial.println(String( curv.elems[editElIndex].endTemp-startTemp) + " s: "+ String(startTemp)+ " e: " + curv.elems[editElIndex].endTemp + " " +String(percent));
    toShow = String(editElIndex + 1) + " " + String(curv.elems[editElIndex].endTemp / 100) + " " + String(millisToHM(curv.elems[editElIndex].hTime));
  }

  if (currElemIndex == editElIndex) {
    int percent;
    if (currElemIndex > 0 && cur.elems[currElemIndex].endTemp == curv.elems[editElIndex - 1].endTemp) {
      percent = constrain(map(millis(), startTime, endTime, 0, 100), 0, 100);
    } else {
      percent = constrain(map(currentTemp, startTemp, curv.elems[editElIndex].endTemp, 0, 100), 0, 100);
    }
    toShow += (" " + String(percent) + "%");
  }
}
