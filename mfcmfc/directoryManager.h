#pragma once
class directoryManager
{
private:
	vector<string> fileList;	// 파일 리스트
	vector<string> dirList;		// 디렉토리 리스트
public:
	void FindFileList(string path);		//파일 리스트 검색 함수
	void GetFindFileList(char* pszDirectory, char* pszFilter, int iLevel = -1);	// 파일 리스트 검색 함수
	void testOutput(string path);		//파일 , 디렉토리 리스트 출력함수
};

