#include "pch.h"
#include "cryptoManager.h"

#include <openssl/conf.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define OUR_SALT "fileenc_oursalt"
#define ITER 4096

cryptoManager::cryptoManager() : cryptoCode(25)
{
}

cryptoManager::~cryptoManager()
{
}

void cryptoManager::Encryption()
{
	CString fileDir = NULL;	// 받을 파일 경로

	// 파일 선택 다이얼로그
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, L"모든파일(*.*)|*.*||");

	if (IDOK == fileDlg.DoModal()) {

		fileDir = fileDlg.GetPathName();  // 경로 받기
	}

	// 파일경로 convert CString -> String
	string path = string(CT2CA(fileDir));

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

		for (int i = 0; i < size; i++) ch[i] ^= cryptoCode;	//암호화 코드를 이용해 암호화 XOR 연산

		p = fopen(path.c_str(), "wb"); // 쓰기 형식으로 파일 오픈
		if (p == NULL) return;

		fwrite(ch, sizeof(char), size, p); // 암호화된 변수 ch를 파일 내용에 쓰기

		fclose(p);
		
		//rename(path.c_str(), "C:\\Users\\User\\Desktop\\암호화된파일");	// 암호화 후 파일이름 변경

		delete[] ch;
	}
	catch (runtime_error e)
	{
		cerr << e.what() << "\n";
		return;
	}
}

void cryptoManager::Decryption() // 암호화랑 로직 똑같음
{
	CString fileDir = NULL;	// 받을 파일 경로

	// 파일 선택 다이얼로그
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, L"모든파일(*.*)|*.*||");

	if (IDOK == fileDlg.DoModal()) {

		fileDir = fileDlg.GetPathName();  // 경로 받기
	}

	// 파일경로 convert CString -> String
	string path = string(CT2CA(fileDir));

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

void cryptoManager::OpenSSL(bool check)
{
	
	CFileDialog fdlg(TRUE);
	if (fdlg.DoModal() == IDOK)
	{
		m_strOrgFile = fdlg.GetPathName();
		CString OrgFileName = fdlg.GetFileName();
		CString OnlyFileName = OrgFileName.Left(OrgFileName.ReverseFind('.'));
		CString OrgFilePath = m_strOrgFile.Left(m_strOrgFile.ReverseFind('\\') + 1);

		m_strTargetFile = OrgFilePath + OnlyFileName + "_result." + fdlg.GetFileExt();

		// TargetFile은 OrgFileName에 _result를 추가함.
		//TRACE(m_strOrgFile + "\n");
		//TRACE(m_strTargetFile + "\n");
	}

	cout << "암호 입력 : ";
	cin >> m_strPassword;

	try
	{
		// 파일 로딩
		CFile file;
		if (file.Open(m_strOrgFile, CFile::modeRead | CFile::typeBinary, NULL) == FALSE) throw runtime_error("파일 읽기 실패");

		// 원본 파일의 길이와 읽어올 버퍼를 만든다
		int fileLen = file.GetLength();
		char* fileBuf = new char[fileLen];
		file.Read(fileBuf, fileLen);
		file.Close();

		// 아웃 버퍼 초기화
		char* outBuf = new char[fileLen + 256];
		memset(outBuf, 0x00, fileLen + 256);
		int outLen = 0;

		// check 
		// true 암호화
		// false 복호화
		if (check) {
			if (FileEncrypt((const unsigned char*)fileBuf, fileLen, (unsigned char*)outBuf, &outLen) == FALSE)
			{
				throw runtime_error("파일 암호화 중 에러 발생!");
			}
		}
		else {
			if (FileDecrypt((const unsigned char*)fileBuf, fileLen, (unsigned char*)outBuf, &outLen) == FALSE)
			{
				throw runtime_error("파일 복호화 중 에러 발생!");
			}
		}

		// 결과 파일 쓰기
		if (file.Open(m_strTargetFile, CFile::modeWrite | CFile::modeCreate) == FALSE)
		{
			throw runtime_error("암호화 파일쓰기 실패");
		}

		file.Write(outBuf, outLen);
		file.Close();

		delete[] fileBuf;
		delete[] outBuf;

		AfxMessageBox(_T("파일 암/복호화에 성공했습니다."));
	}
	catch (runtime_error e)
	{
		cerr << e.what() << "\n";
		return;
	}
}

BOOL cryptoManager::FileEncrypt(const unsigned char* orgBuf, int orgLen, unsigned char* descBuf, int* descLen)
{
	CString cs(m_strPassword.c_str());
	//const char* c = m_strPassword.c_str();

	//Key, IV 만들기
	unsigned char* keyiv = new unsigned char[32];
	int ret = PKCS5_PBKDF2_HMAC_SHA1(/*cs.GetBuffer(0) */ m_strPassword.c_str(), cs.GetLength(), (unsigned char*)OUR_SALT, strlen(OUR_SALT), ITER, 32, keyiv);

	int len = 0;

	//암호화
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);

	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, &keyiv[0], &keyiv[16]))
	{
		//OpenErrLog();
		EVP_CIPHER_CTX_free(ctx);
		return FALSE;
	}

	if (1 != EVP_EncryptUpdate(ctx, descBuf, &len, orgBuf, orgLen))
	{
		//OpenErrLog();
		EVP_CIPHER_CTX_free(ctx);
		return FALSE;
	}
	*descLen = len;

	if (1 != EVP_EncryptFinal_ex(ctx, descBuf + len, &len))
	{
		//OpenErrLog();
		EVP_CIPHER_CTX_free(ctx);
		return FALSE;
	}
	*descLen += len;

	EVP_CIPHER_CTX_free(ctx);
	delete[] keyiv;

	return TRUE;
}

BOOL cryptoManager::FileDecrypt(const unsigned char* orgBuf, int orgLen, unsigned char* descBuf, int* descLen)
{
	CString cs(m_strPassword.c_str());

	//Key, IV 만들기
	unsigned char* keyiv = new unsigned char[48];
	int ret = PKCS5_PBKDF2_HMAC_SHA1(m_strPassword.c_str(), cs.GetLength(), (unsigned char*)OUR_SALT, strlen(OUR_SALT), ITER, 32, keyiv);

	int len = 0;

	//암호화
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);

	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, &keyiv[0], &keyiv[16]))
	{
		//OpenErrLog();
		EVP_CIPHER_CTX_free(ctx);
		return FALSE;
	}

	if (1 != EVP_DecryptUpdate(ctx, descBuf, &len, orgBuf, orgLen))
	{
		//OpenErrLog();
		EVP_CIPHER_CTX_free(ctx);
		return FALSE;
	}
	*descLen = len;

	if (1 != EVP_DecryptFinal_ex(ctx, descBuf + len, &len))
	{
		//OpenErrLog();
		EVP_CIPHER_CTX_free(ctx);
		return FALSE;
	}
	*descLen += len;

	EVP_CIPHER_CTX_free(ctx);
	delete[] keyiv;

	return TRUE;
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
	//pThis->OpenSSL(true);

	return 0;
}

UINT cryptoManager::DecryptoThread(LPVOID aParam)
{
	cryptoManager* pThis = (cryptoManager*)aParam;
	pThis->Decryption();
	//pThis->OpenSSL(false);

	return 0;
}
