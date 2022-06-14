#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "MyTerrain.h"
#include "ObjMgr.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 4)))
	{
		ERR_MSG(L"Stage Tile Image Insert failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Map/Map0.png", TEX_SINGLE, L"Map1")))
	{
		ERR_MSG(L"Cube Image Insert failed");
		return E_FAIL;
	}


	CObj*	pObj = new CMyTerrain;

	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();
	
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pObj);
	
	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{
	CObjMgr::Get_Instance()->Render();
}

void CStage::Release_Scene()
{
	
}
