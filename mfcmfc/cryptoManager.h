#pragma once

class cryptoManager
{
private:
	CWinThread* cryptoThread;		// 암호화 스레드
	string savePath;				// 암호화한 파일 저장 변수
	int cryptoCode;					// 암호화 코드
public:
	cryptoManager();
	~cryptoManager();

	void Encryption();	// 파일 암호화 함수
	void Decryption();	// 파일 복호화 함수
	void StartCryptoThread(AFX_THREADPROC thread);		// 암호화 , 복호화 스레드 시작 함수

	CWinThread* GetThread() { return cryptoThread; } // thread getter
};

