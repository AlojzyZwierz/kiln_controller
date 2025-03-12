
//-----------------------------------------------input

void readXY() {
  int xRead = analogRead(VERT_PIN);
  if (xRead < 480 || xRead > 530) {
    if (xZeroed) {
      if (xRead < 200 || xRead > 800) {
        ud = ((xRead < 512) ? (1) : -1);
        xZTime = millis();
        xZeroed = false;
      }
    }

    else {
      if ((millis() - xZTime) > joyDelay) {

        upV = 1023-xRead;
        upChanged = true;
        xZTime = millis();
      }
    }
  } else {
    xZeroed = true;
  }


  int yRead = analogRead(HORZ_PIN);
  if (yRead < 200 || yRead > 800) {
    if (yZeroed) {
      rl = ((yRead < 512) ? (-1) : 1);
      //yPos = clamp((yRead < 512) ? (yPos + 1) : yPos - 1, 0, 3000);
      yZTime = millis();
      yZeroed = false;
    }

  } else {
    yZeroed = true;
  }
}


void GetButtons() {
  if (ignore) {
    if ((millis() - lastCheck) > checkInterval) {
      if (lastState == digitalRead(buttonPin)) {
        if (!lastState) {
          joystickButtonPressed = true;
          //upChanged = false;
          // currentState = true;
        }
      }
      ignore = false;
    }

  } else {
    bool currentButtonState = digitalRead(buttonPin);

    if (currentButtonState != lastState) {
      lastCheck = millis();
      ignore = true;
    }
    lastState = currentButtonState;
  }
  // currentState = false;
}

