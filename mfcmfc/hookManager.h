#pragma once

#include "Thread.h"

class hookManager : public Thread
{
private:
public:
	hookManager();
	~hookManager();

	static HHOOK hHook;					// hook ����

	void keyBoardHookLoad();			// Ű���� ��ũ �ε�
	void Start_Hook();					// hook start
	void Stop_Hook();					// hook stop

	virtual void checkThread();			// ������ üũ
	virtual void StartThread();			// ������ ����
	virtual void suspendThread();		// ������ ���� �Լ�

	static UINT ThreadUpdata(LPVOID aParam);	// Ű���� hook ������ ����
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);	// Ű���� hook �ݹ�
};

