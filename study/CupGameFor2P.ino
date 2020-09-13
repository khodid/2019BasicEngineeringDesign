//  2P 반응속도 게임 [[[ Cup Game For 2P ]]]
//  빨강이 1P, 초록이 2P
//
//  게임 설명: 
//  부저가 울릴 때 상대방보다 먼저 종이컵을 뒤집으면 승리. 패배할 수록 당신의 LED 불빛은 꺼져간다.
//
//  주의사항:
//  새로운 환경에서 연결 시, 조도 기준 새로 세팅할 것.
//  --> void cupState() <--

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);

#define RND_MX 13
#define RND_MN 5
#define LIFE 3

const int p1 = 5; const int pCup1 = A0; int life1 = LIFE - 1; // player 1 data
const int p2 = 6; const int pCup2 = A1; int life2 = LIFE - 1; // player 2 data

int rd = 0, ar = 0; // rd is for random(), ar is for analogRead()
byte state = 0;
byte gamemode = 0;

int level[LIFE] = {0};

void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT); // Piezo Buzzer
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  randomSeed( analogRead(A5) ); // A5 OUTPPUT: random integer [0,1023]
  for (int i = 0; i < LIFE; i++) level[i] = map(i, 0, LIFE - 1, 0, 255);
}

void loop() {
  blnk();
  if (!gamemode) {
    printStatus();
    randSignal();
  }
  else {
    /* Game over. Need to Reset */
    Serial.println("**************************************");
    gamemode = 0;
    life1 = LIFE - 1;
    life2 = LIFE - 1;
    blnk();
  }
}

void randSignal() {
  rd = random( RND_MN , RND_MX + 1 ) ; // rd gets integer [randMin, randMax]
  delay(rd * 750);
  /* Random Beep Signal */
  tone(8, 500);
  /* Cheat Test */
  cupState();
  if (state) {
    cheat();
    return;
  }
  for (int i = 0; i < 300; i++) {
    delay(5) ;
    cupState();
  }
  noTone(8);
  Serial.println("Player " + (String) state + " WON !!!");

  switch (state) {
    case 1: damage2(); state = 0; break;
    case 2: damage1(); state = 0; break;
  }
}

/* 새로운 환경에서 연결 시, 조도 기준 새로 세팅 할 것. */
void cupState() {
  ar = analogRead(A0);
  if (ar < 230 && !state ) state = 1;
  ar = analogRead(A1);
  if (ar < 230 && !state) state = 2;
}

/* Blink both LED twice. */
void blnk() {
  analogWrite(p1, 0);
  analogWrite(p2, 0);
  delay(400);
  analogWrite(p1, level[life1]);
  analogWrite(p2, level[life2]);
  delay(400);
  analogWrite(p1, 0);
  analogWrite(p2, 0);
  delay(400);
  analogWrite(p1, level[life1]);
  analogWrite(p2, level[life2]);
}

void cheat() {
  Serial.println("Player " + (String)state + " has cheated. Restart The Round. \n");
  noTone(8);
  if (state == 1) damage1();
  else damage2();
  state = 0;
}

void damage1() {
  if (life1 > 0) life1 --;
  else gamemode = 1;
  analogWrite(p1, level[life1]);
}

void damage2() {
  if (life2 > 0) life2 --;
  else gamemode = 1;
  analogWrite(p2, level[life2]);
}

void printStatus() {
  Serial.println("Player 1 life : " + (String) (life1 + 1) );
  Serial.println("Player 2 life : " + (String) (life2 + 1) + "\n");
}
