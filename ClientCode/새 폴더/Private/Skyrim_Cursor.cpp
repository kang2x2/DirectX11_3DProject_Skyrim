#include "framework.h"
#include "Skyrim_Cursor.h"

#include "GameInstance.h"

CSkyrim_Cursor::CSkyrim_Cursor(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimUI(_pDevice, _pContext)
{
}

CSkyrim_Cursor::CSkyrim_Cursor(const CSkyrim_Cursor& rhs)
	: CSkyrimUI(rhs)
{
}

HRESULT CSkyrim_Cursor::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrim_Cursor::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(50.f, 50.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CSkyrim_Cursor::Tick(_float _fTimeDelta)
{
	POINT MousePos = {};
	GetCursorPos(&MousePos);
	ScreenToClient(g_hWnd, &MousePos);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(MousePos.x - ((_float)g_iWinSizeX / 2.f),
			-MousePos.y + ((_float)g_iWinSizeY / 2.f), 0.f, 1.f));
}

void CSkyrim_Cursor::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_2, this);
}

HRESULT CSkyrim_Cursor::Render()
{
	if ((g_curLevel != LEVEL_LOADING && g_bIsPause) ||
		(g_curLevel != LEVEL_LOADING && g_bIsTalk) ||
		g_curLevel == LEVEL_LOGO)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(2);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CSkyrim_Cursor::Ready_Component()
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
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	// Texture
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cursor"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyrim_Cursor::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 전달. */
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;


	return S_OK;
}

CSkyrim_Cursor* CSkyrim_Cursor::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSkyrim_Cursor* pInstance = new CSkyrim_Cursor(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSkyrim_Cursor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyrimUI* CSkyrim_Cursor::Clone(void* pArg)
{
	CSkyrim_Cursor* pInstance = new CSkyrim_Cursor(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CSkyrim_Cursor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyrim_Cursor::Free()
{
	__super::Free();
}
