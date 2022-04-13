
//************************************************************************************************************************************************************************
// Macro for buzzer beep battery alarm
//************************************************************************************************************************************************************************
byte pulseBuzzer; // Sound pulse for battery alarm

void beep()
{
  if (pulseBuzzer < 2)
  {
    tone(pin_buzzer, 900);
  }
  else if (pulseBuzzer > 2 && pulseBuzzer < 4)
  {
    noTone(pin_buzzer);
  }
  else if (pulseBuzzer > 4 && pulseBuzzer < 7)
  {
    tone(pin_buzzer, 900);
  }
  else if (pulseBuzzer > 7 && pulseBuzzer < 9)
  {
    noTone(pin_buzzer);
  }
  
  if (pulseBuzzer > 80)
  {
    pulseBuzzer = 0;
  }
  
  pulseBuzzer++;

/*
 // Another indication when an LED is used instead of a buzzer
  if (pulseBuzzer < 5)
  {
    digitalWrite(pin_buzzer, HIGH);
  }
  else
  {
    digitalWrite(pin_buzzer, LOW);
  }
  
  if (pulseBuzzer > 100)
  {
    pulseBuzzer = 0;
  }
  
  pulseBuzzer++;
*/  
}
 
