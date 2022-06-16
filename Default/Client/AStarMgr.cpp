#include "stdafx.h"
#include "AStarMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"

IMPLEMENT_SINGLETON(CAStarMgr)

CAStarMgr::CAStarMgr()
	: m_iStartIdx(0)
{
}


CAStarMgr::~CAStarMgr()
{
	Release();
}

void CAStarMgr::Astar_Start(const D3DXVECTOR3 & vStart, const D3DXVECTOR3 & vGoal)
{
	Release();

	CObj*		pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();

	// 출발 지점의 타일 인덱스를 구함
	m_iStartIdx = GetTileIndex(vStart);

	int	iGoalIdx = GetTileIndex(vGoal);

	if (0 > m_iStartIdx || 0 > iGoalIdx || m_iStartIdx >= TILEX * TILEY || iGoalIdx >= TILEX * TILEY)
		return;

	if (m_iStartIdx == iGoalIdx)
		return;

	if (0 != vecTile[iGoalIdx]->byOption)
		return;

	if (Make_Route(m_iStartIdx, iGoalIdx))
		Make_BestList(m_iStartIdx, iGoalIdx);

}
				// 현재 위치한 타일의 인덱스 값
bool CAStarMgr::Make_Route(int iStartIdx, int iGoalIdx)
{
	CObj*				pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&		vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();
	vector<list<TILE*>>& vecAdj = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecAdj();

	// queue 형식으로 맨 앞을 지운다
	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	// 현재 위치한 타일 인덱스는 도착지점이 아닌 것이기 때문에 closelist에 삽입된다
	m_CloseList.push_back(iStartIdx);

	// 인접해있는 이웃 타일들을 토대로 경로를 찾기 위한 반복문
	for (auto& pTile : vecAdj[iStartIdx])
	{
		// 찾은 길이 골지점에 도작했다면
		if (iGoalIdx == pTile->iIndex)
		{
			pTile->iParentIndex = iStartIdx;
			return true;
		}
		
		// 인접한 타일 중에 close나 open 리스트에 tile 인덱스가 있는지 조사
		if (false == Check_Close(pTile->iIndex) &&
			false == Check_Open(pTile->iIndex))
		{
			// 이전 타일의 인덱스를 저장
			pTile->iParentIndex = iStartIdx;

			// 두 리스트에 속해있지 않기 때문에 일단 이동할 수 있는 리스트로 판별
			m_OpenList.push_back(pTile->iIndex);
		}
	}

	// 더이상 찾을 길이 없다는 뜻
	if (m_OpenList.empty())
		return false;

	// 휴리스틱 = 최초 시작지점과의 거리 비용 + 골 지점과의 거리비용

	int	iOriginIdx = m_iStartIdx;

	m_OpenList.sort([&vecTile, &iGoalIdx, &iOriginIdx](int iTemp, int Sour) {

		D3DXVECTOR3	vPCost1 = vecTile[iOriginIdx]->vPos - vecTile[iTemp]->vPos;
		D3DXVECTOR3	vPCost2 = vecTile[iOriginIdx]->vPos - vecTile[Sour]->vPos;

		D3DXVECTOR3	vGCost1 = vecTile[iGoalIdx]->vPos - vecTile[iTemp]->vPos;
		D3DXVECTOR3	vGCost2 = vecTile[iGoalIdx]->vPos - vecTile[Sour]->vPos;

		float		fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);
		float		fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);

		return fCost1 < fCost2;

	});

	// queue 형식으로 너비 탐색을 재귀 형식을 계속 진행한다.
	return Make_Route(m_OpenList.front(), iGoalIdx);
}

void CAStarMgr::Make_BestList(int iStartIdx, int iGoalIdx)
{
	CObj*				pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&		vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();


	m_BestList.push_front(vecTile[iGoalIdx]);

	int	iRouteIdx = vecTile[iGoalIdx]->iParentIndex;

	while (true)
	{
		if (iRouteIdx == iStartIdx)
			break;

		// 부모 타일을 역추적하면서 앞에서부터 best를 채운다.
		m_BestList.push_front(vecTile[iRouteIdx]);
		iRouteIdx = vecTile[iRouteIdx]->iParentIndex;
	}
}

void CAStarMgr::Release(void)
{
	m_CloseList.clear();
	m_OpenList.clear();

	//for_each(m_BestList.begin(), m_BestList.end(), CDeleteObj());
	m_BestList.clear();
}


int CAStarMgr::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	CObj*		pTerrain = CObjMgr::Get_Instance()->Get_Terrain();

	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();

	for (size_t iIndex = 0; iIndex < vecTile.size(); ++iIndex)
	{
		if (Picking(vPos, iIndex))
		{
			return iIndex;
		}
	}
	// 피킹 실패 시 음수 반환
	return -1;
}

bool CAStarMgr::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	CObj*		pTerrain = CObjMgr::Get_Instance()->Get_Terrain();

	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();

	D3DXVECTOR3		vPoint[4] = {

		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x + (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f },
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x - (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f }

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

bool CAStarMgr::Check_Close(int iIndex)
{
	for (int& iCloseIdx : m_CloseList)
	{
		if (iIndex == iCloseIdx)
			return true;
	}

	return false;
}

bool CAStarMgr::Check_Open(int iIndex)
{
	for (int& iOpenIdx : m_OpenList)
	{
		if (iIndex == iOpenIdx)
			return true;
	}

	return false;
}
