#include "Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CComponent(_pDevice, _pContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_ppSRVs(rhs.m_ppSRVs)
{
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		Safe_AddRef(m_ppSRVs[i]);
	}
}

HRESULT CTexture::Initialize_ProtoType(const wstring& _strTextureFilePath, _uint _iNumTextures)
{
	// 텍스처 수 저장.
	m_iNumTextures = _iNumTextures;

	// 자원(텍스처)를 받을 동적 배열 할당.
	m_ppSRVs = new ID3D11ShaderResourceView*[_iNumTextures];

	// 텍스처의 수 만큼 반복문을 돌며 텍스처를 생성.
	for (size_t i = 0; i < _iNumTextures; ++i)
	{
		// 전체 파일 경로 저장을 위한 문자열 변수 선언
		_tchar szTextureFilePath[MAX_PATH] = TEXT("");

		// 파일 경로 저장
 		wsprintf(szTextureFilePath, _strTextureFilePath.c_str(), i);

		// 파일 확장자 저장을 위한 변수(dds, jpg, png, bmp)
		_tchar szExt[MAX_PATH] = TEXT("");

		// _wsplitpath_s : 경로 이름을 구성 요소로 분할.
		// 저장 해 두었던 전체 파일명에서 구성 요소로 분할하여 szExt에 담겠다.
		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT hr = 0;
		ID3D11ShaderResourceView* pSRV = nullptr; // 배열에 담을 리소스 변수.

		// 파일 확장자에 따른 처리.(strcmp는 같으면 0을 반환함)
		if (lstrcmp(szExt, TEXT(".dds")) == false)
		{
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else if (lstrcmp(szExt, TEXT(".tga")) == false)
		{
			return S_OK;
		}
		else
		{
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}

		if (FAILED(hr))
			return E_FAIL;

		// 정상적으로 텍스처를 생성하였으면 저장.
		m_ppSRVs[i] = pSRV;
	}

	return S_OK;
}

HRESULT CTexture::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(const CShader* _pShader, const char* _pConstantName, _uint _iNumTextures)
{
	return _pShader->Bind_Texture(_pConstantName, m_ppSRVs[_iNumTextures]);
}

HRESULT CTexture::Bind_ShaderResources(const CShader* _pShader, const char* _pConstantName)
{
	return _pShader->Bind_Textures(_pConstantName, m_ppSRVs, m_iNumTextures);
}

CTexture* CTexture::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strTextureFilePath, _uint _iNumTextures)
{
	CTexture* pInstance = new CTexture(_pDevice, _pContext);

 	if (FAILED(pInstance->Initialize_ProtoType(_strTextureFilePath, _iNumTextures)))
	{
		MSG_BOX("Fail Create : CTexture ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CTexture Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumTextures; ++i)
	{
		Safe_Release(m_ppSRVs[i]);
	}

	// 원본일 때에만 지우겠다. 댕글링 포인터 방지
	// 사본일 때 지우게 되면 현재 얕은 복사로 관리 중이기에 원본이 없는 것을 참조할 경우가 생겨
	// 프로그램이 에러로 종료될 수 있음.
	if (m_isCloned == false)
		Safe_Delete_Array(m_ppSRVs);
}
