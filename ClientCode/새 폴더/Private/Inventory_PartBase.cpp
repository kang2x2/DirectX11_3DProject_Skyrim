#include "framework.h"
#include "Inventory_PartBase.h"

#include "GameInstance.h"

CInventory_PartBase::CInventory_PartBase(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CInventory_PartBase::CInventory_PartBase(const CInventory_PartBase& rhs)
	: CGameObject(rhs)
{
}

HRESULT CInventory_PartBase::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CInventory_PartBase::Initialize_Clone(void* _pArg)
{
	if (_pArg != nullptr)
	{
		INVENPART_DESC* pPartDesc = (INVENPART_DESC*)_pArg;
		m_vBlackPos = pPartDesc->vBlackPos;
		m_vBlackSize = pPartDesc->vBlackSize;

		m_vLine1Pos = pPartDesc->vLine1Pos;
		m_vLine1Size = pPartDesc->vLine1Size;

		m_vLine2Pos = pPartDesc->vLine2Pos;
		m_vLine2Size = pPartDesc->vLine2Size;
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformBlack->Set_Scaling(_float3(m_vBlackSize.x, m_vBlackSize.y, 1.f));
	m_pTransformBlack->Set_State(CTransform::STATE_POSITION,
		XMVectorSet((m_vBlackPos.x) - ((_float)g_iWinSizeX / 2.f),
					-m_vBlackPos.y + m_vBlackSize.y * 0.5f, 0.f, 1.f));

	m_pTransformLine1->Set_Scaling(_float3(m_vLine1Size.x, m_vLine1Size.y, 1.f));
	m_pTransformLine1->Set_State(CTransform::STATE_POSITION,
		XMVectorSet((m_vLine1Pos.x) - ((_float)g_iWinSizeX / 2.f),
					-m_vLine1Pos.y + m_vLine1Size.y * 0.5f, 0.f, 1.f));

	m_pTransformLine2->Set_Scaling(_float3(m_vLine2Size.x, m_vLine2Size.y, 1.f));
	m_pTransformLine2->Set_State(CTransform::STATE_POSITION,
		XMVectorSet((m_vLine2Pos.x) - ((_float)g_iWinSizeX / 2.f),
					-m_vLine2Pos.y + m_vLine2Size.y * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CInventory_PartBase::PriorityTick(_float _fTimeDelta)
{
}

void CInventory_PartBase::Tick(_float _fTimeDelta)
{
	if (m_fAlpha < 0.3f)
		m_fAlpha += 1.f * _fTimeDelta;
}

void CInventory_PartBase::LateTick(_float _fTimeDelta)
{
}

HRESULT CInventory_PartBase::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory_PartBase::Ready_Component()
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
		TEXT("Com_VIBufferBlack"), (CComponent**)&m_pVIBufferBlack)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferLine1"), (CComponent**)&m_pVIBufferLine1)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferLine2"), (CComponent**)&m_pVIBufferLine2)))
		return E_FAIL;

	// Texture
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Black"),
		TEXT("Com_TextureBlack"), (CComponent**)&m_pTextureBlack)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WhiteEm"),
		TEXT("Com_TextureLine1"), (CComponent**)&m_pTextureLine1)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WhiteEm"),
		TEXT("Com_TextureLine2"), (CComponent**)&m_pTextureLine2)))
		return E_FAIL;

	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformBlack"), (CComponent**)&m_pTransformBlack)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformLine1"), (CComponent**)&m_pTransformLine1)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformLine2"), (CComponent**)&m_pTransformLine2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory_PartBase::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 전달. */
	if (FAILED(m_pTransformBlack->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureBlack->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferBlack->Render();

	if (FAILED(m_pTransformLine1->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureLine1->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferLine1->Render();

	if (FAILED(m_pTransformLine2->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureLine2->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferLine2->Render();

	return S_OK;
}

void CInventory_PartBase::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	Safe_Release(m_pTransformBlack);
	Safe_Release(m_pTransformLine1);
	Safe_Release(m_pTransformLine2);

	Safe_Release(m_pTextureBlack);
	Safe_Release(m_pTextureLine1);
	Safe_Release(m_pTextureLine2);

	Safe_Release(m_pVIBufferBlack);
	Safe_Release(m_pVIBufferLine1);
	Safe_Release(m_pVIBufferLine2);
}
