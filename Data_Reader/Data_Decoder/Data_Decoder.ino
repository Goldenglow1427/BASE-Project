//BASE Sensor Data Decoder

#include <EEPROM.h>
#include <math.h>

int address;

int time[10]; //1->6: yyyy/mm/dd: hh/mm/ss
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

void printTime()
{
  Serial.print(time[1]);
  Serial.print("/");
  
  Serial.print(time[2]);
  Serial.print("/");
  
  Serial.print(time[3]);
  Serial.print(" ");
  
  Serial.print(time[4]);
  Serial.print(":");
  
  Serial.print(time[5]);
  Serial.print(":");
  
  Serial.println(time[6]);
}

void Update(int x)
{
  time[5] += x;
  if(time[5] >= 60)
  {
    time[5] -= 60;
    time[4]++;
  }
  if(time[4] >= 24)
  {
    time[4] -= 24;
    time[3]++;
  }
  if(time[3] >= 30)
  {
    time[3] -= 30;
    time[2]++;
  }
  if(time[2] >= 12)
  {
    time[2] -= 12;
    time[1]++;
  }
}

void setup()
{
  Serial.begin(9600);
  
  ad = Search(); //Find out the location
  
  time[1] = 2022;
  time[2] = 3;
  time[3] = 30;
  time[4] = 10;
  time[5] = 05;
  time[6] = 0;
}

void loop()
{
  Serial.println("Please input the beginning time :)");
  delay(3000);
  
  //Data output process
  Serial.print("Maximum Ad: ");
  Serial.println(ad);
  
  //Only output useful data
  for(int i = 0; i <= ad; i++)
  {
    if(i % 2 == 0)
    {
      printTime();
      Update(20);
      Serial.print("Light Intensity: ");
      Serial.println(EEPROM.read(i));
    }
    else
    {
      Serial.print("Soil Moisture: ");
      Serial.println(EEPROM.read(i));
      Serial.println();
    }
  }
  
  while(1);
}
