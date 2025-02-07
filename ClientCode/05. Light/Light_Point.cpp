#include "Light_Point.h"

#include "GameInstance.h"

/* 방향성 광원이 0번째 인덱스이기 때문. */
_uint CLight_Point::g_iLightIndex = 1;

CLight_Point::CLight_Point(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CLight_Point::CLight_Point(const CLight_Point& rhs)
	: CGameObject(rhs)
{
}

HRESULT CLight_Point::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CLight_Point::Initialize_Clone(void* pArg)
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* AABB */
	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};
	AABBDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pColliderCom, &AABBDesc)))
		return E_FAIL;

	m_pColliderCom->Set_OwnerObj(this);

	_matrix* pMatPivot = (_matrix*)pArg;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	Ready_Light();

	m_iLightIndex = g_iLightIndex++;

	m_strName = TEXT("Light_Point");

	return S_OK;
}

HRESULT CLight_Point::Initialize_LightClone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg, LIGHT_DESC* _LightDesc)
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* AABB */
	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};
	AABBDesc.vExtents = _float3(1.f, 1.f, 1.f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pColliderCom, &AABBDesc)))
		return E_FAIL;

	m_pColliderCom->Set_OwnerObj(this);

	_matrix* pMatPivot = (_matrix*)_pArg;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	Ready_Light(*_LightDesc);

	m_iLightIndex = g_iLightIndex++;

	m_strName = TEXT("Light_Point");

	return S_OK;
}

void CLight_Point::PriorityTick(_float _fTimeDelta)
{
}

void CLight_Point::Tick(_float _fTimeDelta)
{
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

	m_pColliderCom->Update(matWorld);
}

void CLight_Point::LateTick(_float _fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pColliderCom);
#endif
}

HRESULT CLight_Point::Render()
{
	return S_OK;
}

LIGHT_DESC* CLight_Point::Get_LightDesc()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHT_DESC* LightDesc = pGameInstance->Get_LightDesc(m_iLightIndex);
	
	Safe_Release(pGameInstance);

	return LightDesc;
}

void CLight_Point::Set_LightDesc(LIGHT_DESC _lightDesc)
{
	m_tLightFileDesc.lightDesc = _lightDesc;
}

HRESULT CLight_Point::Ready_Light()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHT_DESC LightDesc;

	/* 점 광원 추가 */
	ZeroMemory(&LightDesc, sizeof(LightDesc));
	LightDesc.eLightType = LIGHT_DESC::LIGHT_POINT;
	LightDesc.vLightPos = _float4(1.f, -1.3f, 12.f, 1.f);
	LightDesc.fLightRange = 20.f;
	LightDesc.vDiffuse = _float4(1.f, 0.4f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	XMStoreFloat4(&LightDesc.vLightPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (FAILED(pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	m_tLightFileDesc.lightDesc = LightDesc;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLight_Point::Ready_Light(LIGHT_DESC _pLightDesc)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHT_DESC LightDesc;

	/* 점 광원 추가 */
	LightDesc = _pLightDesc;

	if (FAILED(pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CLight_Point* CLight_Point::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLight_Point* pInstance = new CLight_Point(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CLight_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLight_Point::Clone(void* _pArg)
{
	CLight_Point* pInstance = new CLight_Point(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CLight_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLight_Point::LightClone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg, LIGHT_DESC* _LightDesc)
{
	CLight_Point* pInstance = new CLight_Point(*this);

	if (FAILED(pInstance->Initialize_LightClone(_iLevel, _strModelComTag, _pArg, _LightDesc)))
	{
		MSG_BOX("Fail Clone : CLight_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight_Point::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
}
