//BASE Sensor Data Encoder

#include <EEPROM.h>
#include <Wire.h> //BH1750 IIC Mode
#include <math.h>

int BH1750address = 0x23; //setting i2c address
int address = 0;

//The data from SM Sensors
int a1, a2, a3;

//Average of 3 SM Sensor
int average;

//For LI Sensor
byte buff[2];

int Search() //Find how many data is in the EEPROM
{
	//From the last to the first
	for(int i = 1023; i >= 0; i--)
	{
		//The first one which is not 0 will be the position that has storage
		if(EEPROM.read(i) != 0)
			return i+1;
	}
	
	//Everything is 0: something might be wrong
	return 0;
}

//Convert the data range from [a,b] to [c,d]
int numberConvertion(double old_value, double old_min, double old_max, double new_min, double new_max)
{
	double new_value;
	new_value = (old_value - old_min) / (old_max - old_min) * (new_max - new_min)  + new_min;
	return int(new_value);
}

//For LI Sensor
int BH1750_Read(int address)
{
	int i = 0;
	Wire.beginTransmission(address);
	Wire.requestFrom(address, 2);
	while(Wire.available())
	{
		buff[i] = Wire.read();  // receive one byte
		i++;
	}
	Wire.endTransmission();
	return i;
}

//For LI Sensor
void BH1750_Init(int address)
{
	Wire.beginTransmission(address);
	Wire.write(0x10);//1lx reolution 120ms
	Wire.endTransmission();
}

void setup()
{
	//Begin recording from the broke point
	address = Search();
	
	Wire.begin(); //LI Sensor
	Serial.begin(9600);//init Serial rate 
}

void loop()
{
	int i; //I don't know why here's an i either :(
	
	//uint16_t val=0;
	int val = 0;
//	BH1750_Init(BH1750address);
	
	//Read the data from LI Sensor
	if(BH1750_Read(BH1750address) == 2)
	{	
		val = ((buff[0]<<8)|buff[1])/1.2;
		Serial.print("Light Intensity: ");
		Serial.print(val / 60); //The data should be in the range of int[0,255]
		Serial.println("[lx]");
	}
	
	//Convert the SI Sensor's data into int[0,255]
	a1 = numberConvertion(analogRead(A1), 300, 600, 0, 255);
	a2 = numberConvertion(analogRead(A2), 300, 600, 0, 255);
	a3 = numberConvertion(analogRead(A3), 300, 600, 0, 255);
	
//	a1 = analogRead(A1);
//	a2 = analogRead(A2);
//	a3 = analogRead(A3);
	
	Serial.print("SM Sensor 1: ");
	Serial.println(a1);
	
	Serial.print("SM Sensor 2: ");
	Serial.println(a2);
	
	Serial.print("SM Sensor 3: ");
	Serial.println(a3);
	
	average = (a1 + a2 + a3) / 3;
	
	Serial.print("Average Soil Moisture: ");
	Serial.println(average);
	
	Serial.println();
	
	EEPROM.write(address++, val / 60);
	EEPROM.write(address++, average); //Only save average SM Sensor data
	
	delay(60000); //Delay 1min (It should be 20min in real experiment)
}
