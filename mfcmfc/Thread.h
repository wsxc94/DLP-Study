#pragma once

class Thread
{
protected:
	CWinThread* thread;
public:
	Thread();
	virtual ~Thread();

	CWinThread* GetThread() { return thread; }

	void checkThread();	// �����尡 ���������� üũ �Լ�
	void suspendThread();	// ������ ���߱�
	virtual void StartThread() = 0;
};

