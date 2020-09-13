# 2019BasicEngineeringDesign
2019 2학기(9월~12월) <기초설계> 최종 프로젝트

  - /study 에는 설계주차 이전, 아두이노 공부를 한창 하던 때에 재밌는 거 해보겠답시고 만들었던 간단한 오락거리들이다. 일단은 반응속도 게임만 올렸는데 나머지도 찾는대로 올리겠음. 동영상도 다 찍어놨는데 이건 어케 올리는 지를 모르겠네;

  - pdf 파일은 내가 제작했던 발표 파일들이다. 당시 팀원들에게 동의를 구하지 않고 뒤늦게야 깃헙에 정리하는 것이라 내가 만든 것만 올렸고, 최종 발표 파일은 올리지 않았다.

## 개요
**Arduino를 이용한 스마트 푸드코트 시스템**

 2019년 전자정보통신공학과 2학년 기초설계 수업에서 만든 아두이노 프로젝트이다.
 아이디어도 호평을 받았고, 시연회에서도 오류 없이 동작해 좋은 성과를 거뒀었다.

## 아이디어
 본 프로젝트를 간단히 요약하면 **푸드코트에서 음식을 완성한 후 고객을 호출하는 과정을 자동화한 장치**다.

푸드 코트에서 혼밥하다가 조리사들이 조리 완료 후 고객을 호출하기 위해 번호 패드를 누르는 걸 보게 되었는데,
'주방은 위생 관리가 철저할 텐데, 저렇게 음식 만들던 손으로 동일한 전자기기를 계속 만져도 위생에 괜찮은 걸까?' 라는 의문이 들어 시작한 주제다.

<center><img src="https://i.ibb.co/NpfbB2V/Kakao-Talk-20191219-115746665.jpg" width="300" height="300"></center>

^ 기존 호출기의 모습

 푸드 코트 내 키오스크와 LED 전광판과의 통신은 단기간에 완성하기가 어렵고, 테스트하기에도 환경이 따라주지 않아 휴대폰 어플리케이션을 만들어 시뮬레이션에 사용하였다.
제품과 통신하는 휴대폰에서는 주문을 생성하고, 조리가 완료된 주문에 대해 주문 번호를 전광판에 띄우는 작업을 하였다.

## 프로젝트 상세
 본 장치는 푸드코트의 쟁반을 놓을 위치를 사전에 정해 설치해야 한다. 주문이 들어올 시 장치는 쟁반 중 빈 곳을 찾아 주문을 할당한다.
해당 자리에 음식이 올려진 것을 감지하면, 고객을 호출하는 전광판에 신호를 보낸다.

 데이터 활용에 대한 피드백을 받아, 주문 시간부터 음식이 나오기까지 시간이 얼마나 걸렸는지를 엑셀로 기록하는 부분도 만들었다. 이 데이터는 원래 '메뉴별 예상 조리 시간'을 예측하기 위한 기반이 될 예정이었으나, 설계 일정이 예기치 않게 늘어져 그 부분까지는 구현하지 못했다.

 소프트웨어의 흐름은 다음과 같다.

![흐름도](https://i.ibb.co/KVfpjSv/project2019.png)

### 어려웠던 점
**1. 핀 수 맞추기, 배선**
  - 각 쟁반의 마다 LCD와 3digit 7segment, 조도 센서를 하나씩 배정해야 했기 때문에 아두이노의 핀이 너무 부족하게 느껴졌다.
  - LCD 장치의 핀 수를 줄이기 위해 1:N 시리얼 통신(이쪽은 내 파트가 아니어서 기억은 안 나지만 아마 I2C)을 구현해야 했는데, 복잡해진 배선을 고정시키기도 어려웠고 전압도 안 맞고 디버깅 과정도 길어서 참 힘들었다.
  - 장치 수가 많다 보니 선이 많았다. 기본 키트 세 개에 있던 전선을 다 쓰고도 모자라서 더 사올 정도였다. 시연회 전 주 토요일 하루종일 몰딩을 해서 전선 정리에는 성공했는데 시연회 당일 아침에 빵판에 꽂힌 게 문제가 생겨서 보드를 태워먹고도 뭘 수정해야 할지 몰라 몇시간동안 발만 동동 굴렀다.
  
**2. 통신 시스템**
  - 원래는 시뮬레이션 프로그램을 어플리케이션보단 PC용 프로그램을 Python으로 짜서 유선통신이든 무선통신이든 시키려고 했다. 화면이 더 넓어 내가 원하는 디자인을 할 수 있을 것 같기 때문이었다. 그런데 관련 경험이 없기도 했고, Win10 환경에서 내가 원하는 통신 기능을 갖춘 라이브러리를 찾기가 너무 어려웠다.
  - 그냥 시리얼 통신으로 시뮬레이션을 떼우는 방법도 있었지만, 당시 강의 교수님께서 App Inventor를 이용해 간단한 블루투스 통신을 꼭 프로젝트에 포함하기를 강력히 권장하셨었다. 결국 다른 팀보다 훨씬 늦게 어플리케이션 개발에 착수했었다.
  - 결과적으론 단시간 내에 때깔 좋은 결과물을 만들어낸, 훌륭한 선택이었던 것 같다.
  
**3. 센서 선정**
  - 원래는 조도 센서가 아니라 압력 센서로 감지하고자 했었다. 그런데 쟁반의 밑면이 균질하지 않아 쟁반이 확실하게 올라와도 인식이 안 되는 일이 흔했다. 그래서 초음파 거리센서로 바꾸었는데, 초음파 센서는 다수의 기기가 근처에 있을 시 서로의 관측을 방해한다는 결점이 있었다. 아마 교수님의 피드백으로 최종적으론 조도 센서를 채택한 것 같다.
  - 설계 조건이 센서 두 가지를 사용하는 것이었는데, 우리 프로젝트는 조도 센서 한 가지만 사용해도 충분했기에 나머지 하나를 억지로 끼워맞춰 스토리텔링을 해야 했다. 거리 센서는 그렇게 선정하게 되었다.


### 임베디드 보드(아두이노) 요소
1. 블루투스(BLE) 통신 : [MIT App inventor 2](https://appinventor.mit.edu/)를 이용해 통신용 어플을 제작해 아두이노와 휴대폰 간에 데이터 전송을 하도록 하였음.
2. 센서 구성 : 조도 센서, 거리 센서(초음파)
3. 출력 장치 : LCD, 7 segment 각 3개씩.
3. 큐(자료구조) : 주문을 큐에 쌓아놓고 다수의 쟁반에 할당하기 위해 자료 구조를 구현해야 했다. (대단한 건 아닌데 그냥 코딩 잘한다고 칭찬 받아서 기분 좋았기 때문에 작성함ㅎㅎ..)

### 어플리케이션
푸드코트 내 상황을 시뮬레이션하기 위해 만든 어플리케이션이다.

<center><img src="https://i.ibb.co/Bnx11t1/image.jpg" width="300" height="500"></center>

 상단 절반은 주문번호가 뜨는 전광판 역할을 하고, 아래는 주문 받는 키오스크 역할을 한다.
 
 페어링하는 부분을 따로 빼서 만들면 구성 요소가 쓸데없이 많아지는 느낌이라서, 미리 아두이노의 MAC주소를 프로그램에 심어 놓고 원터치로 페어링할 수 있도록 만들었다.

### 3D 프린팅

 조도 센서 위로 쟁반이 매끄럽게 올라탈 수 있도록 경사진 보조물을 만들어야만 했다. (사실 원래 목적은 압력 센서의 감지를 돕기 위해서였는데, 센서가 교체되며 목적도 변경되었다.)
 그래서 Tinkercad 사이트를 이용해 간단하게 모델링하고, 전자정보통신공학과 과사무실 3D 프린터를 사용해 인쇄하였다.

<center><img src="https://i.ibb.co/nBhDNpD/IMG-1542.jpg" width="400" height="400"></center>
 
 위 사진은 여러 도안을 실험적으로 인쇄해본 것이다. 결국엔 가장 간단한 구조를 채택해 양산했다.

## 팀 구성 및 협업 방식
 팀 구성은 분반 내 모든 팀의 학번 평균이 비슷하도록 교수님께서 임의로 짜셨고, 나랑 동기랑 선배가 한 팀이었다.
 
 - 나는 아이디어 제공, 아두이노 프로그램 전체 기획, 큐를 비롯한 주문 관리 관련 함수, 어플 제작과 디자인(BLE 통신 포함), 각종 시각자료 제작을 맡았다.
  - 동기는 대외 활동 경험이 많던 친구라서 협업 툴을 많이 도입해줬고 발표자료 제작, 시연회 발표, 부품 주문, 7segment 부분의 코드를 맡았다.
  - 선배는 LCD 디스플레이와 I2C 통신, 긴급 재료 조달, 부품 테스트, 남은 주문 수를 카운트해 디스플레이에 표시하는 코드를 맡았다.

 프로젝트가 끝난지 1년 가까이 되어가는 지금도 너무 고마운 마음만 많이 드는 팀원들이다. 모임을 싫어하던 내가 '끝나면 회식하자'는 말을 입에 달고 다녔을 정도로...

 공유 작업 공간은 Dropbox를 이용하였다. 학습 주차에는 각자의 학습 결과물을 dropbox 디렉토리에 올렸고, 설계 시작하고부턴 설계 관련 자료를 함께 모아 쓸 수 있어서 좋았다. 자료가 한 데 모여있으니 최종발표 자료 준비를 하는 데에도 용이했다.

 공동 프로그래밍을 위한 협업 툴은 **<구름IDE>** 를 이용했다. 보통은 깃헙을 이용한다던데 나도 깃헙을 쓸 줄 몰랐고, 팀원들도 IT 계열 진로가 아니라서 계정조차 없었다.
구름 IDE의 경우 웹사이트 주소만 알아도 코딩에 참여할 수 있던 덕에 코드 동기화에 큰 문제를 겪지 않고 진행했다. 
소스 코드와 동일한 디렉토리에 작업 히스토리를 txt로 정리했었는데, 이 점을 교수님께서 좋게 보셨었다.
