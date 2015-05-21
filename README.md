Game Server Programming, Step by Step
===================

NHN NEXT 게임 서버 프로그래밍 학습을 위한 과제 위주의 코드입니다. 이 구현 과제를 끝까지 마치면 게임 서버 기반 프레임워크가 나옵니다. 교육용으로 제작된만큼 코드를 최대한 읽고 고치기 쉽게 작성되었고(Black magic은 사용하지 않았습니다), 학생들의 문제 해결 능력 향상을 위해(?) 버그도 심어놓았습니다.

(주의: 이 기반 코드를 서비스용으로 사용하기 위해서는 각종 방어코드, 모듈화 및 예외처리 등을 강화해야 합니다.)

## NHN NEXT의 게임 서버 프로그래밍 과목은
* 게임 서버 기반 프레임워크를 제작
  - Stateful한 게임 서버 (Stateless한 웹서버는 다루지 않음)
  - 거시적인(?) 서버간 아키텍처(scale-out 등)에 관한 부분은 다른 과목에서 다룸
* 필요한 선행 지식(과목)
  - 운영체제 및 시스템 프로그래밍 실습 (Windows)
  - 컴퓨터 네트워크 및 소켓 프로그래밍 실습
  - 컴퓨터 시스템 아키텍처
  - 데이터베이스
  
## 학습목표

1. 게임 서버의 기반 코드를 직접 설계하고 구현
  - 멀티스레드, IOCP, 메모리 관리, DB/SQL, 패킷 처리, …
2. 게임 서버의 성능을 평가하고 개선
  - Dummy Client 제작, Profiling, …
3. 게임 서버가 죽었을(?) 때, 이를 분석하고 수정
 - Crash dump 분석, Logging, 디버깅 힌트 구성, …


## 이 과정에서 배울 수 있는 것

1. IOCP를 사용한 고성능 게임 서버 제작 전반
   - Asynchronous non-blocking 방식의 네트워크 입출력 및 다수의 클라이언트 세션 처리
   - TCP 스트림을 다루는 방법 및 TCP 스트림을 통한 패킷 직렬화/역직렬화
   - 비동기 방식의 접속 처리(AcceptEx/DisconnectEx) 및 CPU 사용률을 줄이기 위한 기법들(zero-byte recv, aggregated send)
   - 서버간 연결 기능 (ConnectEx) 및 소켓 풀링
2. Lock-free 메모리 풀링 기법
   - Memory Pooling
   - Object Pooling
   - Custom STL Allocator 
3. 고성능을 위한 멀티스레딩 기법
   - Lock-hierarchy를 이용한 Deadlock 감지 기법
   - Thread Local Storage 활용 기법
   - Lock-free 알고리즘을 적용한 dispatcher작성 방법
   - 스레드 효율적인 타이머 구현 방법
4. Database 연동 방법
   - SQL 서버와의 연동 및 stored procedure를 이용한 데이터 로직 처리 방법
   - ODBC를 wrapping하여 DB Query를 처리하는 방법 (DBHelper)
   - DB 로직 전용 스레드풀을 구성하여 I/O 스레드풀과 분리하여 처리하는 방법 (Half sync Half async 패턴)
   - Dababase context를 다루는 방법
5. Logging, 디버깅 정보 구성, 예외 처리
   - SEH의 Exception filter를 통한 예외 처리 및 minidump 남기는 방법
   - Tracing을 위한 call history 및 call stack을 남기는 방법
   - 스레드 안전한 로그를 남기는 방법
   - 함수 단위 성능 체크 방법
6. 패킷 핸들링 및 암호화
   - 구글 protobuf를 패킷으로 활용하는 방법
   - 매크로를 이용하여 패킷을 등록하고 dispatching하는 방법
   - 암호화를 위한 키 교환 및 서버-클라이언트간스트림 암호화를 적용하는 방법
7. 기타
   - 포인터의 생명주기 관리 방법 (reference counting)
   - 더미 클라이언트를 활용한 시나리오 기반의 성능 테스트 방법 (DummyClients 프로젝트)
   - 크래시 덤프 (crash dump) 분석


## 이 과정에서 사용된 강의 슬라이드

* [게임서버프로그래밍 #0 - TCP 및 이벤트 통지모델](http://www.slideshare.net/sm9kr/gsp-0-tcpio)
* [게임서버프로그래밍 #1 - IOCP](http://www.slideshare.net/sm9kr/gsp-1-iocp)
* [게임서버프로그래밍 #2 - IOCP Advanced](http://www.slideshare.net/sm9kr/gsp-2-iocp)
* [게임서버프로그래밍 #3 - 메모리 및 오브젝트 풀링](http://www.slideshare.net/sm9kr/gsp-3-pooling)
* [게임서버프로그래밍 #4 - 멀티스레드 프로그래밍](http://www.slideshare.net/sm9kr/gsp-4-multithread)
* [게임서버프로그래밍 #5 - 데이터베이스 핸들링](http://www.slideshare.net/sm9kr/gsp-5-database)
* [게임서버프로그래밍 #6 - 예외처리 및 로깅](http://www.slideshare.net/sm9kr/gsp-6)
* [게임서버프로그래밍 #7 - 패킷핸들링 및 암호화](http://www.slideshare.net/sm9kr/gsp-7)
* [게임서버프로그래밍 #8 - 성능 평가](http://www.slideshare.net/sm9kr/gsp-8)
