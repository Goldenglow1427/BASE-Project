//User Guide: Input 1 to confirm clean the data
#include <EEPROM.h>

int m;

void setup()
{
	Serial.begin(9600);
	
	//Avoid the mistake touching
	Serial.println("Please input 1 to start cleaning process");
}

void loop()
{
	if(Serial.available() > 0) //Check the data input
	{
		//Read the input
		m = Serial.read();
		
		if(m == 49) //1 in ASCII is 49
		{
			//Correct input and clean the data
			Serial.println("Cleaning Process Begin");
			for(int i=0; i<=1023; i++)
			{
				EEPROM.write(i, 0); //Set all the storage as 0
				
				//For every 100 data cleared, print out the progress
				if(i % 100 == 0)
				{
					Serial.print(i/100);
					Serial.println(i==0 ? "%" : "0%");
				}
			}
			Serial.println("Data Cleared!"); //Finish data clearing
			while(1);
		}
		else
		{
			//Mistake touching
			Serial.println("Wrong Cleaner Code");
			while(1);
		}
	}
}
