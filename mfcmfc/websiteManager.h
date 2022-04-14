#pragma once

#include "Thread.h"

class websiteManager : public Thread
{
private:
	vector<string> wsBlockList;	// ������ ������Ʈ name list
	HWINEVENTHOOK LHook = 0;
public:
	websiteManager();
	virtual ~websiteManager();

	void WidgetLoad();	// �� ���� �ε� �Լ�
	void FindWindowWidget();	// ���� ������Ʈ Ž��
	void Hook();
	void Unhook();
	virtual void StartThread();	// ������ �����Լ�
	static UINT ThreadUpdata(LPVOID aParam);	// ������ ����
	static void CALLBACK UrlEventHookProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
	vector<string>& getBlockList() { return wsBlockList; }

};

