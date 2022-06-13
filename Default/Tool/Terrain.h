#pragma once

#include "Include.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void		Initialize(void);
	void		Update(void);
	void		Render(void);
	void		Mini_Render(void);
	void		Release(void);

public:
	void		Tile_Change(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	int			Get_TileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	vector<TILE*>& Get_VecTile(void) { return m_vecTile; }
	void Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }

	// d2d이기 때문에 z값을 매개 변수로 받을 필요가 없다
	void Set_Ratio(D3DXMATRIX* pOut, const float& _fX, const float& _fY, const float& fZ = 0.f);

private:
	vector<TILE*>			m_vecTile;
	CToolView*				m_pMainView = nullptr;

};

