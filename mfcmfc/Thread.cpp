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
		// 스레드 다시 실행
		thread->ResumeThread();
	}
	else {
		// 스레드 실행
		StartThread();
	}

	if (thread == NULL)
	{
		AfxMessageBox(TEXT("스레드 실행 실패"));
	}
}

void Thread::suspendThread()
{
	//스레드 멈춤
	if (thread != NULL) thread->SuspendThread();
}
