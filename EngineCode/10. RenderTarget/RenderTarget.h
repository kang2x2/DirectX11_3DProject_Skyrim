#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CRenderTarget() = default;

public:
	HRESULT Initialize(_uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vColor);
	HRESULT Clear();
	HRESULT Bind_SRV(class CShader* _pShader, const _char* _pConstantName);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(_float _fX, _float _fY, _float _fSizeX, _float _fSizeY);
	HRESULT Render(class CShader* _pShader, class CVIBuffer_Rect* _pBuffer);
#endif

public:
	ID3D11RenderTargetView* Get_RTV() const { return m_pRTV; }

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	ID3D11Texture2D*			m_pTexture2D = nullptr;
	ID3D11RenderTargetView*		m_pRTV = nullptr;
	ID3D11ShaderResourceView*	m_pSRV = nullptr;

	_float4						m_vColor = {};

#ifdef _DEBUG
	_float4x4					m_matWorld;
#endif

public:
	static CRenderTarget* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, _uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vColor);
	virtual void Free() override;
};

END

