#include <SoftwareSerial.h>
#include "EV3.h"

SoftwareSerial bt(10, 11); //(RX, TX)
EV3 ev3;

String MESSAGE_NAME = "sup"; //Message name must be the same as the Message block inside the EV3 code
String MESSAGE_STRING = "dick";
float MESSAGE_NUM = 341.54;
bool MESSAGE_BOOL = true;

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
}

void loop() {
  ev3.sendMailbox(bt, MESSAGE_NAME, MESSAGE_STRING);
  delay(4000);
}
