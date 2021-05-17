#include<Servo.h>
#include<limits.h>

Servo sv;

unsigned long bpm = 0, t = 0, beat = ULONG_MAX;
byte cnt = 0;


void setup() {
  sv.attach(11);
  Serial.begin(9600);
  Serial.println("Servo Start.");
}

void loop() {
  t = millis();
  if (Serial.available()) {
    bpm = (unsigned long)Serial.parseInt();
    beat = 60000 / bpm;
  }
  if (!(t % beat)) {
    cnt = moveIt();
    Serial.println("Move It!" + (String)t);
  };
}
byte moveIt() {
  (cnt) ? (sv.write(0)) : (sv.write(45));
  return (byte)((cnt + 1) % 2);
}
