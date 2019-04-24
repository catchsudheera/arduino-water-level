#include <LiquidCrystal.h>

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

const int trigPin = 9;
const int echoPin = 10;
const int heightTank_1 = 78, bufferTank_1 = 30;
// Tank 1
// Hight from bottom to overflow pipe level            : 88.5 cm
// Hight from top of the tank to overflow pipe level   : 22 cm
// Sensor unit hight                                   : 3 cm
// Hight from ball-value's ball to overflow pipe level : 10.5 cm

double duration, pastData = 0, pastAvg = 0;
float fillPercTank_1;
int distanceCm, loopCount = 0;

void setup() {
  lcd.begin(16, 2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  loopCount++;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.0348 / 2;
  if (distanceCm % 2 != 0) { // we need to lower the precision in order not to fluctuate the output percentage value
    distanceCm++;
  }
  fillPercTank_1 = ((heightTank_1 + bufferTank_1 - distanceCm) * 100.0 / (heightTank_1 * 1.0));

  if (fillPercTank_1 < 0) {
    fillPercTank_1 = 0;
  } else if (fillPercTank_1 > 100) {
    fillPercTank_1 = 100;
  }

  lcd.clear();
  lcd.setCursor(0, 0);

  char displayString[16] = "";
  sprintf(displayString, "T1:%3d.%d", (int)fillPercTank_1, int(fillPercTank_1 * 10) % 10);
  lcd.print(displayString);
  lcd.print("% ");

  if (fillPercTank_1 < 25) {
    lcd.print(" LOW");
  } else if (fillPercTank_1 < 70) {
    lcd.print(" MID");
  } else {
    lcd.print("HIGH");
  }
  
  pastData += distanceCm;
  pastAvg = pastData / loopCount;

  if (pastAvg > distanceCm) {
    lcd.print(" D");
  } else if (pastAvg < distanceCm) {
    lcd.print(" U");
  }

  lcd.setCursor(0, 1);
  lcd.print(distanceCm);
  lcd.print(" ");
  lcd.print(pastAvg);
  
  delay(1000); // No need to update the LCD in very high frequency
  if (loopCount >= 60) {
    loopCount = 0;
    pastData = 0;
  }
}
