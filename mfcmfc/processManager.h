#pragma once
class processManager
{
private:
	CWinThread* sw_block_Thread;	// 소프트웨어 차단 스레드 변수
	vector<string> sw_block_list;		// 차단할 프로세스 name list
public:
	processManager();
	~processManager();

	bool GetProcessModule(DWORD dwPID, string sProcessName); //프로세스 모듈을 불러오는 함수
	void ProcessLoad(); // 프로세스를 로드하고 리스트를 통해 종료 시키는 함수
	void StartSwBlock(AFX_THREADPROC thread); // 프로세스 감시 스레드 시작 함수

	CWinThread* GetThread() { return sw_block_Thread; } // software block thread getter
};

