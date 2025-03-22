 #include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Fingerprint.h>

#define buzzer 4
#define servoPin 5
#define doorLock 80
#define doorOpen 170
Servo servo;

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C dis(0x27, 16, 2);

byte ID = 2; // Enter your fingerprint ID
bool Switch = true;

void setup() {
  Serial.begin(57600);
  servo.attach(servoPin);
  servo.write(doorLock);
  dis.init();
  dis.backlight();
  pinMode(buzzer, OUTPUT);
  while (!Serial); // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getParameters();
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

void loop() {
  int value = getFingerprintIDez();
  dis.setCursor(1, 0);
  dis.print("Place your");
  dis.setCursor(7, 1);
  dis.print("finger->");

  if (value == ID) {
    if (Switch) {
      dis.clear();
      dis.setCursor(3, 0);
      dis.print("Successful");
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      servo.write(doorLock);
      dis.clear();
      Switch = false;
    } else if (Switch == false) {
      dis.clear();
      dis.setCursor(3, 0);
      dis.print("Successful");
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      servo.write(doorOpen);
      dis.clear();
      Switch = true;
    }
  } else if (value == -2) {
    dis.clear();
    dis.setCursor(3, 0);
    dis.print("Try again!");
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
        dis.clear();
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -2;

  return finger.fingerID;
}
