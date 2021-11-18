#include "pch.h"

Thread::Thread() : thread(NULL)
{
}

Thread::~Thread()
{
	SAFE_DELETE(thread);
}

void Thread::checkThread()
{
	if (thread != NULL)
	{
		// ������ �ٽ� ����
		thread->ResumeThread();
	}
	else {
		// ������ ����
		StartThread();
	}

	if (thread == NULL)
	{
		AfxMessageBox(TEXT("������ ���� ����"));
	}
}

void Thread::suspendThread()
{
	//������ ����
	if (thread != NULL) thread->SuspendThread();
}
