#pragma once
#include "afxwin.h"


// CMapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void		Horizontal_Scroll(void);

public:
	afx_msg void OnTileListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSaveData();

	virtual BOOL OnInitDialog();

public:
	CListBox m_TileListBox;
	CStatic m_TilePicture;
	CButton m_Radio[3];

	map<CString, CImage*> m_TilePngImg;

	int	m_iDrawID;

};
