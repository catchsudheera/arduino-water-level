#include <LiquidCrystal.h>

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

const int trigPinT1 = 9;
const int echoPinT1 = 10;

const int trigPinT2 = 11;
const int echoPinT2 = 12;

const int heightTank_1 = 78, bufferTank_1 = 30;
// Tank 1
// Hight from bottom to overflow pipe level            : 88.5 cm
// Hight from top of the tank to overflow pipe level   : 22 cm
// Sensor unit hight                                   : 3 cm
// Hight from ball-value's ball to overflow pipe level : 10.5 cm

const int heightTank_2 = 78, bufferTank_2 = 30;
// Tank 2
// Hight from bottom to overflow pipe level            : 88.5 cm
// Hight from top of the tank to overflow pipe level   : 22 cm
// Sensor unit hight                                   : 3 cm
// Hight from ball-value's ball to overflow pipe level : 10.5 cm

double durationT1, pastDataT1 = 0, pastAvgT1 = 0;
double durationT2, pastDataT2 = 0, pastAvgT2 = 0;
float fillPercTank_1, fillPercTank_2;
int distanceCmT1, distanceCmT2, loopCount = 0;

void setup() {
  lcd.begin(16, 2);
  pinMode(trigPinT1, OUTPUT);
  pinMode(echoPinT1, INPUT);
  pinMode(trigPinT2, OUTPUT);
  pinMode(echoPinT2, INPUT);
}

void loop() {
  loopCount++;
  // Tank 1
  digitalWrite(trigPinT1, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPinT1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinT1, LOW);

  durationT1 = pulseIn(echoPinT1, HIGH);
  distanceCmT1 = durationT1 * 0.0348 / 2;
  if (distanceCmT1 % 2 != 0) { // we need to lower the precision in order not to fluctuate the output percentage value
    distanceCmT1++;
  }
  fillPercTank_1 = ((heightTank_1 + bufferTank_1 - distanceCmT1) * 100.0 / (heightTank_1 * 1.0));

  // Limit the range 0-100
  if (fillPercTank_1 < 0) {
    fillPercTank_1 = 0;
  } else if (fillPercTank_1 > 100) {
    fillPercTank_1 = 100;
  }

  // Tank 2
  digitalWrite(trigPinT2, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPinT2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinT2, LOW);

  durationT2 = pulseIn(echoPinT2, HIGH);
  distanceCmT2 = durationT2 * 0.0348 / 2;
  if (distanceCmT2 % 2 != 0) { // we need to lower the precision in order not to fluctuate the output percentage value
    distanceCmT2++;
  }
  fillPercTank_2 = ((heightTank_2 + bufferTank_2 - distanceCmT2) * 100.0 / (heightTank_2 * 1.0));

// Limit the range 0-100
  if (fillPercTank_2 < 0) {
    fillPercTank_2 = 0;
  } else if (fillPercTank_2 > 100) {
    fillPercTank_2 = 100;
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

  pastDataT1 += distanceCmT1;
  pastAvgT1 = pastDataT1 / loopCount;

  if (pastAvgT1 > distanceCmT1) {
    lcd.print(" U");
  } else if (pastAvgT1 < distanceCmT1) {
    lcd.print(" D");
  } else {
    lcd.print(" -");
  }

  lcd.setCursor(0, 1);

  char displayStringT2[16] = "";
  sprintf(displayStringT2, "T2:%3d.%d", (int)fillPercTank_2, int(fillPercTank_2 * 10) % 10);
  lcd.print(displayStringT2);
  lcd.print("% ");

  if (fillPercTank_2 < 25) {
    lcd.print(" LOW");
  } else if (fillPercTank_2 < 70) {
    lcd.print(" MID");
  } else {
    lcd.print("HIGH");
  }

  pastDataT2 += distanceCmT2;
  pastAvgT2 = pastDataT2 / loopCount;

  if (pastAvgT2 > distanceCmT2) {
    lcd.print(" U");
  } else if (pastAvgT2 < distanceCmT2) {
    lcd.print(" D");
  } else {
    lcd.print(" -");
  }

  delay(1000); // No need to update the LCD in very high frequency
  if (loopCount >= 60) {
    loopCount = 0;
    pastDataT1 = 0;
    pastDataT2 = 0;
  }
}
