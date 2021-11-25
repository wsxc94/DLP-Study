#pragma once

void keyBoardHookLoad();			// 키보드 후크 로드
void Start_Hook();					// hook start
void Stop_Hook();					// hook stop

void checkThread();			// 스레드 체크
void StartThread();			// 스레드 시작
void suspendThread();		// 스레드 종료 함수

UINT ThreadUpdata(LPVOID aParam);	// 키보드 hook 스레드 로직
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);	// 키보드 hook 콜백

