//BASE Sensor Data Encoder
// !!!!! LIS Code comes from ...?
#include <EEPROM.h> //For EEPROM Storage
#include <Wire.h> //BH1750 IIC Mode
#include <math.h>

int BH1750address = 0x23; //setting i2c address
int address = 0; //Adress for EEPROM

//The data from SM Sensors
int a1, a2, a3;

//Average of 3 SM Sensor
int average;
int timer = 0; //Timer as seconds to detect wether it is 20 minutes to store the data, and count every second to accumulate the data
int sum = 0; //Sum of every seconds of the average of SMS
int light = 0; //For blinkning the LED light

int period; //The period of recording data (period = 60 means every 60 seconds a data will be recorded)

//Convert the data range from [a,b] to [c,d]
int numberConvertion(double old_value, double old_min, double old_max, double new_min, double new_max)
{
  double new_value;
  new_value = (old_value - old_min) / (old_max - old_min) * (new_max - new_min)  + new_min;
  return int(new_value);
}

void setup()
{
  address = 0; //For data input
  
  Wire.begin(); //LI Sensor
  Serial.begin(9600);//init Serial rate

  pinMode(LED_BUILTIN, OUTPUT); //Set up the blinking the LED
}

void loop()
{
  int i; //I don't know why here's an i either :(
  
  //Convert the SI Sensor's data into int[0,255] to store it
  a1 = numberConvertion(analogRead(A1), 100, 1000, 0, 255);
  a2 = numberConvertion(analogRead(A2), 100, 1000, 0, 255);
  a3 = numberConvertion(analogRead(A3), 100, 1000, 0, 255);
  
//  a1 = analogRead(A1);
//  a2 = analogRead(A2);
//  a3 = analogRead(A3);

  //print it on the computer for testing
  Serial.print("SM Sensor 1: ");
  Serial.println(a1);
  
  Serial.print("SM Sensor 2: ");
  Serial.println(a2);
  
  Serial.print("SM Sensor 3: ");
  Serial.println(a3);
  
  average = (a1 + a2 + a3) / 3; //Calculate the avergae of SMS since we only want to store the average
  sum = sum + average; //Accumulate(Update) the sum of SMS

  //print on the computer for testing
  Serial.print("Average Soil Moisture: ");
  Serial.println(average);
  Serial.print("Timer is: ");
  Serial.println(timer);
  
  //Letting the LED blink so that we can now the Arduino is Recording immediately
  if (light == 0){
    digitalWrite(LED_BUILTIN, HIGH);  
    light = 1 - light;
  }
  else if (light == 1){
    digitalWrite(LED_BUILTIN, LOW);
    light = 1 - light;
  }

  
  delay(950 * 30); //Delay 30 seconds 

  timer++; //add a count to the timer
  if (timer >= 40) //when 20 minutes past (should be 1200 seconds in experiment)
  {
    EEPROM.write(address++, sum / 40); //Only save average SM Sensor data
    
    //print on the computer for testing
    Serial.print("Average of SMS in 60 seconds is:");
    Serial.println(sum / 40);
    
    timer = 0; //clear the timer for next time's storing
    sum = 0; //clear the sum of SM for next time's storing
  }

  Serial.println();

  if(address >1023)
  {
    while(1); //finish and stop recording
  }
  
}
