#pragma once


class KeyHook
{
private:
	CWinThread* thread;
public:
	KeyHook();
	~KeyHook();

	static HHOOK hHook;					// hook ����

	void keyBoardHookLoad();			// Ű���� ��ũ �ε�
	void Start_Hook();					// hook start
	void Stop_Hook();					// hook stop

	void checkThread();			// ������ üũ
	void StartThread();			// ������ ����
	void suspendThread();		// ������ ���� �Լ�

	static UINT ThreadUpdata(LPVOID aParam);	// Ű���� hook ������ ����
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);	// Ű���� hook �ݹ�

	//HHOOK Get_Hook() { return hHook; }
};

