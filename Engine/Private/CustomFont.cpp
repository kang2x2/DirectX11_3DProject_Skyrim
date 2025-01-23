#include "CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCustomFont::Initialize(const wstring& _strFontFilePath)
{
	m_pFont = new SpriteFont(m_pDevice, _strFontFilePath.c_str());
	m_pBatch = new SpriteBatch(m_pContext);

	return S_OK;
}

HRESULT CCustomFont::Render(const wstring& _strText, const _float2& _vPos, FXMVECTOR _vColor, _float _fRotation, XMFLOAT2 const& _vOrigin, _float _fScale)
{
	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, _strText.c_str(), _vPos, _vColor, _fRotation, _vOrigin, _fScale);

	m_pBatch->End();

	return S_OK;
}

CCustomFont* CCustomFont::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strFontFilePath)
{
	CCustomFont* pInstance = new CCustomFont(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_strFontFilePath)))
	{
		MSG_BOX("Fail Create : _strFontFilePath");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCustomFont::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);
}
