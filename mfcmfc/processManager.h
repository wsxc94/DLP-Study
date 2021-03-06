#pragma once
#include "Thread.h"
class processManager : public Thread
{
private:
	vector<CString> sw_block_list;		// 차단할 프로세스 name list
public:
	processManager();
	virtual ~processManager();

	BOOL FindProcess(LPCTSTR lpszProcessName, set<DWORD>* stPID = NULL);
	bool GetProcessModule(DWORD pid, string ProcessName); //프로세스 모듈을 불러오는 함수
	DWORD GetProcessByName(CString name);
	void ProcessLoad(); // 프로세스를 로드하고 리스트를 통해 종료 시키는 함수

	virtual void StartThread();		// 스레드 시작
	static UINT ThreadUpdata(LPVOID aParam);	// 스레드 로직
};