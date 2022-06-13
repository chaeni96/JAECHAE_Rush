
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "MiniView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)


	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pDevice(CDevice::Get_Instance())
	//, m_pSingle(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	//Safe_Delete(m_pSingle);

}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// SetScrollSizes: 스크롤 바의 사이즈를 지정하는 CScrollView의 멤버 함수
	// MM_TEXT : 픽셀 단위로 크기를 조정하겠다는 옵션
	// CSize : 스크롤 가로, 세로 사이즈

	//CSize : mfc에서 제공하는 사이즈와 관련된 데이터를 관리하는 클래스

	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY * TILEY / 2));

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// AfxGetMainWnd : 현재 메인 윈도우를 반환하는 전역 함수
	// 부모타입을 반환하기에 자식 타입으로 형 변환하여 사용
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT		rcWnd{};

	// GetWindowRect : 현재 윈도우의 RECT 정보를 얻어와 기록해주는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 인수로 RECT의 정보를 기록해주는 함수
	// 현재 0, 0 좌표 기준으로 다시 RECT를 조정하고 있다.
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	//GetClientRect : 현재 view창의 RECT 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	// 메인 창과 View창의 가로, 세로 사이즈를 통해 프레임 가로, 세로의 인터벌을 구함

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	//SetWindowPos : 인수대로 새롭게 윈도우 위치와 크기를 조정하는 함수
	// 1인자 : 순수 변경할 것인가?
	// SWP_NOZORDER : 현재 순서를 유지하겠다는 플래그

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);


	g_hWnd = m_hWnd;

	if (FAILED(m_pDevice->Initialize()))
	{
		AfxMessageBox(L"m_pDevice 생성 실패");
		return;
	}

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->Set_MainView(this);

}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	m_pDevice->Render_Begin();

	m_pTerrain->Render();

	m_pDevice->Render_End();


}

#pragma region 불필요
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}


BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기
#pragma endregion 불필요

// CToolView 인쇄


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.



	CScrollView::OnLButtonDown(nFlags, point);

	m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f), 1);

	// Invalidate : 호출 시 윈도우에 WM_PAINT와 WM_ERASEBKGND 메세지를 발생시킴, 이때 OnDraw 함수를 다시 한 번 호출하게 된다.

	/*매개 변수가 FALSE 일 때 : WM_PAINT만 발생
	매개 변수가 TRUE 일 때 : WM_PAINT와 WM_ERASEBKGND 동시 발생

	WM_ERASEBKGND  : 배경을 지우는 메세지*/

	Invalidate(FALSE);


	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);

}


void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();


	Safe_Delete<CTerrain*>(m_pTerrain);
	CTextureMgr::Get_Instance()->Destroy_Instance();
	m_pDevice->Destroy_Instance();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
