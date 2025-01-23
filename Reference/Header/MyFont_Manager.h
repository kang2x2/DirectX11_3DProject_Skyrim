#pragma once

/* 여러개의 폰트(글씨체마다) 객체를 가진다. */

#include "Base.h"

BEGIN(Engine)

class CMyFont_Manager final : public CBase
{
	DECLARE_SINGLETON(CMyFont_Manager)
private:
	CMyFont_Manager();
	virtual ~CMyFont_Manager() = default;

public:
	HRESULT Add_Font(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, 
		const wstring& _strFontTag, const wstring& _strFontFilePath);

	HRESULT Render(const wstring& _strFontTag, const wstring& _strText,
		const _float2& _vPos, FXMVECTOR _vColor, float _fRotation,
		XMFLOAT2 const& _vOrigin, float _fScale);

private:
	map<const wstring, class CCustomFont*>			m_Fonts;

private:
	class CCustomFont* Find_Font(const wstring& _strFontTag);

public:
	virtual void Free() override;
};

END

