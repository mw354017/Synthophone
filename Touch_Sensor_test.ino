/*****************************************************
 * This file is to test the touch sensors using the serial monitor built into the Arduino IDE.
 * This test case will alow us to analyze what values are given with touching the probes in various ways.
 ****************************************************/

void setup() 
{
  // Initializes serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() 
{
  //Read from each touch sensor and print the value
  Serial.print(touchRead(A9));
  Serial.println();

  Serial.print(touchRead(A8)); 
  Serial.println();

  Serial.print(touchRead(A5));
  Serial.println();

  Serial.print(touchRead(A4));
  Serial.println(); 
  Serial.println();

  /// delay 100 milliseconds (1/10 of a second)
  delay(100);
}
