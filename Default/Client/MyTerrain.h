#pragma once

#include "Obj.h"

class CMyTerrain :	public CObj
{
public:
	CMyTerrain();
	virtual ~CMyTerrain();

public:
	vector<TILE*>&			GetvecTile(void) { return m_vecTile; }
	vector<list<TILE*>>&	GetvecAdj(void) { return m_vecAdjacency; }

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT Load_Tile(const TCHAR* pFilePath);
	void			Ready_Adjacency(void);


private:
	vector<TILE*> m_vecTile;

	vector<list<TILE*>>	m_vecAdjacency;
};

