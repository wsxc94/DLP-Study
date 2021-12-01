#include "pch.h"
#include "directoryManager.h"

void directoryManager::FindFileList()
{
    BROWSEINFO BrInfo;      // �������� �������̽�
    TCHAR szBuffer[512];    // ������� ���� 

    //�޸� �ʱ�ȭ
    ::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
    ::ZeroMemory(szBuffer, 512);

    BrInfo.hwndOwner = NULL; //GetSafeHwnd();
    BrInfo.lpszTitle = _T("�˻��� ������ �����ϼ���");
    BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

    LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);    // ���� ����Ʈ
    ::SHGetPathFromIDList(pItemIdList, szBuffer);               // ���ϰ�� �о����

    CString str;
    str.Format(_T("%s"), szBuffer);

    if (str.IsEmpty()) return;

    // ������ ���� or ���丮�� ��θ� �޾ƿ´�
    string path = string(CT2CA(str)) + char('\\\\') + '*';

    queue<string> q_path;       // ���� ��� queue
    q_path.push(path);
    path.pop_back();

    WIN32_FIND_DATAA data; // ���� �����͸� �޾ƿ� ����

    while (!q_path.empty())
    {
        string current_path = q_path.front(); // ���� ��θ� �����´�
        q_path.pop();
 
        HANDLE hFind = FindFirstFileA(current_path.c_str(), &data); // ���� ��� �������� �ڵ��� �����´�

        // ���� ���Ⱑ �����ϸ� return
        if (hFind == INVALID_HANDLE_VALUE) continue;

        while (FindNextFileA(hFind, &data))
        {
            // �ý��� ������ ������ ���丮 ���� �߰�
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
                dirList.push_back(string(data.cFileName));

                if (string(data.cFileName) == "..") continue;
                  
                // ���� ���� ��θ� queue�� push �Ѵ�
                string next_path = current_path;                  
                next_path.pop_back();      
                next_path += string(data.cFileName) + char('\\\\') + '*';                  
                q_path.push(next_path);
            }
            // �ý��� ������ ������ �Ϲ� ���� �߰�
            else if ((data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) &&
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
            {
                // �Ϲ� ���� ����Ʈ�� push�Ѵ�
                string filepath = current_path;
                filepath.pop_back();
                filepath += string(data.cFileName);
                //fileList.push_back(string(data.cFileName));
                fileList.push_back(filepath);
            }
        }
        FindClose(hFind);     
    }
    FindFileInfo(path);       // �׽�Ʈ ���
}

/*
pszDirectory : ã���� �ϴ� ������ ��ġ

pszFilter : �˻��� ����

    *.dds  dds ���ϸ� �˻��ϴ� ���

    *      ��� ���Ϲ� ������ ã�´�.

    \\    ������ ã�´�

iLevel    : �˻� �� ���� ������ ����

    1      �ٷ� ���� ������ �˻�

    -1    ��� ���� �˻�

    2      �ٷ� ���� ����, �� ���� ���� ���� �˻�
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
            (0 != _stricmp(ConvertWCtoC(FindFileData.cFileName), "Replay"))) //  Replay ������ ������ �ʴ´�
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

void directoryManager::FindFileInfo(string path) // ���丮 ��� test �Լ�
{
    for (int i = 0; i < fileList.size(); i++)
    {
        try
        {
            FILE* p;
            p = fopen(fileList[i].c_str(), "rb");	//�б� ���� ����
            if (p == NULL) throw runtime_error("���� �б� ����");	// �б� ���н� ����

            char* ch;			// ��ȣȭ �ڵ� ���� ����
            int size;			// ���� ������ ���� ����

            fseek(p, 0, SEEK_END);	// ������ ������ ����
            size = ftell(p);		// ������ ����� ��´�
            fseek(p, 0, SEEK_SET);	// ������ ó������ ����

            ch = new char[sizeof(char) * size];	// ���� ������ ��ŭ �����Ҵ�

            size = fread(ch, sizeof(char), size, p);	//���� ������ ���� �� ���� ch�� ������ ������ �ִ´�
            fclose(p); // p ���� �ݱ�

            string buf = ch;

            if (buf.find("@") != string::npos) {
                cout << fileList[i] << " ���� �ΰ������� �߰ߵǾ����ϴ�." << "\n";
                cout << "���� : " << buf << "\n\n";
            }

            delete[] ch;
        }
        catch (runtime_error e)
        {
            cerr << e.what() << "\n";
            return;
        }
    }

    //cout << "���� ����Ʈ" << "\n";
    //for (string s : fileList) {
    //    cout << s << "\n";
    //}
    //cout << "\n";
    //
    //cout << "���丮 ����Ʈ" << "\n";
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

    // dir�� �������� ���丮�� ��������� ��ȸ
    for (auto& p : filesystem::recursive_directory_iterator(path))
    {
        path = string(CT2CA(p.path().c_str()));
        size_t pos = path.rfind('\\');
        cout << path.substr(pos + 1) << "\n";
    }
}
