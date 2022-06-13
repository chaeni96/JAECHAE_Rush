#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	auto		iter = find_if(m_MapMultiTex.begin(), m_MapMultiTex.end(), [&](auto& MyPair)->bool
	{
		if (pStateKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_MapMultiTex.end())
		return nullptr;

	return iter->second[iCount];
}

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	// 경로의 길이는 절대 520byte를 넘어선 안 된다.
	TCHAR	szFullPath[MAX_PATH] = L"";


	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 구조체에 기록하는 함수
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			ERR_MSG(szFullPath);
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),	// 디바이스 컴 객체
			szFullPath,								// 텍스처 파일이 있는 경로
			pTexInfo->tImgInfo.Width,				// 텍스처 가로 사이즈
			pTexInfo->tImgInfo.Height,			// 텍스처 세로 사이즈
			pTexInfo->tImgInfo.MipLevels,			// 텍스처 밉 레벨
			0,										// 0인 경우 일반적인 텍스처, D3DUSAGE_RENDERTARGET 옵션을 통해 렌더타겟 용 텍스처로 생성할 수도 있다
			pTexInfo->tImgInfo.Format,			// 텍스처 포맷
			D3DPOOL_MANAGED,						// 텍스처를 어떤 메모리에 보관할 것인가
			D3DX_DEFAULT,							// 이미지 필터링 방식, 픽셀이 모자라거나 남으면 어떻게 할 것인가
			D3DX_DEFAULT,							// 밉 필터링 방식, 밉맵을 이용한 확대, 축소 시 필터링을 어떻게 할 것인가
			0,										// 제거할 색상, 0을 주면 컬러키 무효화, D2D부터는 제거할 색상을 알파블랜딩을 사용하기 때문에 Colorkey 사용 안함
			nullptr,								// 구조체 이미지 정보 기록(위에서 이미 값을 채웠음)
			nullptr,								// 팔레트 정보를 저장하는 구조체(안 쓴다)
			&(pTexInfo->pTexture))))
		{
			ERR_MSG(L"Multi Texture Create Failed");
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		m_MapMultiTex[pStateKey].push_back(pTexInfo);
	}	

	return S_OK;
}

void CMultiTexture::Release(void)
{
	for_each(m_MapMultiTex.begin(), m_MapMultiTex.end(), [](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), Safe_Delete<TEXINFO*>);
		MyPair.second.clear();	
	});

	m_MapMultiTex.clear();
}
