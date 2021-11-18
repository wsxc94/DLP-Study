#include "pch.h"
#include "cryptoManager.h"

cryptoManager::cryptoManager() : cryptoThread(NULL) , cryptoCode(25)
{
}

cryptoManager::~cryptoManager()
{
}

void cryptoManager::Encryption()
{
	string path;
	cout << "��ȣȭ �� ���� ��� �Է� : ";
	cin >> path;

	savePath = path;	// ���� ��� ���� ����

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

		for (int i = 0; i < size; i++) ch[i] ^= cryptoCode;	//��ȣȭ �ڵ带 �̿��� ��ȣȭ

		p = fopen(path.c_str(), "wb"); // ���� �������� ���� ����
		if (p == NULL) return;

		fwrite(ch, sizeof(char), size, p); // ��ȣȭ�� ���� ch�� ���� ���뿡 ����

		fclose(p);
		
		rename(path.c_str(), "C:\\Users\\User\\Desktop\\��ȣȭ������");	// ��ȣȭ �� �����̸� ����

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
	cout << "��ȣȭ �� ���� ��� �Է� : ";
	cin >> path;

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

void cryptoManager::StartCryptoThread(AFX_THREADPROC thread)
{
	cryptoThread = AfxBeginThread(thread, NULL);
}