// PathFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"

// CPathFind 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{

}

CPathFind::~CPathFind()
{
	for_each(m_PathList.begin(), m_PathList.end(), CDeleteObj());
	m_PathList.clear();
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON3, &CPathFind::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnLoadData)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFind 메시지 처리기입니다.


void CPathFind::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CPathFind::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	wofstream		fout;
	fout.open(L"../Data/ImgPath.txt", ios::out);

	if (!fout.fail()) // 파일 개방 성공
	{
		for (auto& pImgPath : m_PathList)
		{
			fout << pImgPath->wstrObjKey << L"|" << pImgPath->wstrStateKey << L"|" << pImgPath->iCount << L"|" << pImgPath->wstrPath << endl;
		}

		fout.close();
	}

	// WinExec : 윈도우의 기본 프로그램을 실행시켜주는 함수
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);

}


void CPathFind::OnLoadData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wifstream		fin;
	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MIN_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		wstring		wstrCombined = L"";

		m_ListBox.ResetContent(); // 모든 항목 제거

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MIN_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;
			m_ListBox.AddString(wstrCombined.c_str());
		}

		fin.close();
	}

	UpdateData(FALSE);

	// WinExec : 윈도우의 기본 프로그램을 실행시켜주는 함수
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	TCHAR		szFilePath[MAX_PATH] = L"";

	int			iFileCnt = DragQueryFile(hDropInfo, -1, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFilePath, m_PathList);
	}

	m_ListBox.ResetContent();

	wstring		wstrCombined = L"";

	TCHAR		szBuf[MIN_STR] = L"";

	for (auto& pImgPath : m_PathList)
	{
		//_itow_s : 정수를 문자열로 변환
		// 3인자 : 10진수로 변환하겠다는 의미
		_itow_s(pImgPath->iCount, szBuf, 10);

		wstrCombined = pImgPath->wstrObjKey + L"|" + pImgPath->wstrStateKey + L"|" + szBuf + L"|" + pImgPath->wstrPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	UpdateData(FALSE);
}
