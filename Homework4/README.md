# GSP 구현 과제 #4

* 멀티스레드 관련 서버 구현
 * Timer, DoSync, LockOrder(TLS 사용), RWLock (FastSpinLock을 수정), Simple lock-free dispatcher (GCE)
  * TODO 검색해서 구현
 * 숨겨진(?) 버그가 있다면 수정

* 참고: 이 프로그램이 동작하는 시나리오
 1. 클라이언트 로그인 
 2. 플레이어 스타트 (Player::Start) 
 3. Player는 주기적(Player::OnTick)으로 다른 모든 플레이어에게 버프 걸어주기 
 4. 각 Player 받은 버프를 시간이 지나면 제거 (버프는 lock없이 관리)



