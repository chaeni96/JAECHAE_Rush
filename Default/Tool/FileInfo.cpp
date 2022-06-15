#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L"";

	TCHAR		szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);


	//PathRelativePathTo : szCurDirPath에서 strFullPath로 가는 상대 경로를 구해서 szRelativePath에 저장하는 함수, 단 같은 드라이브 내에서만 변환 가능
	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathList)
{
	// CFileFind : mfc에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind		Find;

	// 경로 상의 모든 파일(*.*)을 찾기 위해 경로를 재조정
	wstring			wstrFilePath = wstrPath + L"\\*.*";


	// FindFile : 주어진 경로에 파일의 유무를 판단하는 함수
	// 존재하지 않으면 false, 파일이 있으면 true를 리턴

	// 만약 파일의 유무를 확인하고 실제 그 파일에 접근하고 싶을 때는 FindNextFile 함수를 무조건 호출해야 한다.

	// 매개변수의 경로로 *.*가 붙은 경우 해당 경로의 하위 모든 파일을 찾는 것으로 해석하며
	// 이 때, FindNextFile함수를 호출하면 여러 파일들 중 가장 첫 번째 파일에 접근할 수 있다.

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : 동일 경로 안에서 다음 파일을 찾는다. 더이상 찾을게 없거나 마지막 대상을 찾으면 0을 반환한다.

		bContinue = Find.FindNextFile();

		// 찾은 파일 이름이 .이나 ..이면 찾고자 하는 파일이 아니기 때문에 새로 탐색을 지시한다.
		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath :  Find객체가 지닌 현재 찾은 파일의 경로를 얻어오는 함수
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}

		else
		{
			// 찾은 파일이 숨김 형태의 시스템 파일인 경우 건너뜀
			if (Find.IsSystem())
				continue;

			IMGPATH*	pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());

			// 현재 경로에서 가장 마지막 경로를 잘라냄
			PathRemoveFileSpec(szPathBuf);

			// 현재 폴더 안의 파일의 개수를 계산한다
			pImgPath->iCount = CFileInfo::DirFileCount(szPathBuf);

			//GetFileTitle : 현재 Find가 가리키는 파일의 파일명만 얻어오는 함수
			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(시작, 끝) : 시작에서부터 끝까지 해당하는 문자열을 얻어옴
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			TCHAR	szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);

			// PathCombine(1, 2, 3)
			// 2와 3을 이어붙어 완성된 경로를 1에 저장, 2와 3사이에 \\가 자동으로 삽입된채 1에 저장

			// D:\유준환\129기\Frame129\Texture\Stage\Effect\BossMultiAttack\\BossMultiAttack%d.png
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());

			// 상대경로 변환시켜서 경로 정보를 대입해준다.
			//..\Frame129\Texture\Stage\Effect\BossMultiAttack\\BossMultiAttack%d.png
			pImgPath->wstrPath = CFileInfo::ConvertRelativePath(szBuf);

			//..\Frame129\Texture\Stage\Effect\BossMultiAttack
			PathRemoveFileSpec(szBuf);

			// PathFindFileName : 파일명을 찾거나 가장 마지막 폴더명을 찾아내는 함수
			pImgPath->wstrStateKey = PathFindFileName(szBuf);

			//..\Frame129\Texture\Stage\Effect
			PathRemoveFileSpec(szBuf);

			pImgPath->wstrObjKey = PathFindFileName(szBuf);

			rPathList.push_back(pImgPath);

			bContinue = 0;
		}
	}
}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	CFileFind		Find;

	wstring			wstrFilePath = wstrPath + L"\\*.*";

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	int iFileCount = 0;

	while (bContinue)
	{

		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCount;
	}

	return iFileCount;
}
