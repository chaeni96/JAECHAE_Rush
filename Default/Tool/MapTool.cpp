// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "Include.h"
// CMapTool ��ȭ �����Դϴ�.

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

	// ����Ʈ �ڽ� ���� DC��� ����̱� ������ ȭ�� DC�� ���� �� �ִ�.
	CDC*		pDC = m_TileListBox.GetDC();

	// GetCount : ����Ʈ �ڽ��� �ִ� ����� ���� ��ȯ
	for (int i = 0; i < m_TileListBox.GetCount(); ++i)
	{
		m_TileListBox.GetText(i, strName);

		// ���� ���ڿ��� ���̸� �ȼ� ������ ��ȯ
		size = pDC->GetTextExtent(strName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_TileListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : ����Ʈ �ڽ��� ���η� ��ũ�� �� �� �ִ� �ִ� ������ ������ �Լ�
	if (iWidth > m_TileListBox.GetHorizontalExtent())
		m_TileListBox.SetHorizontalExtent(iWidth);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMapTool::OnTileListBox)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnTileListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.



	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);


	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MAX_STR] = L"";

	// DragQueryFile : ��ӵ� ������ ������ ������ �Լ�(0�� ���ؽ��� �ش��ϴ� ������ ��ü ��θ� ���´�)
	// �ι�° ���ڰ� 0xffffffff(-1)�� �����ϸ� ��ӵ� ������ ������ ��ȯ

	int			iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelative = CFileInfo::ConvertRelativePath(szFilePath);

		//PathFindFileName : ��� �� ���� �̸��� ����� ��� �����ϴ� �Լ�
		CString	strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());

		// ������ Ȯ���� ���� �����ϴ� �Լ�
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto	iter = m_TilePngImg.find(strFileName);

		if (iter == m_TilePngImg.end())
		{
			CImage*		pPngImg = new CImage;

			pPngImg->Load(strRelative);	// �ش� ����� �̹��� �ε�

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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Radio[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
