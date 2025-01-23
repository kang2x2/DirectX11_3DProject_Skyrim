#include "framework.h"
#include "Terrain_Grid.h"

#include "GameInstance.h"

CTerrain_Grid::CTerrain_Grid(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTerrain_Grid::CTerrain_Grid(const CTerrain_Grid& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain_Grid::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CTerrain_Grid::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bHasMesh = false;
	m_strName = TEXT("Tool_GridTerrain");

	return S_OK;
}

void CTerrain_Grid::Tick(_float fTimeDelta)
{

}

void CTerrain_Grid::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CTerrain_Grid::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
	
	return S_OK;
}

HRESULT CTerrain_Grid::Ready_Components()
{
	// VIBuffer
	if (FAILED(__super::Add_CloneComponent(LEVEL_TOOL, TEXT("ProtoType_Component_VIBuffer_Terrain_Grid"),
		TEXT("Com_VIBuffer_Grid"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	// Shader
	if(FAILED(__super::Add_CloneComponent(LEVEL_TOOL, TEXT("ProtoType_Component_Shader_VtxPosCol"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// Renderer
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Grid::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CTerrain_Grid* CTerrain_Grid::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CTerrain_Grid* pInstance = new CTerrain_Grid(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CTerrain_Grid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain_Grid::Clone(void* pArg)
{
	CTerrain_Grid* pInstance = new CTerrain_Grid(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CTerrain_Grid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_Grid::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
}
