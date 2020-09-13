//   함수의 정의를 저장하는 곳입니다.
//   setup과 loop, queue 라이브러리를 제외한 모든 함수는 여기에 써주세요.

#include<Arduino.h>
#include <LiquidCrystal_I2C.h> 

#define NUMBER_OF_TRAY 4

#ifndef INVALID
#define INVALID 255
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define ONTRAY 10     // 실제 값에 따라 조정 필요


unsigned long pre_time[NUMBER_OF_TRAY]= {0};
unsigned long cur_time[NUMBER_OF_TRAY] = {0};
boolean trayCount[NUMBER_OF_TRAY] = {FALSE};

void moveTrayPointer()
{
  byte ctr = 0;
  if(trayOrderState[trayPointer] != no_order){
    while(ctr++<4)
    {
      trayPointer++;
      if (trayOrderState[trayPointer] == no_order) return;  
    }
    trayPointer = INVALID;
  }
  else return;
}

void queueToTray()
{
  orderNum[trayPointer] = peekQueue_orderNum();
  orderTime[trayPointer] = peekQueue_orderTime();
  trayOrderState[trayPointer] = waiting_food;
  deleteQueue();
  moveTrayPointer();
  
  // 쟁반 디스플레이 관련 조작
}

void deleteOrder(int trayIndex)
{
  orderNum[trayIndex] = -1;
  orderTime[trayIndex] = 0;
  trayOrderState[trayIndex] = no_order;
  moveTrayPointer();
  // 해당 쟁반의 LCD 디스플레이 처리
  // 해당 쟁반의 7seg 디스플레이 처리
  
}

void trayUpdate()
{ 
  int v;
  for(int i=0; i<NUMBER_OF_TRAY; i++)
  {
    v = analogRead(pcell[i]);
    if(isSomethingOnTray[i] == FALSE)
    {
      if(v > ONTRAY && trayCount[i] == FALSE)
      {
        pre_time[i]=millis();
        trayCount[i]=TRUE;
      }
      else if(trayCount[i]==TRUE)
        
      {
        cur_time[i] = millis();
        if(v <= ONTRAY)
          trayCount[i] = FALSE;
        if(cur_time[i] - pre_time[i] >= 3000)
        {
          isSomethingOnTray[i] = TRUE;
          trayCount[i] = FALSE;
        }
      }
    }// 압력이 가해진 후 3초 대기 후 isSomethingOnTray 실행, 그전에 압력이 없어지면 초기화 
    else if(isSomethingOnTray[i] == TRUE)
    {
      if(v <= ONTRAY && trayCount[i] == FALSE)
      {
        pre_time[i]=millis();
        trayCount[i]=TRUE;
      }
      else if(trayCount[i]==TRUE)
      {
        cur_time[i] = millis();
        if(v > ONTRAY) 
          trayCount[i] = FALSE;
        if(cur_time[i] - pre_time[i] >= 1000)
        {
          isSomethingOnTray[i] = FALSE;
          trayCount[i] = FALSE;
        }
      }
    }// 고객대기시 압력이 사라질때도 1초간의 여유를 줌. 다른음식 올려놓다가 실수로 건드릴때를 대비
    
  }
/* 
  쟁반에서 모든 압력 센서의 값을 읽어옴 -> 압력 센서의 상태에 따라 isSomethingOnTray[해당 쟁반번호] = TRUE; 를 실행하여 쟁반 상태 변수를 업데이트해줌.
  trayOrderState를 건드리지 말 것! 아마 isSomethingOnTray 변수를 바꾸어 주는 것만으로도 충분할 것임.
                                                                              
*/  
}

void customDis(int numberOfDisplay){
  
  // 첫번째 lcd 설정
  
  if(trayOrderState[numberOfDisplay] == waiting_food){
    // LCD
    setNumber( orderNum[numberOfDisplay]/100    , 0 , numberOfDisplay);
    setNumber( (orderNum[numberOfDisplay]%100)/10   , 3 , numberOfDisplay);
    setNumber( orderNum[numberOfDisplay]%10     , 6 , numberOfDisplay);
    // 7segment
    segment[numberOfDisplay].showNumberDec(orderNum[numberOfDisplay]);
  }
  else if(trayOrderState[numberOfDisplay] == waiting_customer){
    // LCD
    lcd[numberOfDisplay].clear();
    // 7segment
    segment[numberOfDisplay].showNumberDec(00);
    
  }
  

}
void setNumber (int displayNum,int position, int i){
  
  if(displayNum==0){
      lcd[i].setCursor(position,0); lcd[i].write((byte)0);  lcd[i].write(1);  lcd[i].write(2);  
    lcd[i].setCursor(position, 1);  lcd[i].write(3);    lcd[i].write(4);  lcd[i].write(5);
    }
    else if(displayNum==1){
    lcd[i].setCursor(position,0); lcd[i].write(1);    lcd[i].write(2);  lcd[i].print(" ");
      lcd[i].setCursor(position,1); lcd[i].write(4);    lcd[i].write(7);  lcd[i].write(4);
  }
    else if(displayNum==2){
      lcd[i].setCursor(position,0); lcd[i].write(6);    lcd[i].write(6);  lcd[i].write(2);
      lcd[i].setCursor(position, 1);  lcd[i].write(3);    lcd[i].write(4);  lcd[i].write(4);
    }
    else if(displayNum==3){
      lcd[i].setCursor(position,0); lcd[i].write(6);    lcd[i].write(6);  lcd[i].write(2);
    lcd[i].setCursor(position, 1);  lcd[i].write(4);    lcd[i].write(4);  lcd[i].write(5);
  }
    else if(displayNum==4){
      lcd[i].setCursor(position,0);  lcd[i].write(3);     lcd[i].write(4);  lcd[i].write(7);
      lcd[i].setCursor(position, 1);  lcd[i].print(" ");    lcd[i].print(" ");  lcd[i].write(7);
    }
    else if(displayNum==5){
      lcd[i].setCursor(position,0); lcd[i].write(3);      lcd[i].write(6);  lcd[i].write(6);
      lcd[i].setCursor(position, 1);  lcd[i].write(4);    lcd[i].write(4);  lcd[i].write(5);
    }
    else if(displayNum==6){
      lcd[i].setCursor(position,0); lcd[i].write((byte)0);  lcd[i].write(6);  lcd[i].write(6);
      lcd[i].setCursor(position, 1);  lcd[i].write(3);      lcd[i].write(4);  lcd[i].write(5);
    }
    else if(displayNum==7){
      lcd[i].setCursor(position,0); lcd[i].write(1);      lcd[i].write(1);  lcd[i].write(2);
      lcd[i].setCursor(position, 1);  lcd[i].print(" ");    lcd[i].print(" ");  lcd[i].write(7);
    }
   else if(displayNum==8){
      lcd[i].setCursor(position,0); lcd[i].write((byte)0);  lcd[i].write(6);  lcd[i].write(2);
      lcd[i].setCursor(position, 1);  lcd[i].write(3);    lcd[i].write(4);  lcd[i].write(5);
    }
  else if(displayNum==9){
      lcd[i].setCursor(position,0); lcd[i].write((byte)0);  lcd[i].write(6);    lcd[i].write(2);
      lcd[i].setCursor(position, 1);  lcd[i].print(" ");    lcd[i].print(" ");    lcd[i].write(7);
    }
}
