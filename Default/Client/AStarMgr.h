#pragma once

#include "Include.h"

class CAStarMgr
{
	DECLARE_SINGLETON(CAStarMgr)

private:
	CAStarMgr();
	~CAStarMgr();

public:
	list<TILE*>&	Get_BestList(void) { return m_BestList; }

public:
	void			Astar_Start(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);
	void			Release(void);

private:
	bool			Make_Route(int iStartIdx, int iGoalIdx);
	void			Make_BestList(int iStartIdx, int iGoalIdx);


private:
	int				GetTileIndex(const D3DXVECTOR3& vPos);
	bool			Picking(const D3DXVECTOR3& vPos, const int& iIndex);
private:
	bool			Check_Close(int iIndex);
	bool			Check_Open(int iIndex);

private:
	list<int>			m_OpenList;
	list<int>			m_CloseList;
	list<TILE*>			m_BestList;

	int					m_iStartIdx;

};

