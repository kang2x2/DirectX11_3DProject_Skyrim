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

	// 백버퍼 클리어 
	HRESULT Clear_BackBuffer_View(_float4 _vClearColor);

	// 깊이, 스탠실 버퍼 클리어
	HRESULT Clear_DepthStencil_View();

	// 후면 버퍼 전면 버퍼로 교체 (백버퍼 화면에 직접 보여주기)
	HRESULT Present();

private:
	// 메모리 할당 역할. Vertex Index Buffer, TextureLoad와 같은 Com객체의 생성과 관련된 역할을 맡는다. 
	ID3D11Device*			m_pDevice = nullptr;
	// 기능을 실행하거나 그리는 역할. 
	// (바인딩작업, 정점버퍼를 SetStreamSource(), SetIndices(), SetTransform(), SetTexture())
	ID3D11DeviceContext*	m_pContext = nullptr;
	// 후면, 전면 버퍼 교체하면서 화면에 보여주는 역할.
	IDXGISwapChain*			m_pSwapChain = nullptr;

	/* 원래 9에서 사용하던 Com객체 : IDirect3DTexture9 */
	/* ID3D11Texture2D : 텍스쳐를 표현하는 사전객체 타입이다.
	why? 용도에 맞는 실질적으로 사용하기위한 텍스쳐객체를 만들어내기위해.  */
	

	// 렌더타겟용으로 사용될 수 있는 텍스쳐 타입.
	ID3D11RenderTargetView* m_pBackBufferRTV	 = nullptr;
	// 깊이스텐실버퍼로서 사용될 수 있는 타입.
	ID3D11DepthStencilView* m_pDepthStencilView  = nullptr;
	// 셰이더에 전달될 수 있는 텍스처타입.
	// ID3D11ShaderResourceView 

private:
	/* 스왑체인은 필수적으로 백버퍼가 필요하다. 
	   그렇기에 백버퍼를 생성하기 위한 정보를 받아야 한다.
	   즉, 스왑체인을 생성했다는 것은 백버퍼를 생성했다는 것이다. */
	HRESULT Ready_SwapChain(HWND _hWnd, GRAPHIC_DESC::WINMODE _eWinMode, _uint _iWinCX, _uint _iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint _iWinCX, _uint _iWinCY);

public:
	virtual void Free() override;
};

END
