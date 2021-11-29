#pragma once
#include "Thread.h"

class cryptoManager : public Thread
{
private:
	string savePath;				// ��ȣȭ�� ���� ���� ����
	int cryptoCode;					// ��ȣȭ �ڵ�

	CString m_strOrgFile;
	CString m_strTargetFile;
	string m_strPassword;

public:
	cryptoManager();
	virtual ~cryptoManager();

	void Encryption();	// ���� ��ȣȭ �Լ�
	void Decryption();	// ���� ��ȣȭ �Լ�

	BOOL FileEncrypt(const unsigned char* orgBuf, int orgLen, unsigned char* descBuf, int* descLen);
	BOOL FileDecrypt(const unsigned char* orgBuf, int orgLen, unsigned char* descBuf, int* descLen);
	
	void OpenSSL(bool check);
	
	virtual void StartThread() {}	
	void StartThread(bool check); // ������ ����

	static UINT EncryptoThread(LPVOID aParam);	// ��ȣȭ ������ ����
	static UINT DecryptoThread(LPVOID aParam);	// ��ȣȭ ������ ����


};

