
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "TextureMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)


	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
	: m_pDevice(CDevice::Get_Instance())
	//, m_pSingle(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
	//Safe_Delete(m_pSingle);
	CTextureMgr::Get_Instance()->Destroy_Instance();
	m_pDevice->Destroy_Instance();
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	g_hWnd = m_hWnd;

	if (FAILED(m_pDevice->Initialize()))
	{
		AfxMessageBox(L"m_pDevice ���� ����");
		return;
	}

	//m_pSingle = new CSingleTexture;
	//m_pSingle->InsertTexture(L"../Texture/Cube.png");


	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"Cube Image Insert failed");
		return;
	}

	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 36)))
	{
		AfxMessageBox(L"Tile Image Insert failed");
		return;
	}

}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	//pDC->Rectangle(100, 100, 200, 200);


	D3DXMATRIX		matWorld, matScale, matRotZ, matTrans;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(45.f));
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

	matWorld = matScale *  matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	/*float		fX = m_pSingle->Get_Texture()->tImgInfo.Width / 2.f;
	float		fY = m_pSingle->Get_Texture()->tImgInfo.Height / 2.f;*/

	//const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Cube");

	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", 3);
	
	if (nullptr == pTexInfo)
		return;
	
	float		fX = pTexInfo->tImgInfo.Width / 2.f;
	float		fY = pTexInfo->tImgInfo.Height / 2.f;

	m_pDevice->Render_Begin();

	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture,	// �׸����� �ϴ� �ؽ�ó
								nullptr, // ����� �̹��� ������ ���� rect ������, null�� ��� �̹����� 0, 0 �������� ���
								&D3DXVECTOR3(fX, fY, 0.f), // ����� �̹��� �߽� �࿡ ���� vec3 ����ü ������, null�� ��� 0, 0�� �߽� ��ǥ
								nullptr, // ��ġ ��ǥ�� ���� vec3 ����ü ������, null�� ��� ��ũ�� �� 0,0 ��ǥ�� ���
								D3DCOLOR_ARGB(255, 255, 255, 255)); //����� ���� �̹����� ���� ���� ��, ��� �� ���� ������ �ݿ�, 0xffffffff�� �Ѱ��ָ� ���� ���� ������ ���·� ���

	m_pDevice->Render_End();

}

#pragma region ���ʿ�
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}


BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����
#pragma endregion ���ʿ�

// CToolView �μ�


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.



	CView::OnLButtonDown(nFlags, point);
}
