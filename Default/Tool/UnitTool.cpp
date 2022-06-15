// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strName(_T(""))
	, m_iLevel(0)
	, m_iHp(0)
	, m_iAttack(0)
	, m_fSpeed(0)
	, m_ComboStr(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_iLevel);
	DDV_MinMaxInt(pDX, m_iLevel, 0, 99);
	DDX_Text(pDX, IDC_EDIT3, m_iHp);
	DDX_Text(pDX, IDC_EDIT4, m_iAttack);
	DDX_Text(pDX, IDC_EDIT5, m_fSpeed);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_COMBO1, m_ComboControl);
	DDX_CBString(pDX, IDC_COMBO1, m_ComboStr);
	DDX_Control(pDX, IDC_PIC, m_Picture);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnAdd_Charactor)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnDeleteData)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnLoadData)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CUnitTool::OnComboSelect)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.


void CUnitTool::OnAdd_Charactor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	UNITDATA*		pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iAttack = m_iAttack;
	pUnit->iHp = m_iHp;
	pUnit->iLevel = m_iLevel;
	pUnit->fSpeed = m_fSpeed;


	// AddString : ����Ʈ �ڽ��� ���ڿ��� �߰� ��
	m_ListBox.AddString(pUnit->strName);

	m_MapUnitData.insert({ pUnit->strName, pUnit });

	UpdateData(FALSE);
}


void CUnitTool::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CString		strFindName;

	// GetCurSel : ���� ����Ʈ �ڽ����� ���õ� ���� �ε��� ���� ��ȯ
	int iSelect = m_ListBox.GetCurSel();

	// GetText : ���� �ε����� �ش��ϴ� ���ڿ��� ����Ʈ �ڽ��κ��� ����
	m_ListBox.GetText(iSelect, strFindName);

	auto	iter = m_MapUnitData.find(strFindName);

	if (iter == m_MapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iAttack = iter->second->iAttack;
	m_iHp = iter->second->iHp;
	m_iLevel = iter->second->iLevel;
	m_fSpeed = iter->second->fSpeed;

	UpdateData(FALSE);
}


void CUnitTool::OnDeleteData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CString		strFineName = L"";

	int		iSelect = m_ListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_ListBox.GetText(iSelect, strFineName);

	auto&	iter = m_MapUnitData.find(strFineName);

	if (iter == m_MapUnitData.end())
		return;

	Safe_Delete<UNITDATA*>(iter->second);
	m_MapUnitData.erase(strFineName);

	// �ε����� �ش��ϴ� ����Ʈ �ڽ� �� ���ڿ��� ����
	m_ListBox.DeleteString(iSelect);

	UpdateData(FALSE);
}


void CUnitTool::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog		Dlg(FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPath[MAX_PATH] = L"";

	// ���� ������Ʈ ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_PATH, szPath);


	// ��ü ��ο��� ���� �̸��� �߶��ִ� �Լ�, ���� ��� �� ���ϸ��� ���ٸ� ���� ������ �������� �߶󳽴�.
	PathRemoveFileSpec(szPath);

	// data �������� �߶� ��ο� �ռ�
	lstrcat(szPath, L"\\Data");

	// ��ȭ���ڸ� ������ �� ���̴� �⺻ ��η� ����
	Dlg.m_ofn.lpstrInitialDir = szPath;


	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : ���õ� ��θ� ��ȯ
		CString		str = Dlg.GetPathName();

		// GetString ���� ���ڿ��� ��ȯ�ϴ� �Լ�
		const TCHAR* pGetPath = str.GetString();

		HANDLE		hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwStrByte = 0;
		DWORD	dwByte = 0;

		for (auto& Pair : m_MapUnitData)
		{
			// key�� ����
			dwStrByte = sizeof(TCHAR) * (Pair.first.GetLength() + 1);

			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, Pair.first.GetString(), dwStrByte, &dwByte, nullptr);

			// value ����

			WriteFile(hFile, &(Pair.second->iHp), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(Pair.second->iAttack), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(Pair.second->iLevel), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(Pair.second->fSpeed), sizeof(float), &dwByte, nullptr);

		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPath[MAX_PATH] = L"";

	// ���� ������Ʈ ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_PATH, szPath);


	// ��ü ��ο��� ���� �̸��� �߶��ִ� �Լ�, ���� ��� �� ���ϸ��� ���ٸ� ���� ������ �������� �߶󳽴�.
	PathRemoveFileSpec(szPath);

	// data �������� �߶� ��ο� �ռ�
	lstrcat(szPath, L"\\Data");

	// ��ȭ���ڸ� ������ �� ���̴� �⺻ ��η� ����
	Dlg.m_ofn.lpstrInitialDir = szPath;


	if (IDOK == Dlg.DoModal())
	{
		for (auto& Pair : m_MapUnitData)
			delete Pair.second;

		m_MapUnitData.clear();

		// ������ ����Ʈ�ڽ� ����� �ʱ�ȭ
		m_ListBox.ResetContent();


		// GetPathName : ���õ� ��θ� ��ȯ
		CString		str = Dlg.GetPathName();

		// GetString ���� ���ڿ��� ��ȯ�ϴ� �Լ�
		const TCHAR* pGetPath = str.GetString();

		HANDLE		hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwStrByte = 0;
		DWORD		dwByte = 0;
		UNITDATA	tData{};

		while (true)
		{
			// Key �� �ε�
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = nullptr;
			pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value �� �ε�

			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iLevel), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.fSpeed), sizeof(float), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[] pName;
				break;
			}

			UNITDATA*	pUnit = new UNITDATA;

			pUnit->strName = pName;
			delete[] pName;

			pUnit->iHp = tData.iHp;
			pUnit->iAttack = tData.iAttack;
			pUnit->iLevel = tData.iLevel;
			pUnit->fSpeed = tData.fSpeed;

			m_MapUnitData.insert({ pUnit->strName, pUnit });

			m_ListBox.AddString(pUnit->strName);
		}

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CUnitTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	for_each(m_MapUnitData.begin(), m_MapUnitData.end(), CDeleteMap());
	m_MapUnitData.clear();
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_ComboControl.AddString(_T("DarkSlayer"));
	m_ComboControl.AddString(_T("Troll"));
	m_ComboControl.AddString(_T("Bat"));
	m_ComboControl.AddString(_T("Spider"));
	m_ComboControl.AddString(_T("Golem"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUnitTool::OnComboSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_strName = m_ComboStr;

	SetDlgItemText(IDC_EDIT1, m_ComboStr);

	int iIndex = m_ComboControl.GetCurSel();

	HBITMAP		hBitMap[5] =
	{
		(HBITMAP)LoadImage(nullptr, L"../Texture/Stage/Thumbnail/0.bmp", IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE | LR_CREATEDIBSECTION),
		(HBITMAP)LoadImage(nullptr, L"../Texture/Stage/Thumbnail/1.bmp", IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE | LR_CREATEDIBSECTION),
		(HBITMAP)LoadImage(nullptr, L"../Texture/Stage/Thumbnail/2.bmp", IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE | LR_CREATEDIBSECTION),
		(HBITMAP)LoadImage(nullptr, L"../Texture/Stage/Thumbnail/3.bmp", IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE | LR_CREATEDIBSECTION),
		(HBITMAP)LoadImage(nullptr, L"../Texture/Stage/Thumbnail/4.bmp", IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE | LR_CREATEDIBSECTION)
	};

	m_Picture.SetBitmap(hBitMap[iIndex]);


	UpdateData(FALSE);

}
