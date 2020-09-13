/*
    !!! 메인 알고리즘과 각종 선언을 담은 공간입니다. 
    !!! setup과 loop를 제외한 함수는 이 곳에 '선언'하되, 정의는 function_def 쪽 파일에 써주세요.
*/

#include<Arduino.h>
#include<SoftwareSerial.h>
#include <TM1637Display.h>        // for 7 segment
#include <LiquidCrystal_I2C.h>      // for lcd display
#include <Wire.h>

#define QUE_LENGTH 40
#define NUMBER_OF_TRAY 4
#define INVALID 255  
// 설명: byte형 제어 변수에 '유효하지 않은 상태'라는 표시를 하고 싶으면 이 수를 대입합니다. (예 - trayPointer 변수에 '빈 쟁반이 존재하지 않는다'는 걸 기록하고 싶을 때.)
#define TRUE 1
#define FALSE 0

#define CLK 2
#define DIO 3

#define CLK 7
#define DIO 6

#define CLK 8
#define DIO 9

#define CLK 10
#define DIO 11

TM1637Display dsp(CLK, DIO);

enum orderState {
  no_order,
  waiting_food,
  waiting_customer
};

typedef struct _que 
{
  int orderNum[QUE_LENGTH];
  unsigned long orderTime[QUE_LENGTH];
  byte front;   // 다음 append시 데이터를 넣을 공간을 가리킴. (front가 가리키는 위치는 비어있음)
  byte rear;    // 다음 delete시 삭제할 데이터를 가리킴. (rear는 마지막 데이터를 가리킴, f==r인 경우 제외)
} queue;

uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };  // all segments clear
long d;
long lcd_num;
long state;
long num;


/*  변수  선언  */

//       1. 입출력 관련 변수
const uint8_t pcell[NUMBER_OF_TRAY] = {A1, A2, A3, A4};
SoftwareSerial BTSerial(4,5);

TM1637Display segment[NUMBER_OF_TRAY] = 
{
  // TM1637Display(CLK, DIO)
  // 이전에 썼던 코드가 5번을 사용하는데, 디지털 4-5번은 블루투스 통신 때문에 사용하지 못함. 다른 걸로 교체해야 함.  
  // *** 오브젝트 선언 도와주세요! - 위에 오브젝트 생성하고 초기화 되도록 해두었습니다
  // dio가 pwm 핀쓰는것 때문에 6,7 순서바꿨습니다
  TM1637Display(2,3),
  TM1637Display(7,6),
  TM1637Display(8,9),
  TM1637Display(10,11),
};

LiquidCrystal_I2C lcd[NUMBER_OF_TRAY] =
{
  LiquidCrystal_I2C(0x27, 16, 2),
  LiquidCrystal_I2C(0x26, 16, 2),
  LiquidCrystal_I2C(0x25, 16, 2),
  LiquidCrystal_I2C(0x23, 16, 2)  
};


//       2. 제어 관련 변수
byte trayPointer = 0;
unsigned long orderTime[NUMBER_OF_TRAY] = {0};
orderState trayOrderState[NUMBER_OF_TRAY] = {no_order};
boolean isSomethingOnTray[NUMBER_OF_TRAY] = {FALSE};
int orderNum[NUMBER_OF_TRAY] = {-1, -1, -1, -1}; // 

byte LT[8] = { B00111, B01111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte UB[8] = { B11111, B11111, B11111, B00000, B00000, B00000, B00000, B00000 };
byte RT[8] = { B11100, B11110, B11111, B11111, B11111, B11111, B11111, B11111 };
byte LL[8] = { B11111, B11111, B11111, B11111, B11111, B11111, B01111, B00111 };
byte LB[8] = { B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111 };
byte LR[8] = { B11111, B11111, B11111, B11111, B11111, B11111, B11110, B11100 };
byte MB[8] = { B11111, B11111, B11111, B00000, B00000, B00000, B11111, B11111 };
byte block[8] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 }; // lcd 모양 지정

queue Q;


/*  함수  선언  */

void moveTrayPointer();     /* 빈 쟁반을 가리키는 포인터 업데이트 */
void queueToTray();       /* 큐의 가장 오래된 데이터를 빈 쟁반에 할당함 */
void deleteOrder(int);      /* 쟁반의 index를 입력받아 제어 변수 처리, cookDis와 customerDis 출력.*/
void trayUpdate();


//        Queue library 
void initQueue();                   /* 큐에 초기화 값을 넣어줍니다. */

int peekQueue_orderNum();               /* 큐에 남아있는 데이터 중 가장 오래된 데이터(즉 다음 인출 시 나올 값) 중 주문번호를 반환합니다. */
unsigned long peekQueue_orderTime();          /* 큐에 남아있는 데이터 중 가장 오래된 데이터(즉 다음 인출 시 나올 값) 중 주문시간을 반환합니다. */ 

void appendQueue(int ordernum, unsigned long ordertime);/* 큐에 새로운 값을 추가합니다. 주문번호와 주문시간을 입력인자로 주어야 합니다. */
void deleteQueue();                   /* 큐에 남아있는 데이터 중 가장 오래된 데이터를 삭제합니다. */

void printQueue();                    /* 큐 내의 전체 데이터를 한 번씩 Serial Monitor로 출력합니다. */
boolean isQueueEmpty();                 /* 큐가 비어있다면 TRUE, 비어있지 않다면 FALSE를 반환합니다. */




/* 메인 알고리즘 */

//        SETUP : 시작하고 한 번만 실행
void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  initQueue();
  
  for(int i=0;i<NUMBER_OF_TRAY;i++){
  lcd[i].init();
  lcd[i].backlight();
  lcd[i].createChar(0,LT);
    lcd[i].createChar(1,UB);
    lcd[i].createChar(2,RT);
    lcd[i].createChar(3,LL);
    lcd[i].createChar(4,LB);
    lcd[i].createChar(5,LR);
    lcd[i].createChar(6,MB);
    lcd[i].createChar(7,block);
    
  segment[i].setBrightness(0x0f);
  segment[i].setSegments(data);
  segment[i].showNumberDec(00);
  }
  
  
  Serial.println("프로그램을 시작할 준비가 되었습니다.");
  BTSerial.print("[" + (String)1 + "!" +"SP" + "]");
}
//        LOOP : 전원이 꺼질 때까지 반복실행

void loop(){
  int num = 0;
  // 작업1 : 컴퓨터에서 주문 정보 받기 
  if(BTSerial.available())
  {
    num = BTSerial.parseInt();
    if(num) 
    {
      appendQueue(num,millis());   // num = =0 인 출력 무시
      if(trayPointer != INVALID)
      {
        queueToTray();
      
      }
    }
  }
  
  // 작업 2 : 주문을 받은 쟁반에 음식이 준비되면 고객을 호출함
  for(int i=0; i<NUMBER_OF_TRAY ; i++)
  {
    if(trayOrderState[i] == waiting_food && isSomethingOnTray[i] == TRUE)
    {
      BTSerial.print("[" + (String)orderNum[i] + "!" +"WC" + "]");
      Serial.println("SYSTEM: " + (String)orderNum[i] + "번 고객님, 음식이 준비되었습니다!");
      trayOrderState[i] = waiting_customer;
      customDis(i); //디스플레이
    }
  }
  
  // 작업 3 : 주문을 받고 음식이 올라온 쟁반에서 고객이 음식을 가져가면 주문을 삭제함
  for(int i=0; i<NUMBER_OF_TRAY ; i++)
  {
    if(trayOrderState[i] == waiting_customer && isSomethingOnTray[i] == FALSE)
    {
      BTSerial.print("[" + (String)orderNum[i] + "!" +"DL" + "]");
      Serial.println("SYSTEM: " + (String)(i+1) + "번 쟁반에서 고객이 음식을 가져갔습니다.");
      deleteOrder(i);
      
      customDis(i); //디스플레이
    }
  }
  
  // 작업 4 : 주문 큐에서 빈 쟁반에 주문 할당. ~ 빈 쟁반이 더이상 없거나 주문 큐가 빌 때까지 털어넣기.
  while( trayPointer != INVALID && !isQueueEmpty()) 
  {
    queueToTray();
  }
  
  // 작업 5 : 실시간 쟁반의 상태(isSomethingOnTray) 업데이트
  trayUpdate();
}
