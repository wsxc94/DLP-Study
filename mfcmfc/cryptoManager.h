#pragma once
#include "Thread.h"

class cryptoManager : public Thread
{
private:
	string savePath;				// ��ȣȭ�� ���� ���� ����
	int cryptoCode;					// ��ȣȭ �ڵ�
public:
	cryptoManager();
	virtual ~cryptoManager();

	void Encryption();	// ���� ��ȣȭ �Լ�
	void Decryption();	// ���� ��ȣȭ �Լ�
	
	virtual void StartThread() {}	
	void StartThread(bool check); // ������ ����

	static UINT EncryptoThread(LPVOID aParam);	// ��ȣȭ ������ ����
	static UINT DecryptoThread(LPVOID aParam);	// ��ȣȭ ������ ����
};

