#include "framework.h"

#include "Level_Zero.h"

#include "GameInstance.h"
#include "Model.h"

CLevel_Zero::CLevel_Zero(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel(_pDevice, _pContext)
{
}

HRESULT CLevel_Zero::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Zero::Tick(_float _fTimeDelta)
{
	if (!m_bIsBinarying && GetKeyState('Q') & 0x8000)
	{
		m_bIsBinarying = true;
		SetWindowText(g_hWnd, TEXT("Bynarying..."));

		FindFBX(TEXT("../Bin/Resource/Models/Skyrim/"));

		SetWindowText(g_hWnd, TEXT("Bynary Complete!"));
	}

	return S_OK;
}

HRESULT CLevel_Zero::LateTick(_float _fTimeDelta)
{
	if(!m_bIsBinarying)
		SetWindowText(g_hWnd, TEXT("Current Level : Zero."));

	return S_OK;
}

HRESULT CLevel_Zero::FindFBX(const wstring& _wStrFolderPath)
{
	if (visitedDirectories.count(_wStrFolderPath) > 0)
		return S_OK;

	visitedDirectories.insert(_wStrFolderPath);

	for (const auto& entry : filesystem::directory_iterator(_wStrFolderPath))
	{
		if (filesystem::is_directory(entry))
		{
			FindFBX(entry.path());
		}
		else if (filesystem::is_regular_file(entry))
		{
			if (fileExtension == entry.path().extension() || fileExtension2 == entry.path().extension())
			{
				if (FAILED(ThrowFBX(entry.path())))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CLevel_Zero::ThrowFBX(const wstring& _wStrFolderPath)
{
	HANDLE fileSearch;
	WIN32_FIND_DATA wfd;
	const wstring& cStrPBXPath = _wStrFolderPath;
	CModel::MODEL_TYPE eModelType;

	fileSearch = FindFirstFile(cStrPBXPath.c_str(), &wfd);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 찾는 파일이 있다면, 
	if (fileSearch != INVALID_HANDLE_VALUE)
	{
		// Anim, NonAnim 판별하기
		wstring animDir = L"/Anim\\";
		wstring nonAnimDir = L"/NonAnim\\";

		size_t animPos = cStrPBXPath.find(animDir);
		size_t nonAnimPos = cStrPBXPath.find(nonAnimDir);

		if (animPos != wstring::npos) {
			wstring extractedPath = cStrPBXPath.substr(animPos + animDir.length());
			eModelType = CModel::TYPE_ANIM;
		}
		else if (nonAnimPos != wstring::npos) {
			wstring extractedPath = cStrPBXPath.substr(nonAnimPos + nonAnimDir.length());
			eModelType = CModel::TYPE_NONANIM;
		}
		else
		{
			MSG_BOX("Anim, NonAnim 확인 불가.");
			return E_FAIL;
		}

		/* 세이브 경로 지정 */
		string StrSavePath = "../Bin/Resource/BinaryFBX/";

		wstring originalPath = cStrPBXPath;
		wstring pathToRemove = TEXT("../Bin/Resource/Models/Skyrim/");

		size_t pos = originalPath.find(pathToRemove);

		if (pos != wstring::npos) {
			originalPath.erase(pos, pathToRemove.length());
		}

		wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
		// FBX 경로 변환
		string strPath = converter.to_bytes(cStrPBXPath);
		// 잘라낸 저장 경로 변환 
		string  ConvertPathToRemove = converter.to_bytes(originalPath);

		// 잘라낸 저장 경로 기존 저장 경로에 합치기.
		StrSavePath += ConvertPathToRemove;

		// 확장자 자르기
		size_t lastDotPos = StrSavePath.find_last_of('.');
		if (lastDotPos != string::npos) {
			// 확장자 추출
			StrSavePath.substr(lastDotPos + 1);
			// 확장자 제외한 파일 이름 추출
			StrSavePath = StrSavePath.substr(0, lastDotPos);
		}
		else {
			MSG_BOX("확장자가 없거나 확인 불가.");
			return E_FAIL;
		}

		// 확장자 추가
		StrSavePath += ".bin";

		/* 폴더 경로 생성(객체 파일을 폴더 별로 저장하기 위함) */
		/* 파일명 자르고 폴더까지만 남기기 */
		string StrPath = filesystem::path(StrSavePath).remove_filename().string(); 
		filesystem::path directory = filesystem::path(StrPath).parent_path();

		// 폴더가 존재하지 않는 경우, 폴더를 생성
		if (!filesystem::exists(directory)) {
			/* 예외 처리*/
			if (!filesystem::create_directories(directory)) {
				MSG_BOX("폴더 생성 실패.");
				return E_FAIL;
			}
		}

		/* FBX 바이너리화 시작 */
		ofstream fileStream(StrSavePath, ios::binary);
		if (fileStream.is_open())
		{
			pGameInstance->Binary_OutFile(fileStream, strPath.c_str(), eModelType);

			fileStream.close();
		}
		else
			MessageBox(g_hWnd, L"파일을 저장하는 중 오류가 발생했습니다.", L"저장 오류", MB_OK | MB_ICONERROR);

		Safe_Release(pGameInstance);
		// 파일 찾기 핸들 값 닫기	
		FindClose(fileSearch);
	}

	return S_OK;
}

CLevel_Zero* CLevel_Zero::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_Zero* pInstance = new CLevel_Zero(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CLevel_Zero");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Zero::Free()
{
	__super::Free();
}



/* HRESULT CLevel_Zero::ThrowFBX(CString _cStrFolderName)
{
	HANDLE fileSearch;
	WIN32_FIND_DATA wfd;
	CString musiccount;
	CString cStrPBXPath;
	CString cStrFolderName = _cStrFolderName;
	CModel::MODEL_TYPE eModelType = CModel::TYPE_END;

	// 일단 모델 폴더에서 anim, nonanim으로 나누어 두었다.
	// 갔다와서 어떻게 경로 찾을지 확인하자

	// Anim, NonAnim을 구별 할 txt 파일을 먼저 찾는다.
musiccount.Format(_T("../Bin/Resource/Models/Skyrim/") + cStrFolderName + _T("/*.txt"));

// FindFirstFile 함수를 통해 검색하려고 하는 파일이 없을 경우 
// 핸들 값은 INVALID_HANDLE_VALUE 값을 가짐
fileSearch = FindFirstFile(musiccount, &wfd);

if (fileSearch != INVALID_HANDLE_VALUE)
{
	if (_tcscmp(wfd.cFileName, _T("00.NonAnim.txt")) == 0)
		eModelType = CModel::TYPE_NONANIM;

	else if (_tcscmp(wfd.cFileName, _T("00.Anim.txt")) == 0)
		eModelType = CModel::TYPE_ANIM;

	else
	{
		MSG_BOX("해당하는 txt 파일 못 찾음.");
		return E_FAIL;
	}
}
else
{
	MSG_BOX("Fail Find : Type.Txt");
	return E_FAIL;
}

// FBX 찾기
musiccount.Format(_T("../Bin/Resource/Models/Skyrim/") + cStrFolderName + _T("/*.fbx"));
fileSearch = FindFirstFile(musiccount, &wfd);

CGameInstance* pGameInstance = CGameInstance::GetInstance();
Safe_AddRef(pGameInstance);

// 찾는 파일이 있다면, 
if (fileSearch != INVALID_HANDLE_VALUE)
{
	do
	{
		// 확장자 자르기
		CString cStrFBXName = CString(wfd.cFileName);
		int lastDotPos = cStrFBXName.ReverseFind('.');
		if (lastDotPos != -1) {
			CString Extension = cStrFBXName.Right(cStrFBXName.GetLength() - lastDotPos - 1);
			cStrFBXName = cStrFBXName.Left(lastDotPos);
		}
		else
		{
			MSG_BOX("확장자를 찾을 수 없음. ");
			return E_FAIL;
		}


		// CString -> const char* : FBX 경로 const char*로 변환. (strFBXPath)
		cStrPBXPath = "../Bin/Resource/Models/Skyrim/" + cStrFolderName + "/" + CString(wfd.cFileName) + ".bin";
		const wchar_t* wStrModelPath = cStrPBXPath;
		// wchar_t 배열을 UTF-8로 변환
		int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wStrModelPath, -1, nullptr, 0, nullptr, nullptr);
		string strFBXPath(utf8Length, '\0');
		WideCharToMultiByte(CP_UTF8, 0, wStrModelPath, -1, &strFBXPath[0], utf8Length, nullptr, nullptr);

		// 세이브 경로 지정 
		CString cStrSavePath = "../Bin/Resource/BinaryFBX/";

		if (eModelType == CModel::TYPE_ANIM)
			cStrSavePath += "Anim/";
		else if (eModelType == CModel::TYPE_NONANIM)
			cStrSavePath += "NonAnim/";

		cStrSavePath += cStrFolderName;
		cStrSavePath += "/";
		cStrSavePath += cStrFBXName;

		// FBX 바이너리화 시작 
		ofstream fileStream(cStrSavePath, ios::binary);
		if (fileStream.is_open())
		{
			pGameInstance->Binary_OutFile(fileStream, strFBXPath.c_str(), eModelType);

			fileStream.close();
		}
		else
			MessageBox(g_hWnd, L"파일을 저장하는 중 오류가 발생했습니다.", L"저장 오류", MB_OK | MB_ICONERROR);

	} while (FindNextFile(fileSearch, &wfd));

	Safe_Release(pGameInstance);
	// 파일 찾기 핸들 값 닫기	
	FindClose(fileSearch);
}

return S_OK;
} */