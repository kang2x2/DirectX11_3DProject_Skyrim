#include "framework.h"
#include "SkyrimArmor.h"

#include "GameInstance.h"

CSkyrimArmor::CSkyrimArmor(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimItem(_pDevice, _pContext)
{
}

CSkyrimArmor::CSkyrimArmor(const CSkyrimArmor& rhs)
	: CSkyrimItem(rhs)
{
}

HRESULT CSkyrimArmor::Initialize_ProtoType()
{
	for (_int i = 0; i < VIEW_END; ++i)
		m_pModelComAry[i] = nullptr;

	return S_OK;
}

HRESULT CSkyrimArmor::Initialize_Clone(void* _pArg)
{
	m_eItemType = ITEM_ARMOR;

	return S_OK;
}

void CSkyrimArmor::Tick(_float _fTimeDelta)
{
}

void CSkyrimArmor::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CSkyrimArmor::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	// 메시 몇개
	_uint		iNumMeshes = m_pModelComAry[m_eViewType]->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelComAry[m_eViewType]->Bind_MaterialTexture(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelComAry[m_eViewType]->Bind_MaterialTexture(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelComAry[m_eViewType]->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSkyrimArmor::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Shader_VtxNonAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyrimArmor::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	// 뷰, 투영 행렬을 던져준다.
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CSkyrimArmor::Free()
{
	__super::Free();

	for (_int i = 0; i < VIEW_END; ++i)
	{
		if (m_pModelComAry[i] != nullptr)
			Safe_Release(m_pModelComAry[i]);
	}

	
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
}
