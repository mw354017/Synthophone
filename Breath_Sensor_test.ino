/*****************************************************
 * This file is to test the breath sensor using the serial monitor and plotter built into the Arduino IDE.
 ****************************************************/

/// The minimum value at which to start sending midi data.
int BREATH_THRESHOLD=150;

/// The 7 bit number that midi uses for Control Change.
int ccValue;

void setup() 
{
  // Initializes serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() 
{
  /// Read the input from the pressure sensor on analog pin 0.
  int sensorValue1 = analogRead(A0);
  /// Map pressure values to a 7 bit number.
  ccValue = map(sensorValue1, BREATH_THRESHOLD, 1023, 0, 127);
  /// Prints the value straicht from the sensor.
  Serial.print(sensorValue1);
  /// Optionally prints the value that would be sent over MIDI Control Change.
  //Serial.print(ccValue);
  Serial.println();
  /// delay 100 milliseconds (1/10 of a second)
  delay(100);
}
