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

HRESULT CTextureMgr::ReadImgPath(const wstring& wstrPath)
{
	wifstream		fin;
	fin.open(wstrPath, ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MIN_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		wstring		wstrCombined = L"";


		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MIN_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			int iCount = _ttoi(szCount);


			if (FAILED(InsertTexture(szPath, TEX_MULTI, szObjKey, szStateKey, iCount)))
			{
				ERR_MSG(szPath);
				return E_FAIL;
			}

		}

		fin.close();
	}
	return S_OK;
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
			ERR_MSG(pFilePath);
			return E_FAIL;
		}
		m_MapTex.emplace(pObjKey, pTexture);
	}

	else if (TEX_MULTI == eType)
		iter->second->InsertTexture(pFilePath, pStateKey, iCount);

	return S_OK;
}

void CTextureMgr::Release(void)
{
	for_each(m_MapTex.begin(), m_MapTex.end(), [](auto& MyPair) { Safe_Delete<CTexture*>(MyPair.second); });
	m_MapTex.clear();
}
