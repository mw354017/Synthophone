/*****************************************************
 * This file is to test the touch sensors using the serial monitor built into the Arduino IDE.
 ****************************************************/

/// The 7 bit number that midi uses for Control Change.

void setup() 
{
  // Initializes serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() 
{

  Serial.print(touchRead(A9));
    Serial.println();

  Serial.print(touchRead(A8)); 
  Serial.println();

  Serial.print(touchRead(A5));
    Serial.println();

  Serial.print(touchRead(A4));

  /// Optionally prints the value that would be sent over MIDI Control Change.
  //Serial.print(ccValue);
  Serial.println();
    Serial.println();

  /// delay 100 milliseconds (1/10 of a second)
  delay(100);
}
