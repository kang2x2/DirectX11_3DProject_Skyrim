#include "framework.h"
#include "SkyrimUI_SpBar.h"

#include "GameInstance.h"

CSkyrimUI_SpBar::CSkyrimUI_SpBar(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimUI(_pDevice, _pContext)
{
}

CSkyrimUI_SpBar::CSkyrimUI_SpBar(const CSkyrimUI_SpBar& rhs)
	: CSkyrimUI(rhs)
{
}

HRESULT CSkyrimUI_SpBar::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimUI_SpBar::Initialize_Clone(void* _pArg)
{
	__super::Initialize_Clone(_pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	UI_InitPos(1100.f, 680.f);

	m_fSizeX = 200.f;
	m_fSizeY = 20.f;

	// (m_fX), m_fY + m_fSizeY * 0.5f, 0.f, 1.f)

	// 부모의 init 윈도우 상의 0,0 으로 잡아놓는 코드 추가해놓자.

	m_pTransformEmpty->Set_Scaling(_float3(m_fSizeX + 50.f, m_fSizeY + 10.f, 1.f));
	m_pTransformEmpty->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX, m_fY, 0.f, 1.f));

	m_pTransformFill->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformFill->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX, m_fY, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_strName = TEXT("UI_Spbar");

	return S_OK;
}

void CSkyrimUI_SpBar::PriorityTick(_float _fTimeDelta)
{
}

void CSkyrimUI_SpBar::Tick(_float _fSp, _float _fTimeDelta)
{
	if (_fSp < 100.f)
	{
		if (m_fAlpha < 1.f)
			m_fAlpha += _fTimeDelta * 0.5f;

		m_bIsShow = true;
	}
	else
	{
		if (m_fAlpha > 0.f)
			m_fAlpha -= _fTimeDelta * 0.5f;

		if (m_fAlpha <= 0.f)
			m_bIsShow = false;
	}

	_float fTempSizeX = m_fSizeX;
	m_fSizeX = _fSp * 2.f;

	if (m_fSizeX != fTempSizeX)
	{
		if (m_fSizeX < 0.1f)
		{
			m_fSizeX = 0.1f;
			UI_InitPos(1100.f, 680.f);
			m_fX -= 100.f;
		}

			m_fX -= (fTempSizeX - m_fSizeX) / 2.f;

		m_pTransformFill->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformFill->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX, m_fY, 0.f, 1.f));
	}
}

void CSkyrimUI_SpBar::LateTick(_float _fTimeDelta)
{
	if (m_fAlpha > 0.1f)
		m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_0, this);
}

HRESULT CSkyrimUI_SpBar::Render()
{
	if (m_fAlpha > 0.1f)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSkyrimUI_SpBar::Ready_Component()
{
	// Renderer
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// Shader
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// VIBuffer
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferEmpty"), (CComponent**)&m_pVIBufferEmpty)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferFill"), (CComponent**)&m_pVIBufferFill)))
		return E_FAIL;

	// Texture
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EmptyBar"),
		TEXT("Com_TextureEmpty"), (CComponent**)&m_pTextureEmpty)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SpBar"),
		TEXT("Com_TextureFill"), (CComponent**)&m_pTextureFill)))
		return E_FAIL;

	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformEmpty"), (CComponent**)&m_pTransformEmpty)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformFill"), (CComponent**)&m_pTransformFill)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyrimUI_SpBar::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 전달. */
	if (FAILED(m_pTransformEmpty->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	if (FAILED(m_pTextureEmpty->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferEmpty->Render();

	if (FAILED(m_pTransformFill->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//_float4 tempColor = { 0.f, 0.5f, 0.f, 1.f };
	//if (FAILED(m_pShaderCom->Bind_RawValue(("g_UIColor"), &tempColor, sizeof(_float4))))
	//	return E_FAIL;

	if (FAILED(m_pTextureFill->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferFill->Render();

	return S_OK;
}

CSkyrimUI_SpBar* CSkyrimUI_SpBar::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSkyrimUI_SpBar* pInstance = new CSkyrimUI_SpBar(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSkyrimUI_SpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyrimUI* CSkyrimUI_SpBar::Clone(void* _pArg)
{
	CSkyrimUI_SpBar* pInstance = new CSkyrimUI_SpBar(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CSkyrimUI_SpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyrimUI_SpBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformEmpty);
	Safe_Release(m_pTextureEmpty);
	Safe_Release(m_pVIBufferEmpty);

	Safe_Release(m_pTransformFill);
	Safe_Release(m_pTextureFill);
	Safe_Release(m_pVIBufferFill);
}
