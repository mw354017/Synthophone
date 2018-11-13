/*****************************************************
 * This file is to test the touch sensors using the serial monitor built into the Arduino IDE.
 * This test case evaluates the probes use as a digital switch. Data from Touch_Sensor_test was analyzed to select this cut off value.
 ****************************************************/

void setup() 
{
  // Initializes serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() 
{
  //These if statements print a 1 if the probe is touched, and a 0 if not.
  //This will let us analyze if this value is appropriate for the probes to function as switches.
  if(touchRead(A9)>2000)
  {
    Serial.print(1);
  }
  else
  {
    Serial.print(0);
  }
  Serial.println();

  
  if(touchRead(A8)>2000)
  {
    Serial.print(1);
  }
  else
  {
    Serial.print(0);
  }
  Serial.println();

  
  if(touchRead(A4)>2000)
  {
    Serial.print(1);
  }
  else
  {
    Serial.print(0);
  }
  Serial.println();

  
  if(touchRead(A5)>2000)
  {
    Serial.print(1);
  }
  else
  {
    Serial.print(0);
  }
  Serial.println();
  Serial.println();

  /// delay 100 milliseconds (1/10 of a second)
  delay(100);
}
