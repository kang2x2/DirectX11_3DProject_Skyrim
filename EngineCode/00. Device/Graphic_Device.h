#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	// 그래픽 디바이스 초기화
	HRESULT Ready_Graphic_Device(HWND _hWnd, GRAPHIC_DESC::WINMODE _eWinMode, _uint _iWinCX, _uint _iWinCY,
		_Inout_ ID3D11Device** _pDevice, _Inout_ ID3D11DeviceContext** _pContext);

	HRESULT Clear_BackBuffer_View(_float4 _vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;

	ID3D11RenderTargetView* m_pBackBufferRTV	 = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView  = nullptr;

private:
	HRESULT Ready_SwapChain(HWND _hWnd, GRAPHIC_DESC::WINMODE _eWinMode, _uint _iWinCX, _uint _iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint _iWinCX, _uint _iWinCY);

public:
	virtual void Free() override;
};

END
