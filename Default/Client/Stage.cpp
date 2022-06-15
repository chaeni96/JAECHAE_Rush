#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "MyTerrain.h"
#include "ObjMgr.h"
#include "Player.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{

	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"ImgPath Load failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Map/Map2.png", TEX_SINGLE, L"Map2")))
	{
		ERR_MSG(L"Cube Image Insert failed");
		return E_FAIL;
	}


	CObj*	pObj = new CMyTerrain;

	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();
	
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pObj);
	

	pObj = new CPlayer;

	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, pObj);

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
