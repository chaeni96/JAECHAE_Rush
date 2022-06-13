#pragma once
#include "afxwin.h"
#include "Include.h"

// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnAdd_Charactor();
	afx_msg void OnListBox();
	afx_msg void OnDeleteData();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
public:

	//value
	int m_iLevel; // ���� : 1~ 99
	int m_iHp;
	int m_iAttack;
	float m_fSpeed;
	CString m_strName;

	//control
	CListBox m_ListBox;

	//general
	map<CString, UNITDATA*>			m_MapUnitData;

	afx_msg void OnDestroy();
	CComboBox m_ComboControl;
	CString m_ComboStr;
	virtual BOOL OnInitDialog();
	afx_msg void OnComboSelect();
	CStatic m_Picture;
};
