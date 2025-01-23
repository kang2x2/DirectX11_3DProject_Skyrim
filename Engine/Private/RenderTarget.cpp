#include "RenderTarget.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

CRenderTarget::CRenderTarget(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Initialize(_uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vColor)
{
	m_vColor = _vColor;

	D3D11_TEXTURE2D_DESC TextureDesc = {};

	TextureDesc.Width		= _iSizeX;
	TextureDesc.Height		= _iSizeY;
	TextureDesc.MipLevels	= 1;
	TextureDesc.ArraySize	= 1;
	TextureDesc.Format		= _ePixelFormat;

	TextureDesc.SampleDesc.Quality  = 0;
	TextureDesc.SampleDesc.Count	= 1;

	TextureDesc.Usage			= D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags		= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags  = 0;
	TextureDesc.MiscFlags		= 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderTarget::Clear()
{
	m_pContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vColor);

	return S_OK;
}

HRESULT CRenderTarget::Bind_SRV(CShader* _pShader, const _char* _pConstantName)
{
	return _pShader->Bind_Texture(_pConstantName, m_pSRV);
}

#ifdef _DEBUG
HRESULT CRenderTarget::Ready_Debug(_float _fX, _float _fY, _float _fSizeX, _float _fSizeY)
{
	D3D11_VIEWPORT ViewPortDesc;

	_uint iNumViewport = 1;

	m_pContext->RSGetViewports(&iNumViewport, &ViewPortDesc);

	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());

	m_matWorld._11 = _fSizeX;
	m_matWorld._22 = _fSizeY;

	m_matWorld._41 = _fX - ViewPortDesc.Width * 0.5f;
	m_matWorld._42 = -_fY + ViewPortDesc.Height * 0.5f;

	return S_OK;
}

HRESULT CRenderTarget::Render(CShader* _pShader, CVIBuffer_Rect* _pBuffer)
{
	if (FAILED(_pShader->Bind_Matrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;

	if (FAILED(_pShader->Bind_Texture("g_Texture", m_pSRV)))
		return E_FAIL;

	if (FAILED(_pShader->Begin(0)))
		return E_FAIL;

	if (FAILED(_pBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

#endif

CRenderTarget* CRenderTarget::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, _uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vColor)
{
	CRenderTarget* pInstance = new CRenderTarget(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_iSizeX, _iSizeY, _ePixelFormat, _vColor)))
	{
		MSG_BOX("Fail Create : CRenderTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	__super::Free();

	Safe_Release(m_pTexture2D);
	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
