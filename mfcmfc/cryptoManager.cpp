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
	CString fileDir = NULL;	// ���� ���� ���

	// ���� ���� ���̾�α�
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, L"�������(*.*)|*.*||");

	if (IDOK == fileDlg.DoModal()) {

		fileDir = fileDlg.GetPathName();  // ��� �ޱ�
	}

	// ���ϰ�� convert CString -> String
	string path = string(CT2CA(fileDir));

	try
	{
		FILE* p;
		p = fopen(path.c_str(), "rb");	//�б� ���� ����
		if (p == NULL) throw runtime_error("���� �б� ����");	// �б� ���н� ����

		char* ch;			// ��ȣȭ �ڵ� ���� ����
		int size;			// ���� ������ ���� ����
		
		fseek(p, 0, SEEK_END);	// ������ ������ ����
		size = ftell(p);		// ������ ����� ��´�
		fseek(p, 0, SEEK_SET);	// ������ ó������ ����

		ch = new char[sizeof(char) * size];	// ���� ������ ��ŭ �����Ҵ�

		size = fread(ch, sizeof(char), size, p);	//���� ������ ���� �� ���� ch�� ������ ������ �ִ´�
		fclose(p); // p ���� �ݱ�

		for (int i = 0; i < size; i++) ch[i] ^= cryptoCode;	//��ȣȭ �ڵ带 �̿��� ��ȣȭ XOR ����

		p = fopen(path.c_str(), "wb"); // ���� �������� ���� ����
		if (p == NULL) return;

		fwrite(ch, sizeof(char), size, p); // ��ȣȭ�� ���� ch�� ���� ���뿡 ����

		fclose(p);
		
		//rename(path.c_str(), "C:\\Users\\User\\Desktop\\��ȣȭ������");	// ��ȣȭ �� �����̸� ����

		delete[] ch;
	}
	catch (runtime_error e)
	{
		cerr << e.what() << "\n";
		return;
	}
}

void cryptoManager::Decryption() // ��ȣȭ�� ���� �Ȱ���
{
	CString fileDir = NULL;	// ���� ���� ���

	// ���� ���� ���̾�α�
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, L"�������(*.*)|*.*||");

	if (IDOK == fileDlg.DoModal()) {

		fileDir = fileDlg.GetPathName();  // ��� �ޱ�
	}

	// ���ϰ�� convert CString -> String
	string path = string(CT2CA(fileDir));

	try
	{
		FILE* p;
		p = fopen(path.c_str(), "rb");	//�б� ���� ����
		if (p == NULL) throw runtime_error("���� �б� ����");	// �б� ���н� ����

		char* ch;			// ��ȣȭ �ڵ� ���� ����
		int size;			// ���� ������ ���� ����

		fseek(p, 0, SEEK_END);	// ������ ������ ����
		size = ftell(p);		// ������ ����� ��´�
		fseek(p, 0, SEEK_SET);	// ������ ó������ ����

		ch = new char[sizeof(char) * size];	// ���� ������ ��ŭ �����Ҵ�

		size = fread(ch, sizeof(char), size, p);	// ���� ������ ���� �� ���� ch�� ������ ������ �ִ´�
		fclose(p); // p ���� �ݱ�

		for (int i = 0; i < size; i++) ch[i] ^= cryptoCode;	//��ȣȭ �ڵ带 �̿��� ��ȣȭ

		p = fopen(path.c_str(), "wb");		// ���� �������� ����
		if (p == NULL) return;

		fwrite(ch, sizeof(char), size, p);	// ��ȣȭ�� ���� ch�� ���� ���뿡 ����
		fclose(p);

		rename(path.c_str(), savePath.c_str()); // ��ȣȭ �� ó�� �����̸����� �ٽ� ����

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

		// TargetFile�� OrgFileName�� _result�� �߰���.
		//TRACE(m_strOrgFile + "\n");
		//TRACE(m_strTargetFile + "\n");
	}

	cout << "��ȣ �Է� : ";
	cin >> m_strPassword;

	try
	{
		// ���� �ε�
		CFile file;
		if (file.Open(m_strOrgFile, CFile::modeRead | CFile::typeBinary, NULL) == FALSE) throw runtime_error("���� �б� ����");

		// ���� ������ ���̿� �о�� ���۸� �����
		int fileLen = file.GetLength();
		char* fileBuf = new char[fileLen];
		file.Read(fileBuf, fileLen);
		file.Close();

		// �ƿ� ���� �ʱ�ȭ
		char* outBuf = new char[fileLen + 256];
		memset(outBuf, 0x00, fileLen + 256);
		int outLen = 0;

		// check 
		// true ��ȣȭ
		// false ��ȣȭ
		if (check) {
			if (FileEncrypt((const unsigned char*)fileBuf, fileLen, (unsigned char*)outBuf, &outLen) == FALSE)
			{
				throw runtime_error("���� ��ȣȭ �� ���� �߻�!");
			}
		}
		else {
			if (FileDecrypt((const unsigned char*)fileBuf, fileLen, (unsigned char*)outBuf, &outLen) == FALSE)
			{
				throw runtime_error("���� ��ȣȭ �� ���� �߻�!");
			}
		}

		// ��� ���� ����
		if (file.Open(m_strTargetFile, CFile::modeWrite | CFile::modeCreate) == FALSE)
		{
			throw runtime_error("��ȣȭ ���Ͼ��� ����");
		}

		file.Write(outBuf, outLen);
		file.Close();

		delete[] fileBuf;
		delete[] outBuf;

		AfxMessageBox(_T("���� ��/��ȣȭ�� �����߽��ϴ�."));
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

	//Key, IV �����
	unsigned char* keyiv = new unsigned char[32];
	int ret = PKCS5_PBKDF2_HMAC_SHA1(/*cs.GetBuffer(0) */ m_strPassword.c_str(), cs.GetLength(), (unsigned char*)OUR_SALT, strlen(OUR_SALT), ITER, 32, keyiv);

	int len = 0;

	//��ȣȭ
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

	//Key, IV �����
	unsigned char* keyiv = new unsigned char[48];
	int ret = PKCS5_PBKDF2_HMAC_SHA1(m_strPassword.c_str(), cs.GetLength(), (unsigned char*)OUR_SALT, strlen(OUR_SALT), ITER, 32, keyiv);

	int len = 0;

	//��ȣȭ
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
	** true = ��ȣȭ  false = ��ȣȭ **
	**********************************/
	cryptoManager* param = new cryptoManager;
	param = this;

	if (check) thread = AfxBeginThread(EncryptoThread, param); // ��ȣȭ ������ ����
	else thread = AfxBeginThread(DecryptoThread, param);		  // ��ȣȭ ������ ����
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
