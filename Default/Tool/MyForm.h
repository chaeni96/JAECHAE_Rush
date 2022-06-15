#pragma once
#include "afxwin.h"
#include "MapTool.h"
#include "UnitTool.h"
#include "PathFind.h"

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
	CFont m_Font;
	CMapTool m_MapTool;
	CUnitTool m_UnitTool;
	CPathFind	m_PathFind;

public:
	virtual void OnInitialUpdate();
	
	afx_msg void OnUnitTool();
	afx_msg void OnMapTool();
	afx_msg void OnPathFind();
};


