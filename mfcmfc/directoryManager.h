#pragma once
class directoryManager
{
private:
	vector<string> fileList;	// ���� ����Ʈ
	vector<string> dirList;		// ���丮 ����Ʈ
public:
	void FindFileList(string path);		//���� ����Ʈ �˻� �Լ�
	void GetFindFileList(char* pszDirectory, char* pszFilter, int iLevel = -1);
	void testOutput();					//���� , ���丮 ����Ʈ ����Լ�
};

