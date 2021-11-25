#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") // 콘솔 창 출력 코드

/*******************************
********* 네임 스페이스 *********
********************************/
using namespace std;

/*******************************
********* 외부 추가 헤더 ********
********************************/
#include <iostream>					// c++ 기본 입출력 헤더
#include <TlHelp32.h>				// 프로세스 스냅샷 정보 접근 헤더
#include <process.h>				// 프로세스 정보
#include <Windows.h>				// 윈도우의 핸들을 얻어 오는 헤더
#include <afxmt.h>					// 멀티 스레드 어플리케이션을 위한 동기화 객체 헤더
#include <vector>					// STL vector
#include <map>						// STL map
#include <string>					// 문자열 string 헤더
#include <memory>					// 스마트 포인터 및 메모리 관련 헤더
#include <algorithm>				// STL algorithm
#include <cctype>					// 대소문자변환
#include <UIAutomation.h>			// UI Automation 헤더
#include <UIAutomationClient.h>		// UI Automation Client 헤더
#include <comutil.h>				// 유틸 헤더
#include <io.h>						// 파일 관련 헤더
#include <wincrypt.h>				// 암호화 관련 헤더
#include <fstream>					// 파일 입출력 헤더
#include <filesystem>				// 파일 시스템 헤더'
#include <conio.h>

/*******************************
********* 사용자 헤더 ***********
********************************/
#include "util.h"					// 전역으로 쓸 유틸 함수들
#include "keeperManager.h"			// 키퍼 매니저 헤더
#include "uiAutomationManager.h"	// UI 오토메이션 매니저 헤더
#include "../UtilFunction/UtilFunctionAPI.h" // DLL API 함수 include
#pragma comment( lib, "UtilFunction.lib")
//#pragma comment( lib, "HookDLL.lib")

/*******************************
******* Singleton define *******
********************************/
#define KEEPER_MANAGER keeperManager::getSingleton()				// 키퍼 매니저
#define UIA_MANAGER uiAutomationManager::getSingleton()	// UI 오토메이션 매니저

/*******************************
********* 매크로 define ********
********************************/
#define SAFE_DELETE(p)		{if(p) {delete (p); (p) = NULL;}}		// 단일 메모리 해제
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = NULL;}}	// 배열 메모리 해제