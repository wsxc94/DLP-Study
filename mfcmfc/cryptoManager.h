#pragma once

class cryptoManager
{
private:
	CWinThread* cryptoThread;		// ��ȣȭ ������
	string savePath;				// ��ȣȭ�� ���� ���� ����
	int cryptoCode;					// ��ȣȭ �ڵ�
public:
	cryptoManager();
	~cryptoManager();

	void Encryption();	// ���� ��ȣȭ �Լ�
	void Decryption();	// ���� ��ȣȭ �Լ�
	void StartCryptoThread(AFX_THREADPROC thread);		// ��ȣȭ , ��ȣȭ ������ ���� �Լ�

	CWinThread* GetThread() { return cryptoThread; } // thread getter
};

