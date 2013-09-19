const int leftmotorpin1 = A0; //4; // signal input of Dc motor driven plate
const int leftmotorpin2 = A1; //5;
const int rightmotorpin1 = A2;//7;
const int rightmotorpin2 = A3; //6;
const int sonarTX = 10;
const int sonarRX = 10;
const float closest = 24.0;

int tooClose = 0;
long lastRange = 0;

void setup() {
  Serial.begin(9600);
  pinMode(leftmotorpin1, OUTPUT);
  pinMode(leftmotorpin2, OUTPUT);
  pinMode(rightmotorpin1, OUTPUT);
  pinMode(rightmotorpin2, OUTPUT);
  pinMode(sonarTX, OUTPUT);
  pinMode(sonarRX, INPUT);
}

void loop() {
  long duration, inches, cm;
  pinMode(sonarRX, OUTPUT);
  digitalWrite(sonarRX, LOW);
  delayMicroseconds(2);
  digitalWrite(sonarRX, HIGH);
  delayMicroseconds(5);
  digitalWrite(sonarRX, LOW);
  
  pinMode(sonarRX, INPUT);
  duration = pulseIn(sonarRX, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  if (inches <= closest && inches != 0.00) {
    tooClose++;
  }
  else {
    tooClose = 0;
  }
  
  if (tooClose < 1) {
    forward();
    Serial.print("Wheels on ");
  }
  else {
    backward();
    delay(200);
    backLeft();
    delay(2000);
    forward();
    delay(200);
    right();
    delay(2000);
    tooClose = 0;
    Serial.print("Wheels off ");
  }
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(500);
}

void backward() {
  analogWrite(leftmotorpin1, 255);
  analogWrite(leftmotorpin2, 0);
  analogWrite(rightmotorpin1, 0);
  analogWrite(rightmotorpin2, 255);
}

void backLeft() {
  analogWrite(leftmotorpin1, 60);//LOW);
  analogWrite(leftmotorpin2, 0);//HIGH);
  analogWrite(rightmotorpin1, 0);
  analogWrite(rightmotorpin2, 255);
}

void forward() {
  analogWrite(leftmotorpin1, 0);//LOW);
  analogWrite(leftmotorpin2, 255);//HIGH);
  analogWrite(rightmotorpin1, 255);//LOW);
  analogWrite(rightmotorpin2, 0);//HIGH);
}

void left() {
  analogWrite(leftmotorpin1, 255);//HIGH);
  analogWrite(leftmotorpin2, 0);//LOW);
  analogWrite(rightmotorpin1, 0);//LOW);
  analogWrite(rightmotorpin2, 60);//HIGH);
}

void right() {
  analogWrite(leftmotorpin1, 0);//LOW);
  analogWrite(leftmotorpin2, 255);//HIGH);
  analogWrite(rightmotorpin1, 60);
  analogWrite(rightmotorpin2, 0);
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
