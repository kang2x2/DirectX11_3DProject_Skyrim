#include "framework.h"
#include "Player_Weapon.h"

#include "GameInstance.h" 
#include "Bone.h"

#include "Weapon_AkaviriSword.h"

#include "Layer.h"
#include "Player.h"
#include "Monster.h"

#include "Effect_CombatSpark.h"

CPlayer_Weapon::CPlayer_Weapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlayerPart_Base(_pDevice, _pContext)
{
}

CPlayer_Weapon::CPlayer_Weapon(const CPlayer_Weapon& rhs)
	: CPlayerPart_Base(rhs)
{
}

HRESULT CPlayer_Weapon::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CPlayer_Weapon::Initialize_Clone(void* _pArg)
{
	if (nullptr != _pArg)
	{
		WEAPON_DESC* pWeaponDesc = (WEAPON_DESC*)_pArg;

		m_pSocketBone = pWeaponDesc->pSocketBone;
		Safe_AddRef(m_pSocketBone);

		m_matSocketPivot = pWeaponDesc->matSocketPivot;

		if (FAILED(__super::Initialize_Clone(_pArg)))
			return E_FAIL;
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(0.0128f, 0.0128f, 0.0128f));
	m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-105.0f));
	// _float4 vInitPos;
	// XMStoreFloat4(&vInitPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	// vInitPos.x -= 0.05f;
	// vInitPos.y -= 0.03f;
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vInitPos));
	
	m_strName = TEXT("PlayerWeapon");

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// m_pSpark = dynamic_cast<CEffect_CombatSpark*>(pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_CombatSpark"), this));

	pGameInstance->Add_CloneObject(g_curLevel, TEXT("Temp"), TEXT("ProtoType_GameObject_Weapon_AkaviriSword"));

	m_pWeapon = pGameInstance->Find_CloneObject(g_curLevel, TEXT("Temp"), TEXT("아카비르 블레이드"));

	Safe_Release(pGameInstance);

	return S_OK;
}

void CPlayer_Weapon::Tick(_float _fTimeDelta)
{
	/* 내 행렬 * (소캣 뼈의 컴바인드 행렬 * 소캣의 행렬 * 페어런트의 월드 행렬) */
	_float4x4 matSocketCombined = m_pSocketBone->Get_CombinedTransformationMatrix();

	_matrix		WorldMatrix = XMLoadFloat4x4(&matSocketCombined) *
		XMLoadFloat4x4(&m_matSocketPivot);

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);

	dynamic_cast<CTransform*>(m_pWeapon->Get_Component(TEXT("Com_Transform")))->Set_WorldMatrix(XMLoadFloat4x4(&m_matWorld));

	if (dynamic_cast<CPlayer*>(m_pParent)->Get_PlayerEquipState() != CPlayer::EQUIP_UNEQUIP &&
		dynamic_cast<CPlayer*>(m_pParent)->Get_PlayerEquipState() != CPlayer::EQUIP_MAGIC)
	{
		m_pColliderCom->Update(XMLoadFloat4x4(&m_matWorld));
	}

	//m_pSpark->Tick(m_pColliderCom->Get_WorldCenter(), _fTimeDelta);
}

void CPlayer_Weapon::LateTick(_float _fTimeDelta)
{
	m_pWeapon->LateTick(_fTimeDelta);

#ifdef _DEBUG
	if (dynamic_cast<CPlayer*>(m_pParent)->Get_PlayerEquipState() != CPlayer::EQUIP_UNEQUIP &&
		dynamic_cast<CPlayer*>(m_pParent)->Get_PlayerEquipState() != CPlayer::EQUIP_MAGIC)
	{
		m_pRendererCom->Add_Debug(m_pColliderCom);
	}
#endif
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	_bool bIsCol = false;

	m_pColliderCom->Late_Update(_fTimeDelta);

	//m_pSpark->LateTick(_fTimeDelta);
}

HRESULT CPlayer_Weapon::Render()
{
	//if (m_pWeapon != nullptr)
	//	m_pWeapon->Render();

	//m_pSpark->Render();

	return S_OK;
}

void CPlayer_Weapon::Set_SoketBone(CBone* _pSocketBone)
{
	Safe_Release(m_pSocketBone);
	m_pSocketBone = _pSocketBone;
	Safe_AddRef(_pSocketBone);
}

const char* CPlayer_Weapon::Get_SoketBoneName()
{
	return m_pSocketBone->Get_BoneName();
}

void CPlayer_Weapon::Set_ViewType(CSkyrimWeapon::WEAPON_VIEWTYPE _eType)
{
	dynamic_cast<CSkyrimWeapon*>(m_pWeapon)->Set_ViewType(_eType);
}

void CPlayer_Weapon::CheckHit_Onehand(_uint _iSourFrame, _uint _iDestFrame)
{

}

void CPlayer_Weapon::Create_Spark()
{
	//m_pSpark->Set_Spark();
}

HRESULT CPlayer_Weapon::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CBounding_OBB::BOUNDING_OBB_DESC OBBDesc = {};
	
	OBBDesc.vExtents = _float3(10.3f, 10.7f, 30.3f);
	OBBDesc.vDegree = _float3(0.f, 0.f, 0.f);
	OBBDesc.vCenter = _float3(0.f, 0.f, OBBDesc.vExtents.z);
	
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), (CComponent**)&m_pColliderCom, &OBBDesc)))
		return E_FAIL;
	
	m_pColliderCom->Set_OwnerObj(m_pParent);

	return S_OK;
}

CPlayer_Weapon* CPlayer_Weapon::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlayer_Weapon* pInstance = new CPlayer_Weapon(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CPlayer_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Weapon::Clone(void* _pArg)
{
	CPlayer_Weapon* pInstance = new CPlayer_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CPlayer_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pSocketBone);
}
