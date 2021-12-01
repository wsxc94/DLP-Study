#include "pch.h"
#include "directoryManager.h"

void directoryManager::FindFileList()
{
    BROWSEINFO BrInfo;      // 폴더선택 인터페이스
    TCHAR szBuffer[512];    // 경로저장 버퍼 

    //메모리 초기화
    ::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
    ::ZeroMemory(szBuffer, 512);

    BrInfo.hwndOwner = NULL; //GetSafeHwnd();
    BrInfo.lpszTitle = _T("검사할 폴더를 선택하세요");
    BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

    LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);    // 폴더 리스트
    ::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

    CString str;
    str.Format(_T("%s"), szBuffer);

    if (str.IsEmpty()) return;

    // 선택한 파일 or 디렉토리의 경로를 받아온다
    string path = string(CT2CA(str)) + char('\\\\') + '*';

    queue<string> q_path;       // 파일 경로 queue
    q_path.push(path);
    path.pop_back();

    WIN32_FIND_DATAA data; // 파일 데이터를 받아올 변수

    while (!q_path.empty())
    {
        string current_path = q_path.front(); // 현재 경로를 가져온다
        q_path.pop();
 
        HANDLE hFind = FindFirstFileA(current_path.c_str(), &data); // 현재 경로 데이터의 핸들을 가져온다

        // 파일 열기가 실패하면 return
        if (hFind == INVALID_HANDLE_VALUE) continue;

        while (FindNextFileA(hFind, &data))
        {
            // 시스템 파일을 제외한 디렉토리 파일 추가
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
                dirList.push_back(string(data.cFileName));

                if (string(data.cFileName) == "..") continue;
                  
                // 다음 폴더 경로를 queue에 push 한다
                string next_path = current_path;                  
                next_path.pop_back();      
                next_path += string(data.cFileName) + char('\\\\') + '*';                  
                q_path.push(next_path);
            }
            // 시스템 파일을 제외한 일반 파일 추가
            else if ((data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) &&
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
            {
                // 일반 파일 리스트에 push한다
                string filepath = current_path;
                filepath.pop_back();
                filepath += string(data.cFileName);
                //fileList.push_back(string(data.cFileName));
                fileList.push_back(filepath);
            }
        }
        FindClose(hFind);     
    }
    FindFileInfo(path);       // 테스트 출력
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

void directoryManager::FindFileInfo(string path) // 디렉토리 출력 test 함수
{
    for (int i = 0; i < fileList.size(); i++)
    {
        try
        {
            FILE* p;
            p = fopen(fileList[i].c_str(), "rb");	//읽기 전용 오픈
            if (p == NULL) throw runtime_error("파일 읽기 실패");	// 읽기 실패시 종료

            char* ch;			// 암호화 코드 저장 변수
            int size;			// 파일 사이즈 저장 변수

            fseek(p, 0, SEEK_END);	// 파일의 끝으로 간다
            size = ftell(p);		// 파일의 사이즈를 얻는다
            fseek(p, 0, SEEK_SET);	// 파일의 처음으로 간다

            ch = new char[sizeof(char) * size];	// 파일 사이즈 만큼 동적할당

            size = fread(ch, sizeof(char), size, p);	//파일 내용을 읽은 뒤 변수 ch에 파일의 내용을 넣는다
            fclose(p); // p 파일 닫기

            string buf = ch;

            if (buf.find("@") != string::npos) {
                cout << fileList[i] << " 에서 민감정보가 발견되었습니다." << "\n";
                cout << "내용 : " << buf << "\n\n";
            }

            delete[] ch;
        }
        catch (runtime_error e)
        {
            cerr << e.what() << "\n";
            return;
        }
    }

    //cout << "파일 리스트" << "\n";
    //for (string s : fileList) {
    //    cout << s << "\n";
    //}
    //cout << "\n";
    //
    //cout << "디렉토리 리스트" << "\n";
    //for (string s : dirList)
    //{
    //    cout << s << "\n";
    //}
    //cout << "\n";
    //
    //fileList.clear();
    //dirList.clear();
    cout << "\n\n";
    string dir = "C:/Users/USER/Desktop";

    cout << "c++ 17 filesystem" << "\n";

    path.pop_back();

    // dir을 기준으로 디렉토리를 재귀적으로 순회
    for (auto& p : filesystem::recursive_directory_iterator(path))
    {
        path = string(CT2CA(p.path().c_str()));
        size_t pos = path.rfind('\\');
        cout << path.substr(pos + 1) << "\n";
    }
}
