#pragma once

#include "Thread.h"

class websiteManager : public Thread
{
private:
	vector<string> wsBlockList;	// 차단할 웹사이트 name list
public:
	websiteManager();
	virtual ~websiteManager();

	void WidgetLoad();	// 웹 위젯 로드 함수
	void FindWindowWidget();	// 현재 웹사이트 탐색

	virtual void StartThread();	// 스레드 시작함수
	static UINT ThreadUpdata(LPVOID aParam);	// 스레드 로직
};

