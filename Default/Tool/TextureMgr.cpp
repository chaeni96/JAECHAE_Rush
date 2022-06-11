#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto		iter = find_if(m_MapTex.begin(), m_MapTex.end(), CTag_Finder(pObjKey));

	if (iter == m_MapTex.end())
		return nullptr;	

	/*auto		iter = find_if(m_MapTex.begin(), m_MapTex.end(), [&](auto& MyPair)->bool 
	{
		if (pObjKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_MapTex.end())
	{
		return nullptr;
	}*/

	
	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::InsertTexture(const TCHAR * pFilePath, TEXTYPE eType, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto		iter = find_if(m_MapTex.begin(), m_MapTex.end(), CTag_Finder(pObjKey));

	if (iter == m_MapTex.end())
	{
		CTexture*	pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCount)))
		{
			AfxMessageBox(pFilePath);
			return E_FAIL;
		}
		m_MapTex.emplace(pObjKey, pTexture);
	}
	else
	{
		switch (eType)
		{
		case TEX_SINGLE:
			AfxMessageBox(pFilePath);
			return E_FAIL;
			break;


		case TEX_MULTI:
			if (FAILED(m_MapTex[pObjKey]->InsertTexture(pFilePath, pStateKey, iCount)))
			{
				AfxMessageBox(pFilePath);
				return E_FAIL;
			}
			break;
		}
	}

	return S_OK;
}

void CTextureMgr::Release(void)
{
	for_each(m_MapTex.begin(), m_MapTex.end(), [](auto& MyPair) { Safe_Delete<CTexture*>(MyPair.second); });
	m_MapTex.clear();
}
