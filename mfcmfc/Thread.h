#pragma once

class Thread
{
protected:
	CWinThread* thread;
public:
	Thread();
	virtual ~Thread();

	CWinThread* GetThread() { return thread; }

	virtual void checkThread();	// �����尡 ���������� üũ �Լ�
	virtual void suspendThread();	// ������ ���߱�
	virtual void StartThread() = 0;	// ������ ���� �Լ� ���� �����ٶ�
};

