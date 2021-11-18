
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

UINT ThreadWaitProcessKill(LPVOID aParam) {
	//프로세스 감시 스레드
	while (true) {
		Sleep(500);
		KEEPER_MANAGER->Get_ProcessMng()->ProcessLoad();
	}
	return 0;
}
UINT ThreadWaitWebsiteBlock(LPVOID aParm) {
	 //웹사이트 감시 스레드
	while (true)
	{
		Sleep(500);
		KEEPER_MANAGER->Get_WebsiteMng()->WidgetLoad();
	}
	return 0;
}
UINT ThreadWaitEncryption(LPVOID aParm) {
	KEEPER_MANAGER->Get_CryptoMng()->Encryption();
	return 0;
}
UINT ThreadWaitDecryption(LPVOID aParm) {
	KEEPER_MANAGER->Get_CryptoMng()->Decryption();
	return 0;
}

void CmfcmfcDlg::OnBnClickedButton1()
{
	CString szMsg;

	szMsg.Append(_T("\n업데이트 적용"));
	
	
	if (checkbox1.GetCheck() == BST_CHECKED) // 체크박스1 체크여부 소프트웨어 차단
	{
		checkSoftThread(); //스레드 실행상태 체크
	}
	else {
		if (KEEPER_MANAGER->Get_ProcessMng()->GetThread() != NULL)
			KEEPER_MANAGER->Get_ProcessMng()->GetThread()->SuspendThread(); // 프로세스 감시 스레드 멈춤
	}

	if (checkbox2.GetCheck() == BST_CHECKED) // 체크박스2 체크여부 웹사이트 차단
	{

		checkWebThread(); 
	}
	else {
		if (KEEPER_MANAGER->Get_WebsiteMng()->GetThread() != NULL)
			KEEPER_MANAGER->Get_WebsiteMng()->GetThread()->SuspendThread(); // 웹사이트 감시 스레드 멈춤
	}

	if (checkbox3.GetCheck() == BST_CHECKED) // 체크박스3 체크여부 검사
	{
		string path = "C:\\Users\\USER\\Desktop\\*";
		KEEPER_MANAGER->Get_DirectoryMng()->FindFileList(path);
	}

	if (checkbox4.GetCheck() == BST_CHECKED) // 체크박스4 체크여부 암호화
	{
		KEEPER_MANAGER->Get_CryptoMng()->StartCryptoThread(ThreadWaitEncryption);
	}
	
	if (checkbox5.GetCheck() == BST_CHECKED) {
		KEEPER_MANAGER->Get_CryptoMng()->StartCryptoThread(ThreadWaitDecryption);
	}

	AfxMessageBox(szMsg);
}

void CmfcmfcDlg::checkSoftThread()
{
	if (KEEPER_MANAGER->Get_ProcessMng()->GetThread() != NULL)
	{
		//AfxMessageBox(TEXT("차단 감시 스레드가 이미 실행중입니다."));
		KEEPER_MANAGER->Get_ProcessMng()->GetThread()->ResumeThread();
	}
	else {
		cout << "스레드 실행" << "\n";
		KEEPER_MANAGER->Get_ProcessMng()->StartSwBlock(ThreadWaitProcessKill);
	}
	if (KEEPER_MANAGER->Get_ProcessMng()->GetThread() == NULL)
	{
		AfxMessageBox(TEXT("소프트웨어 감시 스레드 실행 실패"));
	}
}

void CmfcmfcDlg::checkWebThread()
{
	if (KEEPER_MANAGER->Get_WebsiteMng()->GetThread() != NULL)
	{
		KEEPER_MANAGER->Get_WebsiteMng()->GetThread()->ResumeThread();
	}
	else {
		KEEPER_MANAGER->Get_WebsiteMng()->StartWsBlock(ThreadWaitWebsiteBlock);
	}
	if (KEEPER_MANAGER->Get_WebsiteMng()->GetThread() == NULL)
	{
		AfxMessageBox(TEXT("웹사이트 감시 스레드 실행 실패"));
	}
}