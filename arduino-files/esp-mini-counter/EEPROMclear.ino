#include <EEPROM.h>

void resetEEPROM (bool debug)
{
  if(debug)
  {
    int len=EEPROM.length();
    Serial.print("EEPROM length: ");
    Serial.println(len);
  }
  for(int i=0; i<EEPROM.length();i++)
  {
    if(debug)
    {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(EEPROM.read(i));
    }
    EEPROM.write(i,0);
  }
}
