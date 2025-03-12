
void Kiln() {


  uint8_t fault = thermocouple.readFault();
  if (fault) {
    byte errorBeepNumber = 0;
    /*if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
    if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH) Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW) Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH) Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW) Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV) Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN) Serial.println("Thermocouple Open Fault");
    */
    if (fault & MAX31856_FAULT_CJRANGE) errorBeepNumber = 0;
    if (fault & MAX31856_FAULT_TCRANGE) errorBeepNumber = 1;
    if (fault & MAX31856_FAULT_CJHIGH) errorBeepNumber = 2;
    if (fault & MAX31856_FAULT_CJLOW) errorBeepNumber = 3;
    if (fault & MAX31856_FAULT_TCHIGH) errorBeepNumber = 4;
    if (fault & MAX31856_FAULT_TCLOW) errorBeepNumber = 5;
    if (fault & MAX31856_FAULT_OVUV) errorBeepNumber = 6;
    if (fault & MAX31856_FAULT_OPEN) errorBeepNumber = 7;
    //if(ON) ErrorBeep(errorBeepNumber + 1);
    if (ON) {
      tone(BUZZERPIN, 600, 120);
      tone(BUZZERPIN, 1200, 60);
      toShow = (String("e" + String(errorBeepNumber) + " " + thermocouple.readCJTemperature()) + " " + String(error));
      mLevel = 11;
      error++;
      //delay(1250);
    }
    delay(2250);
    Serial.println(errorBeepNumber);


    //lcd.setCursor(0, 0);

  }
  /*
  if (isnan(c)) {
    if (!error) {
      Serial.print(String(currentTemp / 100) + "TF");
      uint8_t e = thermocouple.readError();
      if (e & MAX31855_FAULT_OPEN) Serial.println("1");
      if (e & MAX31855_FAULT_SHORT_GND) Serial.println("2");
      if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("3");
      error = true;
      if (millis() - lastCorrectTempReadTime > hCycle * 2) {
        tone(BUZZERPIN, 320, 150);
        delay(100);
        tone(BUZZERPIN, 640, 150);
        if (ON) {
          toShow = "T/F!";
          HeatingOff();
        }
      } else {
        tone(BUZZERPIN, 200, 50);
      }
    }
  }*/
  else {
    //lastCorrectTempReadTime = millis();
    error = 0;
    //currentTemp = thermocouple.readThermocoupleTemperature()*100 * 1.026;
    //currentTemp = (thermocouple.readThermocoupleTemperature()*1.013648+18.8)*100;
    //currentTemp = (thermocouple.readThermocoupleTemperature() * thermocouple.readThermocoupleTemperature() * 0.0002085 + 1.2853 * thermocouple.readThermocoupleTemperature() + 4.2) * 100;
    //if (!isnan(thermocouple.readThermocoupleTemperature()) )
    currentTemp = thermocouple.readThermocoupleTemperature() * 100;
    //Serial.println(">"+String(thermocouple.readThermocoupleTemperature()));
    //currentTemp = avrT(c);
    //if ((millis() / 1000) % 3 == 0 && ON) {
    // Serial.println("t:" + String((float)(currentTemp / 100)));
    // Serial.println("l:" + String((lineVal(eL, millis()) / 100)));
    //}
    if (currentTemp > -1000 && currentTemp < 130000) {
      lcd.setCursor(0, 0);
      lcd.print(String(float(currentTemp) / 100, 1) + "  ");
    }
  }
}

/*
long avrT(long newTemp) {
  long toReturn = 0;
  //Serial.println(String(Temps[tempIndex]) + " " +String( newTemp));
  if (abs(Temps[tempIndex] - newTemp * 100) < 3500) {
    // tempIndex++;
    // if (tempIndex > 2) tempIndex = 0;
    // Temps[tempIndex] = (long)(newTemp * 100);
    // toReturn = 0;
  } else {
    tone(BUZZERPIN, 1032, 100);
    delay(20);
    tone(BUZZERPIN, 1564, 100);

    //hOn = false;
  }
  {
    tempIndex++;
    if (tempIndex > 2) tempIndex = 0;
    Temps[tempIndex] = (long)(newTemp * 100);
  }


  for (byte ii = 0; ii < 3; ii++) {
    toReturn += Temps[ii];
  }

  return toReturn / 3;
}

void fillTempArray() {

  //Serial.println("c"+String(c));
  for (byte ii = 0; ii < 3; ii++) {
    //Serial.println(ii);
    Temps[ii] = K2N(thermocouple.readCelsius()) * 100;
    Serial.println(String(ii) + " " + Temps[ii]);
    if (ii > 0 & abs(Temps[ii] - Temps[ii - 1]) > 300) {
      //Serial.println("t " + String(Temps[ii]) + " " + String(Temps[ii - 1]));

      ii = -1;
    }
    if (rl != 0 || millis() > 30000) break;
    delay(1000);
    //Serial.println("t"+Temps[ii]);
  }
  rl = 0;
}*/
/*
float K2S(float tempK) {
  return ((-0.00312158 * tempK * tempK + 4.91485 * tempK + 25.6876));

  //return tempK;
}
*/
/*
float K2N(float tempK) {
  if (tempK < 200) {
    return 0.0857143 + 1.52257 * tempK - 0.000942857 * tempK * tempK;
  } else {
    return 37.47126 + 1.154343 * tempK - 0.00005785 * tempK * tempK;
  }
}
*/