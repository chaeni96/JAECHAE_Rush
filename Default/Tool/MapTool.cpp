// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "Include.h"
// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST2, m_TileListBox);
	DDX_Control(pDX, IDC_PICTURE2, m_TilePicture);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
}

void CMapTool::Horizontal_Scroll(void)
{
	CString		strName;
	CSize		size;

	int		iWidth = 0;

	// 리스트 박스 또한 DC기반 출력이기 때문에 화면 DC를 얻어올 수 있다.
	CDC*		pDC = m_TileListBox.GetDC();

	// GetCount : 리스트 박스에 있는 목록의 개수 반환
	for (int i = 0; i < m_TileListBox.GetCount(); ++i)
	{
		m_TileListBox.GetText(i, strName);

		// 현재 문자열의 길이를 픽셀 단위로 변환
		size = pDC->GetTextExtent(strName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_TileListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iWidth > m_TileListBox.GetHorizontalExtent())
		m_TileListBox.SetHorizontalExtent(iWidth);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMapTool::OnTileListBox)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnTileListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString		strSelectName;

	int iSelect = m_TileListBox.GetCurSel();

	m_TileListBox.GetText(iSelect, strSelectName);

	auto	iter = m_TilePngImg.find(strSelectName);

	if (iter == m_TilePngImg.end())
		return;

	m_TilePicture.SetBitmap(*(iter->second));


	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.



	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);


	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MAX_STR] = L"";

	// DragQueryFile : 드롭된 파일의 정보를 얻어오는 함수(0번 인텍스에 해당하는 파일의 전체 경로를 얻어온다)
	// 두번째 인자가 0xffffffff(-1)로 지정하면 드롭된 파일의 개수를 반환

	int			iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelative = CFileInfo::ConvertRelativePath(szFilePath);

		//PathFindFileName : 경로 중 파일 이름만 남기고 모두 제거하는 함수
		CString	strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());

		// 파일의 확장자 명을 제거하는 함수
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto	iter = m_TilePngImg.find(strFileName);

		if (iter == m_TilePngImg.end())
		{
			CImage*		pPngImg = new CImage;

			pPngImg->Load(strRelative);	// 해당 경로의 이미지 로드

			m_TilePngImg.insert({ strFileName, pPngImg });
			m_TileListBox.AddString(szFileName);
		}
	}

	Horizontal_Scroll();

	UpdateData(FALSE);
}


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Radio[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
