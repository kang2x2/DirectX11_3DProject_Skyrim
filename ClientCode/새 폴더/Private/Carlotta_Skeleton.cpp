#include "framework.h"
#include "Carlotta_Skeleton.h"

#include "GameInstance.h"
#include "Player.h"

CCarlotta_Skeleton::CCarlotta_Skeleton(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCarlottaPart_Base(_pDevice, _pContext)
{
}

CCarlotta_Skeleton::CCarlotta_Skeleton(const CCarlotta_Skeleton& rhs)
	: CCarlottaPart_Base(rhs)
{
}

HRESULT CCarlotta_Skeleton::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CCarlotta_Skeleton::Initialize_Clone(void* _pArg)
{
	if (FAILED(__super::Initialize_Clone(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_strName = TEXT("Carlotta Skeleton");

	return S_OK;
}

void CCarlotta_Skeleton::Tick(_float _fTimeDelta)
{
	m_pModelCom->Play_Animation(_fTimeDelta);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix());

	/* aabb오리지널 바운딩 * 행렬을 해서 실제 충돌하기위한 데이터(aabb)에게 전달한다.*/
	m_pColliderCom->Update(XMLoadFloat4x4(&m_matWorld));
}

void CCarlotta_Skeleton::LateTick(_float _fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pColliderCom);
#endif
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	m_pColliderCom->Late_Update();
}

HRESULT CCarlotta_Skeleton::Render()
{
	/* 뼈만 가지고 있을거라 그릴 필요가 없다.*/
	return S_OK;
}

void CCarlotta_Skeleton::Set_AnimationIndex(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex)
{
	m_pModelCom->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex);
}

_uint CCarlotta_Skeleton::Get_CurFrameIndex()
{
	return m_pModelCom->Get_CurFrameIndex();
}

_bool CCarlotta_Skeleton::Get_CurAnimationIsLoop()
{
	return m_pModelCom->Get_CurAnimationIsLoop();
}

_bool CCarlotta_Skeleton::Get_IsAnimationFin()
{
	return m_pModelCom->Get_IsAnimationFin();
}

string CCarlotta_Skeleton::Get_CurAnimationName()
{
	return m_pModelCom->Get_CurAnimationName().c_str();
}

HRESULT CCarlotta_Skeleton::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_Carlotta_Skeleton"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};

	AABBDesc.vExtents = _float3(0.3f, 0.7f, 0.3f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pColliderCom, &AABBDesc)))
		return E_FAIL;

	m_pColliderCom->Set_OwnerObj(m_pParent);

	return S_OK;
}

HRESULT CCarlotta_Skeleton::Bind_ShaderResource()
{
	return S_OK;
}

CCarlotta_Skeleton* CCarlotta_Skeleton::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCarlotta_Skeleton* pInstance = new CCarlotta_Skeleton(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CCarlotta_Skeleton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCarlotta_Skeleton::Clone(void* _pArg)
{
	CCarlotta_Skeleton* pInstance = new CCarlotta_Skeleton(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CCarlotta_Skeleton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCarlotta_Skeleton::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
