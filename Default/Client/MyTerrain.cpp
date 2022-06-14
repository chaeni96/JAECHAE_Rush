#include "stdafx.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"


CMyTerrain::CMyTerrain()
{
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	if (FAILED(Load_Tile(L"../Data/Map1Tile.dat")))
		return E_FAIL;

	return S_OK;
}

int CMyTerrain::Update(void)
{
	/*D3DXVECTOR3		vMouse = ::Get_Mouse();

	if (0.f > vMouse.x)
		m_vScroll.x += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCX < vMouse.x)
		m_vScroll.x -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();
	
	if (0.f > vMouse.y)
		m_vScroll.y += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCY < vMouse.y)
		m_vScroll.y -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();*/

	return OBJ_NOEVENT;
}

void CMyTerrain::Late_Update(void)
{

}

void CMyTerrain::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	RECT	rc{};

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	int		iCullX = int(-m_vScroll.x) / TILECX;
	int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int		iTileCntX = WINCX / TILECX;
	int		iTileCntY = WINCY / (TILECY / 2);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	matWorld = matScale;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map1");

	if (nullptr == pTexInfo)
	{
		return;
	}

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// 그리고자 하는 텍스처
		nullptr, // 출력할 이미지 영역에 대한 rect 포인터, null인 경우 이미지의 0, 0 기준으로 출력
		&D3DXVECTOR3(0.f, 0.f, 0.f), // 출력할 이미지 중심 축에 대한 vec3 구조체 포인터, null인 경우 0, 0이 중심 좌표
		nullptr, // 위치 좌표에 대한 vec3 구조체 포인터, null인 경우 스크린 상 0,0 좌표에 출력
		D3DCOLOR_ARGB(255, 255, 255, 255)); //출력할 원본 이미지와 섞을 색상 값, 출력 시 섞은 색상이 반영, 0xffffffff를 넘겨주면 원본 색상 유지된 상태로 출력


	for(int i = iCullY; i < iCullY + iTileCntY; ++i)
	{
		for (int j = iCullX; j < iCullX + iTileCntX; ++j)
		{
			int	iIndex = i * TILEX + j;

			if(0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans, m_vecTile[iIndex]->vPos.x + m_vScroll.x,
				m_vecTile[iIndex]->vPos.y + m_vScroll.y,
				m_vecTile[iIndex]->vPos.z);

			matWorld = matScale *  matTrans;

			const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

			if (nullptr == pTexInfo)
				return;

			float		fX = pTexInfo->tImgInfo.Width / 2.f;
			float		fY = pTexInfo->tImgInfo.Height / 2.f;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(fX, fY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}	
	
	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

}

HRESULT CMyTerrain::Load_Tile(const TCHAR* pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	DWORD dwByte = 0;

	TILE* pTile = nullptr;

	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete<TILE*>(pTile);
			break;
		}

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);


	return S_OK;
}
