#pragma once
#include "Thread.h"

class cryptoManager : public Thread
{
private:
	string savePath;				// 암호화한 파일 저장 변수
	int cryptoCode;					// 암호화 코드
public:
	cryptoManager();
	virtual ~cryptoManager();

	void Encryption();	// 파일 암호화 함수
	void Decryption();	// 파일 복호화 함수
	
	void StartThread() {}	
	void StartThread(bool check); // 스레드 시작

	static UINT EncryptoThread(LPVOID aParam);	// 암호화 스레드 로직
	static UINT DecryptoThread(LPVOID aParam);	// 복호화 스레드 로직
};

