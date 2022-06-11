#pragma once

#include "Include.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual const TEXINFO*	Get_Texture(const TCHAR* pStateKey = L"", const int& iCount = 0)PURE;

public:
	virtual HRESULT		InsertTexture(const TCHAR* pFilePath,	// 텍스처 파일 경로
									const TCHAR* pStateKey = L"", 
									const int& iCount = 0)PURE;

	virtual void		Release(void)PURE;
};

