//  큐와 관련된 라이브러리를 구현한 공간입니다. Queue가 main 파일의 주문 대기열 'Q'만 존재한다고 가정하고 만든 라이브러리입니다. 여러 큐를 사용해야 한다면 수정해야 합니다.
//  큐(Queue; 대기열)는 먼저 들어간 데이터가 먼저 나오는 형식의 자료구조입니다. 쉽게 말하면 줄 서는 것과 똑같습니다.
//  연결리스트로 할 예정이었으나 아두이노 개발에선 메모리 관리의 불안정성 문제 때문에 연결리스트를 거의 사용하지 않는 듯 해서 순차리스트(배열)로 바꿨습니다.


#include<Arduino.h>
 // 주문 대기열의 최대 길이

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif


/* 
    !!! 사용되는 함수들 - 사용자는 각 함수의 입력값과 반환값만 익혀서 사용할 것 !!!
*/







/*  
    !!! 함수 정의 - 사용자는 이 아래로는 참고하지 않아도 됨 !!!
*/



void initQueue()
{
  for(int i=0; i<QUE_LENGTH; i++) Q.orderNum[i] =-1;
  for(int i=0; i<QUE_LENGTH; i++) Q.orderTime[i] = 0;
  Q.front=0;
  Q.rear=0;
}

int peekQueue_orderNum()
{
  if(isQueueEmpty()) Serial.println("!!! ERROR: 주문 대기열이 비어 있습니다.");
  else return Q.orderNum[Q.rear];
}
unsigned long peekQueue_orderTime()
{
  if(isQueueEmpty()) Serial.println("!!! ERROR: 주문 대기열이 비어 있습니다.");
  else return Q.orderTime[Q.rear];
}

void appendQueue(int ordernum, unsigned long ordertime)
{
  if((Q.rear - Q.front) % QUE_LENGTH == 1)
  {
    Serial.println("!!! ERROR: 주문 대기열에 더이상 공간이 없습니다. 소스파일 컴파일 시 QUE_LENGTH를 조정하세요.");
  }
  else 
  {
    Q.orderNum[Q.front] = ordernum;
    Q.orderTime[Q.front] = ordertime;
    Q.front = (Q.front+1) % QUE_LENGTH;
  }
}

void deleteQueue()
{
  if(isQueueEmpty()) Serial.println("!!! ERROR: 비어 있는 대기열입니다. 삭제할 데이터가 없습니다.");
  else 
  {
    Q.rear = (Q.rear + 1) % QUE_LENGTH;
  }
}

void printQueue()
{
  byte ptr = Q.rear;
  if(isQueueEmpty()) 
  {
    Serial.println("대기열은 비어 있습니다."); 
    return;
  }
  while(ptr != Q.front)
  {
    Serial.print(Q.orderNum[ptr]); Serial.println("");Serial.print(Q.orderTime[ptr]); Serial.println("\n");
    ptr = (ptr + 1) % QUE_LENGTH;
  }
}

boolean isQueueEmpty(){
  return (Q.front == Q.rear)? (TRUE) : (FALSE);
}
