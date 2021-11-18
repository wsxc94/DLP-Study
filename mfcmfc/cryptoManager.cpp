#include "pch.h"
#include "cryptoManager.h"

cryptoManager::cryptoManager() : cryptoCode(25)
{
}

cryptoManager::~cryptoManager()
{
}

void cryptoManager::Encryption()
{
	string path;
	cout << "암호화 할 파일 경로 입력 : ";
	cin >> path;

	savePath = path;	// 원래 경로 따로 저장

	try
	{
		FILE* p;
		p = fopen(path.c_str(), "rb");	//읽기 전용 오픈
		if (p == NULL) throw runtime_error("파일 읽기 실패");	// 읽기 실패시 종료

		char* ch;			// 암호화 코드 저장 변수
		int size;			// 파일 사이즈 저장 변수
		
		fseek(p, 0, SEEK_END);	// 파일의 끝으로 간다
		size = ftell(p);		// 파일의 사이즈를 얻는다
		fseek(p, 0, SEEK_SET);	// 파일의 처음으로 간다

		ch = new char[sizeof(char) * size];	// 파일 사이즈 만큼 동적할당

		size = fread(ch, sizeof(char), size, p);	//파일 내용을 읽은 뒤 변수 ch에 파일의 내용을 넣는다
		fclose(p); // p 파일 닫기

		for (int i = 0; i < size; i++) ch[i] ^= cryptoCode;	//암호화 코드를 이용해 암호화

		p = fopen(path.c_str(), "wb"); // 쓰기 형식으로 파일 오픈
		if (p == NULL) return;

		fwrite(ch, sizeof(char), size, p); // 암호화된 변수 ch를 파일 내용에 쓰기

		fclose(p);
		
		rename(path.c_str(), "C:\\Users\\User\\Desktop\\암호화된파일");	// 암호화 후 파일이름 변경

		delete[] ch;
	}
	catch (runtime_error e)
	{
		cerr << e.what() << "\n";
		return;
	}
}

void cryptoManager::Decryption()
{
	string path;
	cout << "복호화 할 파일 경로 입력 : ";
	cin >> path;

	try
	{
		FILE* p;
		p = fopen(path.c_str(), "rb");	//읽기 전용 오픈
		if (p == NULL) throw runtime_error("파일 읽기 실패");	// 읽기 실패시 종료

		char* ch;			// 암호화 코드 저장 변수
		int size;			// 파일 사이즈 저장 변수

		fseek(p, 0, SEEK_END);	// 파일의 끝으로 간다
		size = ftell(p);		// 파일의 사이즈를 얻는다
		fseek(p, 0, SEEK_SET);	// 파일의 처음으로 간다

		ch = new char[sizeof(char) * size];	// 파일 사이즈 만큼 동적할당

		size = fread(ch, sizeof(char), size, p);	// 파일 내용을 읽은 뒤 변수 ch에 파일의 내용을 넣는다
		fclose(p); // p 파일 닫기

		for (int i = 0; i < size; i++) ch[i] ^= cryptoCode;	//암호화 코드를 이용해 복호화

		p = fopen(path.c_str(), "wb");		// 쓰기 전용으로 오픈
		if (p == NULL) return;

		fwrite(ch, sizeof(char), size, p);	// 복호화된 변수 ch를 파일 내용에 쓰기
		fclose(p);

		rename(path.c_str(), savePath.c_str()); // 복호화 후 처음 파일이름으로 다시 변경

		delete[] ch;
	}
	catch (runtime_error e)
	{
		cerr << e.what() << "\n";
		return;
	}
}

void cryptoManager::StartThread(bool check)
{
	/**********************************
	** true = 암호화  false = 복호화 **
	**********************************/
	cryptoManager* param = new cryptoManager;
	param = this;

	if (check) thread = AfxBeginThread(EncryptoThread, param); // 암호화 스레드 실행
	else thread = AfxBeginThread(DecryptoThread, param);		  // 복호화 스레드 실행
}

UINT cryptoManager::EncryptoThread(LPVOID aParam)
{
	cryptoManager* pThis = (cryptoManager*)aParam;
	pThis->Encryption();

	return 0;
}

UINT cryptoManager::DecryptoThread(LPVOID aParam)
{
	cryptoManager* pThis = (cryptoManager*)aParam;
	pThis->Decryption();

	return 0;
}
