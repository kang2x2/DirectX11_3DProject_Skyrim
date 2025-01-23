#pragma once

#include "Base.h"

BEGIN(Engine)

class CCustomFont final : public CBase
{
private:
	CCustomFont(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize(const wstring& _strFontFilePath);
	HRESULT Render(const wstring& _strText, const _float2& _vPos, FXMVECTOR _vColor,
		_float _fRotation, XMFLOAT2 const& _vOrigin, _float _fScale);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	SpriteBatch* m_pBatch = { nullptr };
	SpriteFont* m_pFont = { nullptr };

public:
	static CCustomFont* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strFontFilePath);
	virtual void Free() override;
};

END

