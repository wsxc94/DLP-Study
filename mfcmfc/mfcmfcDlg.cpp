﻿
// mfcmfcDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "mfcmfc.h"
#include "mfcmfcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CmfcmfcDlg 대화 상자
CmfcmfcDlg::CmfcmfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, checkbox1);
	DDX_Control(pDX, IDC_CHECK2, checkbox2);
	DDX_Control(pDX, IDC_CHECK3, checkbox3);
	DDX_Control(pDX, IDC_CHECK4, checkbox4);
	DDX_Control(pDX, IDC_CHECK5, checkbox5);
	DDX_Control(pDX, IDC_CHECK6, checkbox6);
	DDX_Control(pDX, IDC_EDIT1, edit1);
}

BEGIN_MESSAGE_MAP(CmfcmfcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CmfcmfcDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CmfcmfcDlg 메시지 처리기

BOOL CmfcmfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	checkbox1.SetCheck(BST_UNCHECKED);
	checkbox2.SetCheck(BST_UNCHECKED);
	checkbox3.SetCheck(BST_UNCHECKED);
	checkbox4.SetCheck(BST_UNCHECKED);
	checkbox5.SetCheck(BST_UNCHECKED);
	checkbox6.SetCheck(BST_UNCHECKED);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CmfcmfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CmfcmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CmfcmfcDlg::OnBnClickedButton1()
{
	CString szMsg;

	szMsg.Append(_T("\n업데이트 적용"));
	
	
	if (checkbox1.GetCheck() == BST_CHECKED) // 체크박스1 체크여부 소프트웨어 차단
	{
		KEEPER_MANAGER->Get_ProcessMng()->checkThread();	// 소프트웨어 감시 스레드 실행상태 체크
	}
	else {
		KEEPER_MANAGER->Get_ProcessMng()->suspendThread();	// 스레드 멈춤
	}

	if (checkbox2.GetCheck() == BST_CHECKED) // 체크박스2 체크여부 웹사이트 차단
	{
		wstring process_name = L"chrome.exe";
		//wstring dll_name = L".\\HookDLL.dll";
		//wstring dll_name = L"C:\\Users\\USER\\Desktop\\MfcStudy\\mfcmfc\\x64\\Debug\\HookDLL.dll"; // 노트북 주소
		wstring dll_name = L"C:\\Users\\wx94\\Desktop\\DLPStudy\\mfcmfc\\x64\\Debug\\HookDLL.dll"; // 데탑 주소

		if (KEEPER_MANAGER->Get_InjectMng()->Process_name_to_pid(process_name)) {
			KEEPER_MANAGER->Get_InjectMng()->Dll_injection(dll_name);
		}

		//KEEPER_MANAGER->Get_WebsiteMng()->checkThread();	// 웹사이트 감시 스레드 실행상태 체크
	}
	else {
		//KEEPER_MANAGER->Get_WebsiteMng()->suspendThread();	// 스레드 멈춤
	}

	if (checkbox3.GetCheck() == BST_CHECKED) // 체크박스3 체크여부 검사
	{
		// path를 기준으로 모든 디렉토리 , 파일 리스트 찾기
		string path = "C:\\Users\\USER\\Desktop\\*";
		KEEPER_MANAGER->Get_DirectoryMng()->FindFileList(path);
	}

	if (checkbox4.GetCheck() == BST_CHECKED)	// 체크박스 4 암호화 체크여부
	{
		//암호화
		KEEPER_MANAGER->Get_CryptoMng()->StartThread(true);
	}
	
	if (checkbox5.GetCheck() == BST_CHECKED) {	// 체크박스 5 복호화 체크여부
		//복호화
		KEEPER_MANAGER->Get_CryptoMng()->StartThread(false);
	}

	if (checkbox6.GetCheck() == BST_CHECKED)	// 체크박스 6 키차단 체크여부
	{
		// 키보드 HOOK 스레드 로드
		KEEPER_MANAGER->Get_HookMng()->checkThread();
	}
	else {
		KEEPER_MANAGER->Get_HookMng()->suspendThread();

		//CPrintDialog digPrint(FALSE);
		//if (IDOK == digPrint.DoModal())
		//{
		//	// 프린터 정보 얻
		//}
	}

	AfxMessageBox(szMsg);
}

//void CmfcmfcDlg::DllTest()
//{
//	HMODULE hDll = NULL;
//	PFN_HOOKSTART HookStart = NULL;
//	PFN_HOOKSTOP HookStop = NULL;
//	char ch = 0;
//
//	hDll = LoadLibraryA(DEF_DLL_NAME);
//
//	if (hDll == NULL)
//	{
//		cout << "dll load fail" << "\n";
//	}
//
//	HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
//	HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);
//
//	HookStart();
//	
//	cout << "q누르면 종료" << "\n";
//
//	while (true)
//	{
//		if (_getch() == 'q') break;
//	}
//	HookStop();
//
//	FreeLibrary(hDll);
//
//}
