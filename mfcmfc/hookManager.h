#pragma once

#include "Thread.h"

class hookManager : public Thread
{
private:
public:
	hookManager();
	~hookManager();

	static HHOOK hHook;					// hook 변수

	void keyBoardHookLoad();			// 키보드 후크 로드
	void Start_Hook();					// hook start
	void Stop_Hook();					// hook stop

	virtual void checkThread();			// 스레드 체크
	virtual void StartThread();			// 스레드 시작
	virtual void suspendThread();		// 스레드 종료 함수

	static UINT ThreadUpdata(LPVOID aParam);	// 키보드 hook 스레드 로직
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);	// 키보드 hook 콜백

	//HHOOK Get_Hook() { return hHook; }
};

