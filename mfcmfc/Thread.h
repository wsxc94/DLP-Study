#pragma once

class Thread
{
protected:
	CWinThread* thread;
public:
	Thread();
	virtual ~Thread();

	CWinThread* GetThread() { return thread; }

	void checkThread();	// 스레드가 실행중인지 체크 함수
	void suspendThread();	// 스레드 멈추기
	virtual void StartThread() = 0;
};

