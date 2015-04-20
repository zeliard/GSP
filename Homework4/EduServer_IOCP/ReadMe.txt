
IOCP Echo Server for educational purposes only.

# Student Homework 

8월 16일 토요일 자정까지: (약 2주)
(다음주는 휴강)

*IOCP를 사용하여 Dummy Client 구현. 
(다른 프로젝트로 분리, perf_client처럼 특정 시간동안 멀티 세션을 맺어서 send/recv 양 측정 가능하게, ConnectEx 사용)

*멀티스레드 관련 서버 구현
Timer, DoSync, LockOrder(TLS 사용), RWLock (FastSpinLock을 수정)
Simple lock-free dispatcher (GCE)

  - TODO 검색해서 구현
  - 숨겨진(?) 버그 수정


*참고: 이 프로그램이 동작하는 시나리오
클라이언트 로그인 
플레이어 스타트 (Player::Start) 
Player는 주기적(Player::OnTick)으로 다른 모든 플레이어에게 버프 걸어주기 
각 Player 받은 버프를 시간이 지나면 제거 (버프는 lock없이 관리)


# 참고자료 (과제 구현시 도움이 될 수 있음. 그리고, 과제와 별개로도 반드시 이해할 것)

https://github.com/zeliard/Dispatcher/tree/bind_version


