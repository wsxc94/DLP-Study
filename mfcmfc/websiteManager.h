#pragma once

#include "Thread.h"

class websiteManager : public Thread
{
private:
	vector<string> wsBlockList;	// ������ ������Ʈ name list
public:
	websiteManager();
	virtual ~websiteManager();

	void WidgetLoad();	// �� ���� �ε� �Լ�
	void FindWindowWidget();	// ���� ������Ʈ Ž��

	virtual void StartThread();	// ������ �����Լ�
	static UINT ThreadUpdata(LPVOID aParam);	// ������ ����
};

