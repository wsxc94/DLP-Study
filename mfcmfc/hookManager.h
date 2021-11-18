#pragma once

#include "Thread.h"

class hookManager : public Thread
{
private:
public:
	hookManager();
	~hookManager();

	static HHOOK hHook;					// ��ũ ����
	
	void keyBoardHookLoad();			// Ű���� ��ũ �ε�
	void SetHook();						// ��ũ set
	void UnHook();						// ��ũ out
	void StartThread();					// ������ ����

	static UINT ThreadUpdata(LPVOID aParam);	// ������ ����
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);	// Ű���� ��ũ �ݹ�

	HHOOK Get_Hook() { return hHook; }	
};

