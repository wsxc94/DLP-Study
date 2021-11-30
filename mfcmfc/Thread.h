#pragma once

class Thread
{
protected:
	CWinThread* thread;	// 스레드 변수
public:
	Thread();
	virtual ~Thread();

	CWinThread* GetThread() { return thread; }

	virtual void checkThread();	// 스레드가 실행중인지 체크 함수
	virtual void suspendThread();	// 스레드 멈추기
	virtual void StartThread() = 0;	// 스레드 시작 함수 각자 구현바람
};

