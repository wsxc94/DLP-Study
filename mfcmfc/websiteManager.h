#pragma once
class websiteManager
{
private:
	CWinThread* wsBlockThread;	// 웹사이트 차단 스레드 변수
	vector<string> wsBlockList;	// 차단할 웹사이트 name list
public:
	websiteManager();
	~websiteManager();

	void WidgetLoad();	// 웹 위젯 로드 함수
	void StartWsBlock(AFX_THREADPROC thread); // 프로세스 감시 스레드 시작 함수
	void FindWindowWidget();	// 현재 웹사이트 탐색
	CWinThread* GetThread() { return wsBlockThread; } // website block thread getter
};

