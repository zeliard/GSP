# GSP 구현 과제 #4

4월 27일 월요일 자정까지: (중간 고사 기간으로 인하여 하루 늦춰즘)
(다음주 월요일은 휴강, 수요일 수업시간에 리뷰 및 C++ ADV 모두 할 예정)

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



