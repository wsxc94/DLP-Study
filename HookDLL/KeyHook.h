#pragma once

void keyBoardHookLoad();			// Ű���� ��ũ �ε�
void Start_Hook();					// hook start
void Stop_Hook();					// hook stop

void checkThread();			// ������ üũ
void StartThread();			// ������ ����
void suspendThread();		// ������ ���� �Լ�

UINT ThreadUpdata(LPVOID aParam);	// Ű���� hook ������ ����
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);	// Ű���� hook �ݹ�

