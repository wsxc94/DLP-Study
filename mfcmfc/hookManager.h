#pragma once

#include "Thread.h"

class hookManager : public Thread
{
private:
public:
	hookManager();
	~hookManager();

	static HHOOK hHook;					// 후크 변수
	
	void keyBoardHookLoad();			// 키보드 후크 로드
	void SetHook();						// 후크 set
	void UnHook();						// 후크 out
	void StartThread();					// 스레드 시작

	static UINT ThreadUpdata(LPVOID aParam);	// 스레드 로직
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);	// 키보드 후크 콜백

	HHOOK Get_Hook() { return hHook; }	
};

