const int wheels = 2;
const int sonar = 7;
int sonarVal = 0;

void setup() {
  pinMode(wheels, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long duration, inches, cm;
  pinMode(sonar, OUTPUT);
  digitalWrite(sonar, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar, HIGH);
  delayMicroseconds(5);
  digitalWrite(sonar, LOW);
  
  pinMode(sonar, INPUT);
  duration = pulseIn(sonar, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  if (inches > 6.0) {
    digitalWrite(wheels, HIGH);
    Serial.print("Wheels on ");
  }
  else {
    digitalWrite(wheels, LOW);
    Serial.print("Wheels off ");
  }
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100); 
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
