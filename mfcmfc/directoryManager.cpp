#include "pch.h"
#include "directoryManager.h"

void directoryManager::FindFileList(string path)
{
    WIN32_FIND_DATAA data;

    try
    {
        // ���� ��η� �ڵ��� ���´�
        HANDLE hFind = FindFirstFileA(path.c_str(), &data);

        if (hFind == INVALID_HANDLE_VALUE) throw runtime_error("FindFirstFile ����");

        while (FindNextFileA(hFind , &data))
        {
            // �ý��� ������ ������ ���丮 ���� �߰�
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
                dirList.push_back(string(data.cFileName));
            }
            // �ý��� ������ ������ �Ϲ� ���� �߰�
            else if ((data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) &&
                !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
            {
                fileList.push_back(string(data.cFileName));
            }
        }

        FindClose(hFind);

        testOutput();       // �׽�Ʈ ���
    }
    catch (runtime_error e)
    {
        cerr << e.what() << "\n";
    }
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

void directoryManager::testOutput() // ���丮 ��� test �Լ�
{
    cout << "���� ����Ʈ" << "\n";
    for (string s : fileList) {
        cout << s << "\n";
    }
    cout << "\n";
    
    cout << "���丮 ����Ʈ" << "\n";
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
    // dir�� �������� ���丮�� ��������� ��ȸ
    for (auto& p : filesystem::recursive_directory_iterator(dir))
    {
        path = string(CT2CA(p.path().c_str()));
        size_t pos = path.rfind('\\');
        cout << path.substr(pos + 1) << "\n";
    }
}
