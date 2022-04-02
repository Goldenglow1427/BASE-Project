//BASE Sensor Data Decoder

#include <EEPROM.h>
#include <math.h>

int address;

int ad;

int Search() //Find how many data is in the EEPROM
{
  //From the last to the first
  for(int i = 1023; i >= 0; i--)
  {
    //The first one which is not 0 will be the position that has storage
    if(EEPROM.read(i) != 0)
      return i;
  }
  
  //Everything is 0: something might be wrong
  return -1;
}

void setup()
{
  Serial.begin(9600);
  
  ad = Search(); //Find out the location
}

void loop()
{
  //Data output process
  Serial.print("Maximum Ad: ");
  Serial.println(ad);
  
  //Only output useful data
  for(int i = 0; i <= ad; i++)
  {
    Serial.println(EEPROM.read(i));
  }
  
  while(1); //Stuck the program here
}
