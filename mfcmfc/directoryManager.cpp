#include "pch.h"
#include "directoryManager.h"

void directoryManager::FindFileList(string path)
{
    WIN32_FIND_DATAA data;

    try
    {
        // 파일 경로로 핸들을 얻어온다
        HANDLE hFind = FindFirstFileA(path.c_str(), &data);

        if (hFind == INVALID_HANDLE_VALUE) throw runtime_error("FindFirstFile 실패");

        while (FindNextFileA(hFind , &data))
        {
            // 시스템 파일을 제외한 디렉토리 파일 추가
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
                dirList.push_back(string(data.cFileName));
            }
            // 시스템 파일을 제외한 일반 파일 추가
            else if ((data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) &&
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
            {
                fileList.push_back(string(data.cFileName));
            }
        }

        FindClose(hFind);

        testOutput();       // 테스트 출력
    }
    catch (runtime_error e)
    {
        cerr << e.what() << "\n";
    }
}

/*
pszDirectory : 찾고자 하는 폴더의 위치

pszFilter : 검색할 필터

    *.dds  dds 파일만 검색하는 경우

    *      모든 파일및 폴더를 찾는다.

    \\    폴더만 찾는다

iLevel    : 검색 할 하위 폴더의 깊이

    1      바로 하위 폴더만 검색

    -1    모든 폴더 검색

    2      바로 하위 폴더, 그 다음 하위 폴더 검색
*/

void directoryManager::GetFindFileList(char* pszDirectory, char* pszFilter, int iLevel)
{
    if (iLevel == 0) return;

    char buf[MAX_PATH];

    WIN32_FIND_DATA FindFileData;

    bool bAddFile = true;
    bool bAddFolder = false;

    if (pszFilter[1] == 0)
    {
        if (pszFilter[0] == '\\')
        {
            bAddFile = false;
            bAddFolder = true;
        }
        else if (pszFilter[0] == '*')
        {
            bAddFolder = true;
        }
    }

    strcpy_s(buf, MAX_PATH, pszDirectory);
    strcat_s(buf, MAX_PATH, "\\");

    if (bAddFolder == true) strcat_s(buf, MAX_PATH, "*.*");
    else strcat_s(buf, MAX_PATH, pszFilter);

    wchar_t* tmp = ConvertCharToWC(buf);
    
    cout << tmp << "  " << buf << "\n";
    HANDLE hHandle = FindFirstFile(tmp, &FindFileData);
    
    for (; hHandle != INVALID_HANDLE_VALUE; )
    {
        if ((FindFileData.cFileName[0] != '.' || strlen(ConvertWCtoC(FindFileData.cFileName)) > 2) &&
            (0 != _stricmp(ConvertWCtoC(FindFileData.cFileName), "Replay"))) //  Replay 폴더는 지우지 않는다
        {
            strcpy_s(buf, MAX_PATH, pszDirectory);
            strcat_s(buf, MAX_PATH, "\\");
            strcat_s(buf, MAX_PATH, ConvertWCtoC(FindFileData.cFileName));

            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (bAddFolder) fileList.push_back(buf);
                GetFindFileList(buf, pszFilter, iLevel - 1);
            }
            else
            {
                if (bAddFile) fileList.push_back(buf);
            }
        }
        if (!FindNextFile(hHandle, &FindFileData)) break;
    }
    FindClose(hHandle);

    SAFE_DELETE_ARRAY(tmp);
}

void directoryManager::testOutput() // 디렉토리 출력 test 함수
{
    cout << "파일 리스트" << "\n";
    for (string s : fileList) {
        cout << s << "\n";
    }
    cout << "\n";
    
    cout << "디렉토리 리스트" << "\n";
    for (string s : dirList)
    {
        cout << s << "\n";
    }
    cout << "\n";
    
    fileList.clear();
    dirList.clear();

    string dir = "C:/Users/USER/Desktop";

    cout << "c++ 17 filesystem" << "\n";

    string path;
    // dir을 기준으로 디렉토리를 재귀적으로 순회
    for (auto& p : filesystem::recursive_directory_iterator(dir))
    {
        path = string(CT2CA(p.path().c_str()));
        size_t pos = path.rfind('\\');
        cout << path.substr(pos + 1) << "\n";
    }
}
