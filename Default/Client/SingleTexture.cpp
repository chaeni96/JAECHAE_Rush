#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const TCHAR* pFilePath,
									const TCHAR* pStateKey /*= L""*/, 
									const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 구조체에 기록하는 함수
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		ERR_MSG(pFilePath);
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),	// 디바이스 컴 객체
											pFilePath,								// 텍스처 파일이 있는 경로
											m_pTexInfo->tImgInfo.Width,				// 텍스처 가로 사이즈
											m_pTexInfo->tImgInfo.Height,			// 텍스처 세로 사이즈
											m_pTexInfo->tImgInfo.MipLevels,			// 텍스처 밉 레벨
											0,										// 0인 경우 일반적인 텍스처, D3DUSAGE_RENDERTARGET 옵션을 통해 렌더타겟 용 텍스처로 생성할 수도 있다
											m_pTexInfo->tImgInfo.Format,			// 텍스처 포맷
											D3DPOOL_MANAGED,						// 텍스처를 어떤 메모리에 보관할 것인가
											D3DX_DEFAULT,							// 이미지 필터링 방식, 픽셀이 모자라거나 남으면 어떻게 할 것인가
											D3DX_DEFAULT,							// 밉 필터링 방식, 밉맵을 이용한 확대, 축소 시 필터링을 어떻게 할 것인가
											0,										// 제거할 색상, 0을 주면 컬러키 무효화, D2D부터는 제거할 색상을 알파블랜딩을 사용하기 때문에 Colorkey 사용 안함
											nullptr,								// 구조체 이미지 정보 기록(위에서 이미 값을 채웠음)
											nullptr,								// 팔레트 정보를 저장하는 구조체(안 쓴다)
											&(m_pTexInfo->pTexture))))
	{
		ERR_MSG(L"Single Texture Create Failed");
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}


	// D3DPOOL_DEFAULT : 가장 적합한 메모리에 보관, 보통 그래픽 메모리를 의미, 동적 버퍼
	// D3DPOOL_MANAGED : DX가 자원을 관리하며 그래픽 메모리를 사용하지만 RAM에 백업, 정적 버퍼
	
	// D3DPOOL_SYSTEMMEM : 시스템 메모리에 보관
	// D3DPOOL_SCRATCH : 시스템 메모리를 사용하지만 DX 장치로부터 접근할 수 없음


	return S_OK;
}

void CSingleTexture::Release(void)
{
	m_pTexInfo->pTexture->Release();
	Safe_Delete(m_pTexInfo);
}
