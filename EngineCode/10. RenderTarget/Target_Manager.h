#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Add_RenderTarget(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strTargetTag, _uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vColor);
	HRESULT Add_MRT(const wstring& _strMRTTag, const wstring& _strTargetTag);
	HRESULT Bind_SRV(class CShader* _pShader, const wstring& _strTargetTag, const _char* _pConstantName);

	HRESULT Begin_MRT(ID3D11DeviceContext* _pContext, const wstring& _strMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* _pContext);

#ifdef _DEBUG
	HRESULT Ready_Debug(const wstring& _strTargetTag, _float _fX, _float _fY, _float _fSizeX, _float _fSizeY);
	HRESULT Render(const wstring& _strMRTTag, class CShader* _pShader, class CVIBuffer_Rect* _pBuffer);
#endif

private:
	map<const wstring, class CRenderTarget*> m_mapRenderTarget;

	map<const wstring, list<class CRenderTarget*>> m_mapMRT;

	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView* m_pDSV = nullptr;

private:
	class CRenderTarget* Find_RenderTarget(const wstring& _strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const wstring& _strMRTTag);

public:
	virtual void Free() override;
};

END

