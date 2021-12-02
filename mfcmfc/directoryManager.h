#pragma once
#include "Thread.h"
class directoryManager : public Thread
{
private:
	vector<string> fileList;	// 파일 리스트
	vector<string> dirList;		// 디렉토리 리스트
public:
	void FindFileList();		//파일 리스트 검색 함수
	void GetFindFileList(char* pszDirectory, char* pszFilter, int iLevel = -1);	// 파일 리스트 검색 함수
	void FindFileInfo(string path);		//파일 , 디렉토리 리스트 출력함수

	virtual void StartThread();

	static UINT FindFileThread(LPVOID aParam); // 민감 정보 검사 스레드
};

