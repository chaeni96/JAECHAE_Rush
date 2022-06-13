#pragma once
#include "afxwin.h"
#include "MapTool.h"
#include "UnitTool.h"



// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CFont		m_Font;
	CMapTool m_MapTool;

public:
	afx_msg void OnMapTool();
	virtual void OnInitialUpdate();
	CUnitTool m_UnitTool;
	afx_msg void OnUnitTool();
};


