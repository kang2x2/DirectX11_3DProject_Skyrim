#include "framework.h"
#include "BossSpider_Mouth.h"

#include "GameInstance.h"

#include "Bone.h"
#include "Player.h"
#include "BossSpider.h"

CBossSpider_Mouth::CBossSpider_Mouth(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CBossSpiderPart_Base(_pDevice, _pContext)
{
}

CBossSpider_Mouth::CBossSpider_Mouth(const CBossSpider_Mouth& rhs)
	: CBossSpiderPart_Base(rhs)
{
}

HRESULT CBossSpider_Mouth::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CBossSpider_Mouth::Initialize_Clone(void* _pArg)
{
	if (nullptr != _pArg)
	{
		MOUTH_DESC* pMouthDesc = (MOUTH_DESC*)_pArg;

		m_pSocketBone = pMouthDesc->pSocketBone;
		Safe_AddRef(m_pSocketBone);

		m_matSocketPivot = pMouthDesc->matSocketPivot;

		if (FAILED(__super::Initialize_Clone(_pArg)))
			return E_FAIL;
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(0.036f, 0.036f, 0.036f));
	m_pTransformCom->Fix_Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(45.0f));
	_float4 vInitPos;
	XMStoreFloat4(&vInitPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	vInitPos.x += 0.1f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vInitPos));
	
	m_strName = TEXT("BossSpider_Mouth");

	return S_OK;
}

void CBossSpider_Mouth::Tick(_float _fTimeDelta)
{
	/* 내 행렬 * (소캣 뼈의 컴바인드 행렬 * 소캣의 행렬 * 페어런트의 월드 행렬) */
	_float4x4 matSocketCombined = m_pSocketBone->Get_CombinedTransformationMatrix();

	_matrix		WorldMatrix = XMLoadFloat4x4(&matSocketCombined) *
		XMLoadFloat4x4(&m_matSocketPivot);

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);

	m_pColliderCom->Update(XMLoadFloat4x4(&m_matWorld));

	if (g_curLevel != LEVEL_TOOL)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CGameObject* m_pPlayer = pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player"));
		CGameObject* m_pPlayerWeapon = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Part(CPlayer::PART_WEAPON);

		pGameInstance->Collision_Out(m_pColliderCom, dynamic_cast<CCollider*>(m_pPlayerWeapon->Get_Component(TEXT("Com_Collider_OBB"))));
		pGameInstance->Collision_Out(m_pColliderCom, dynamic_cast<CCollider*>(dynamic_cast<CPlayer*>(m_pPlayer)->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));

		Safe_Release(pGameInstance);
	}
}

void CBossSpider_Mouth::LateTick(_float _fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pColliderCom);
#endif
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CBossSpider_Mouth::Render()
{
	return S_OK;
}

const char* CBossSpider_Mouth::Get_SoketBoneName()
{
	return m_pSocketBone->Get_BoneName();
}

HRESULT CBossSpider_Mouth::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CBounding_OBB::BOUNDING_OBB_DESC OBBDesc = {};

	OBBDesc.vExtents = _float3(12.3f, 30.7f, 12.3f);
	OBBDesc.vDegree = _float3(0.f, 90.f, 0.f);
	OBBDesc.vCenter = _float3(0.f, OBBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), (CComponent**)&m_pColliderCom, &OBBDesc)))
		return E_FAIL;

	m_pColliderCom->Set_OwnerObj(m_pParent);

	return S_OK;
}

CBossSpider_Mouth* CBossSpider_Mouth::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CBossSpider_Mouth* pInstance = new CBossSpider_Mouth(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Craete : CBossSpider_Mouth");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossSpider_Mouth::Clone(void* _pArg)
{
	CBossSpider_Mouth* pInstance = new CBossSpider_Mouth(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CBossSpider_Mouth");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossSpider_Mouth::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pSocketBone);
}
