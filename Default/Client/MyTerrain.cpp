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
