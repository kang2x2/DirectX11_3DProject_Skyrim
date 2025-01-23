#include "MyFont_Manager.h"
#include "CustomFont.h"

IMPLEMENT_SINGLETON(CMyFont_Manager)

CMyFont_Manager::CMyFont_Manager()
{
}

HRESULT CMyFont_Manager::Add_Font(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strFontTag, const wstring& _strFontFilePath)
{
	if (nullptr != Find_Font(_strFontTag))
		return E_FAIL;

	CCustomFont* pFont = CCustomFont::Create(_pDevice, _pContext, _strFontFilePath);
	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.emplace(_strFontTag, pFont);

	return S_OK;
}

HRESULT CMyFont_Manager::Render(const wstring& _strFontTag, const wstring& _strText, const _float2& _vPos, FXMVECTOR _vColor, float _fRotation, XMFLOAT2 const& _vOrigin, float _fScale)
{
	CCustomFont* pFont = Find_Font(_strFontTag);

	if (nullptr == pFont)
		return E_FAIL;

	return pFont->Render(_strText, _vPos, _vColor, _fRotation, _vOrigin, _fScale);
}

CCustomFont* CMyFont_Manager::Find_Font(const wstring& _strFontTag)
{
	auto	iter = m_Fonts.find(_strFontTag);

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

void CMyFont_Manager::Free()
{
	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();

}
