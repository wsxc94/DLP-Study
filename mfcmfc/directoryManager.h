#pragma once
#include "Thread.h"
class directoryManager : public Thread
{
private:
	vector<string> fileList;	// ���� ����Ʈ
	vector<string> dirList;		// ���丮 ����Ʈ
public:
	void FindFileList();		//���� ����Ʈ �˻� �Լ�
	void GetFindFileList(char* pszDirectory, char* pszFilter, int iLevel = -1);	// ���� ����Ʈ �˻� �Լ�
	void FindFileInfo(string path);		//���� , ���丮 ����Ʈ ����Լ�

	virtual void StartThread();

	static UINT FindFileThread(LPVOID aParam); // �ΰ� ���� �˻� ������
};

