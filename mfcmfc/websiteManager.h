#pragma once
class websiteManager
{
private:
	CWinThread* wsBlockThread;	// ������Ʈ ���� ������ ����
	vector<string> wsBlockList;	// ������ ������Ʈ name list
public:
	websiteManager();
	~websiteManager();

	void WidgetLoad();	// �� ���� �ε� �Լ�
	void StartWsBlock(AFX_THREADPROC thread); // ���μ��� ���� ������ ���� �Լ�
	void FindWindowWidget();	// ���� ������Ʈ Ž��
	CWinThread* GetThread() { return wsBlockThread; } // website block thread getter
};

