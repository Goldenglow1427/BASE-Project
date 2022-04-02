//User Guide: Input 1 to confirm clean the data
#include <EEPROM.h>

int m;

void setup()
{
  Serial.begin(9600); // Set communicate rate
  
  Serial.println("Please input 1 to start cleaning process"); //Prevent Accidentally clear all the data
}

void loop()
{
  if(Serial.available() > 0)
  {
    m = Serial.read();
    
    if(m == 49) //user input 1
    {
      Serial.println("Cleaning Process Begin"); //tell user the progress
      for(int i=0; i<=1023; i++)
      {
        EEPROM.write(i, 0);
        if(i % 100 == 0)
        {
          Serial.print(i/100);
          Serial.println(i==0 ? "%" : "0%"); //tell user the progress
        }
      }
      Serial.println("Data Cleared!"); 
      while(1); //stuck the program here
    }
    else
    {
      Serial.println("Wrong Cleaner Code");
      while(1); //stuck the program here
    }
  }
}
