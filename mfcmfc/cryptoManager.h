#pragma once
#include "Thread.h"

class cryptoManager : public Thread
{
private:
	string savePath;				// 암호화한 파일 저장 변수
	int cryptoCode;					// 암호화 코드

	/********************************
	* open ssl에 사용하는 경로 변수들 *
	*********************************/
	CString m_strOrgFile;
	CString m_strTargetFile;
	string m_strPassword;

public:
	cryptoManager();
	virtual ~cryptoManager();

	void Encryption();	// 파일 암호화 함수
	void Decryption();	// 파일 복호화 함수

	BOOL FileEncrypt(const unsigned char* orgBuf, int orgLen, unsigned char* descBuf, int* descLen); // 암호화
	BOOL FileDecrypt(const unsigned char* orgBuf, int orgLen, unsigned char* descBuf, int* descLen); // 복호화
	
	void OpenSSL(bool check);	// 암복호화 함수
	
	virtual void StartThread() {}	
	void StartThread(bool check); // 스레드 시작

	static UINT EncryptoThread(LPVOID aParam);	// 암호화 스레드 로직
	static UINT DecryptoThread(LPVOID aParam);	// 복호화 스레드 로직


};

