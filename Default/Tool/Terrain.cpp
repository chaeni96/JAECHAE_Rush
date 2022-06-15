#include "stdafx.h"
#include "Terrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"

CTerrain::CTerrain()
{
	m_vecTile.reserve(TILEX * TILEY);
}


CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Map/Map2.png", TEX_SINGLE, L"Map2")))
	{
		AfxMessageBox(L"Cube Image Insert failed");
		return;
	}


	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 4)))
	{
		AfxMessageBox(L"Tile Image Insert failed");
		return;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float		fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);
			float		fY = (TILECY / 2.f) * i;

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY, 0.f };
			pTile->byDrawID = 0;
			pTile->byOption = 0;

			pTile->iIndex = i * TILEX + j;
			pTile->iParentIndex = 0;

			m_vecTile.push_back(pTile);
		}
	}
}

void CTerrain::Update(void)
{
}

void CTerrain::Render(void)
{

	D3DXMATRIX	matWorld, matScale, matTrans;

	RECT	rc{};

	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	matWorld = matScale;

	Set_Ratio(&matWorld, fX, fY);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map2");

	if (nullptr == pTexInfo)
	{
		return;
	}

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// 그리고자 하는 텍스처
		nullptr, // 출력할 이미지 영역에 대한 rect 포인터, null인 경우 이미지의 0, 0 기준으로 출력
		&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지 중심 축에 대한 vec3 구조체 포인터, null인 경우 0, 0이 중심 좌표
		nullptr, // 위치 좌표에 대한 vec3 구조체 포인터, null인 경우 스크린 상 0,0 좌표에 출력
		D3DCOLOR_ARGB(255, 255, 255, 255)); //출력할 원본 이미지와 섞을 색상 값, 출력 시 섞은 색상이 반영, 0xffffffff를 넘겨주면 원본 색상 유지된 상태로 출력


	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0),
										 iter->vPos.y - m_pMainView->GetScrollPos(1),
										 iter->vPos.z);

		matWorld = matScale *  matTrans;

		Set_Ratio(&matWorld, fX, fY);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

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

void CTerrain::Mini_Render(void)
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x,
			iter->vPos.y,
			iter->vPos.z);

		matWorld = matScale *  matTrans;

		Set_Ratio(&matWorld, 0.7f, 0.7f);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

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

void CTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTerrain::Tile_Change(const D3DXVECTOR3 & vPos, const BYTE & byDrawID)
{
	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t iIndex = 0; iIndex < m_vecTile.size(); ++iIndex)
	{
		if (Picking_Dot(vPos, iIndex))
		{
			return iIndex;
		}
	}
	// 피킹 실패 시 음수 반환
	return -1;
}

bool CTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	// 12, 3, 6, 9 시 방향 순서로 위치를 잡음
	float	fGradient[4] = {

		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f),
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f)
	};

	D3DXVECTOR3		vPoint[4] = {

		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }

	};

	// y = ax + b
	// -b = ax - y
	// b = y - ax		

	float	fB[4] = {

		vPoint[0].y - fGradient[0] * vPoint[0].x,
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x

	};


	//0 == ax + b - y	// 직선 상에 있는 경우
	//0 > ax + b - y  // 직선 위에 있는 경우
	//0 < ax + b - y  // 직선 아래에 있는 경우

	bool	bCheck[4] = { false };

	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;

	if (0 > fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	if (0 > fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;

	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	D3DXVECTOR3		vPoint[4] = {

		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }

	};

	D3DXVECTOR3		vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3		vNormal[4] = {

		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }
	};

	D3DXVECTOR3	vMouseDir[4] = {

		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};
	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		// 양수인 경우 예각이어서 타일 외부에 피킹한 꼴인 셈
		if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
			return false;
	}

	return true;
}

void CTerrain::Set_Ratio(D3DXMATRIX * pOut, const float & _fX, const float & _fY, const float & fZ)
{
	pOut->_11 *= _fX;
	pOut->_21 *= _fX;
	pOut->_31 *= _fX;
	pOut->_41 *= _fX;

	pOut->_12 *= _fY;
	pOut->_22 *= _fY;
	pOut->_32 *= _fY;
	pOut->_42 *= _fY;
}
