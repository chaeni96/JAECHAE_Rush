// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	CDevice::Get_Instance()->Render_Begin();

	// AfxGetMainWnd : 현재 쓰레드로부터 wnd를 반환한다.
	// 현재 쓰레드가 메인 쓰레드일 경우에 MainFrameWnd를 반환하게 되는데 만약 다른 쓰레드가 구동되고 있다면 잘못된 정보를 반환할 수 있다.
	//CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// AfxGetApp : 메인 쓰레드가 갖고 있는 현재 메인 app을 반환
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	// GetParentFrame : 현재 view를 둘러싸고 있는 상위 FrameWnd를 반환
	// CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());

	CToolView*		pView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	CTerrain*		pTerrain = pView->Get_Terrain();

	pTerrain->Mini_Render();


	CDevice::Get_Instance()->Render_End(m_hWnd);
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.
