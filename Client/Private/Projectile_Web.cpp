#include "framework.h"
#include "Projectile_Web.h"

#include "GameInstance.h"

CProjectile_Web::CProjectile_Web(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CProjectile_Web::CProjectile_Web(const CProjectile_Web& rhs)
	: CGameObject(rhs)
{
}

HRESULT CProjectile_Web::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CProjectile_Web::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_bHasMesh = true;
	m_bCreature = false;
	m_strName = TEXT("Projectile_Web");

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_vector*)pArg);
	m_pTransformCom->Set_Speed(1.f);
	m_fLifeTime = GetTickCount64();

	return S_OK;
}

void CProjectile_Web::PriorityTick(_float _fTimeDelta)
{
}

void CProjectile_Web::Tick(_float _fTimeDelta)
{
	m_fTimeDelta = _fTimeDelta;
	m_pTransformCom->Go_Foward(_fTimeDelta);
}

void CProjectile_Web::LateTick(_float _fTimeDelta)
{
	//if (m_fLifeTime + 1000.f < GetTickCount64())
	//{
	//	m_bReadyDead = true;
	//}
}

HRESULT CProjectile_Web::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	// 메시 몇개
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
			return E_FAIL;

		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;

}

HRESULT CProjectile_Web::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_SpiderBullet"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Shader_VtxNonAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};

	AABBDesc.vExtents = _float3(0.5f, 1.5f, 2.0f);
	AABBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pColliderCom, &AABBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CProjectile_Web::Bind_ShaderResource()
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

CProjectile_Web* CProjectile_Web::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CProjectile_Web* pInstance = new CProjectile_Web(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CProjectile_Web");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CProjectile_Web::Clone(void* pArg)
{
	CProjectile_Web* pInstance = new CProjectile_Web(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CProjectile_Web");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CProjectile_Web::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
