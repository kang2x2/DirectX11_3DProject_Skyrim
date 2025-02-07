#include "framework.h"
#include "Player.h"

#include "GameInstance.h"

#include "StateManager_Player.h"

#include "Player_Weapon.h"

#include "Player_CameraPart.h"
#include "Player_Body.h"
#include "Player_Head.h"
#include "Player_Hand.h"
#include "Player_Foot.h"
#include "Player_Weapon.h"

#include "SkyrimArmor.h"

#include "Inventory.h"
#include "SkyrimUI_SpBar.h"
#include "SkyrimUI_HpBar.h"

#include "Effect_BloodFlare.h"


CPlayer::CPlayer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCreatureObject(_pDevice, _pContext)
{
	for (_int i = 0; i < STAGE_END; ++i)
	{
		m_pNavigationCom[i] = nullptr;
	}
}
CPlayer::CPlayer(const CPlayer& rhs)
	: CCreatureObject(rhs)
{
}

HRESULT CPlayer::Initialize_ProtoType()
{
	m_vecPlayerPart.resize(PART_END);

	for (_int i = 0; i < m_vecPlayerPart.size(); ++i)
	{
		m_vecPlayerPart[i] = nullptr;
	}

	return S_OK;
}
HRESULT CPlayer::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));

	if (FAILED(Ready_Part()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bIsMaintain = true;
	m_bHasMesh = true;
	m_bHasPart = true;
	m_strName = TEXT("Player");
	m_eEquipState = EQUIP_UNEQUIP;

	m_pTransformCom->Set_Speed(m_fRunSpeed);

	Play_Animation_All(true, "mt_idle");

	m_eCurCamMode = CAM_1ST;
	//m_eCurCamMode = CAM_3ST;

	if (FAILED(Ready_PlayerUI()))
		return E_FAIL;

	return S_OK;
}

void CPlayer::PriorityTick(_float _fTimeDelta)
{
	// cout << "x : " << m_pTransformCom->Get_WorldMatrix_Float4x4()._41 << endl;
	// cout << "y : " << m_pTransformCom->Get_WorldMatrix_Float4x4()._42 << endl;
	// cout << "z : " << m_pTransformCom->Get_WorldMatrix_Float4x4()._43 << endl;

	if (!g_bIsPause)
	{
		for (auto& iter : m_vecPlayerPart)
		{
			if (iter != nullptr)
				iter->PriorityTick(_fTimeDelta);
		}
	}
}
void CPlayer::Tick(_float _fTimeDelta)
{
	/* 아이템 추가. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (m_pInven == nullptr)
	{
		m_pInven = dynamic_cast<CInventory*>
			(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Player_Inventory"), TEXT("UI_Inventory")));
	
		m_pInven->Inven_AddItem(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("SkyrimItem"), TEXT("ProtoType_GameObject_Weapon_AkaviriSword"), nullptr));
		m_pInven->Inven_AddItem(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("SkyrimItem"), TEXT("ProtoType_GameObject_TorsoM_Blades"), nullptr));
		m_pInven->Inven_AddItem(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("SkyrimItem"), TEXT("ProtoType_GameObject_BootsM_Blades"), nullptr));
	}
	Safe_Release(pGameInstance);

	if (m_bIsSuccesCounter)
	{
		g_bIsPause = true;
		m_fPauseTime += _fTimeDelta;
		if (m_fPauseTime > 0.1f)
		{
			m_fPauseTime = 0.1f - m_fPauseTime;
			m_bIsSuccesCounter = false;
			g_bIsPause = false;
		}
	}

	if (!g_bIsPause)
	{
		if (m_bIsCounter)
		{
			m_fAnimationSpeed = 1.5f;
			m_fCounterTime += _fTimeDelta;
			if (m_fCounterTime > 4.f)
			{
				m_fCounterTime = 4.f - m_fCounterTime;
				m_bIsCounter = false;
			}
		}
		else
			m_fAnimationSpeed = 1.f;

		Player_Recovery(_fTimeDelta);

		if (!g_bIsTalk)
			m_pStateManager->Update(_fTimeDelta);

		for (auto& iter : m_vecPlayerPart)
		{
			if (iter != nullptr)
				iter->Tick(_fTimeDelta * m_fAnimationSpeed);
		}
	}

	_vector	vPosition = m_pNavigationCom[g_curStage]->Set_OnCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}
void CPlayer::LateTick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		m_pStateManager->Late_Update(_fTimeDelta);

		for (auto& iter : m_vecPlayerPart)
		{
			if (iter != nullptr)
				iter->LateTick(_fTimeDelta);
		}
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (pGameInstance->Get_DIKeyDown('I'))
	{
		if (m_pInven->Get_IsInvenShow())
		{
			m_pInven->Set_IsInvenShow(false);
			g_bIsPause = false;
		}
		else
		{
			m_pInven->Set_IsInvenShow(true);
			g_bIsPause = true;
		}
	}
	Safe_Release(pGameInstance);

}
HRESULT CPlayer::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	for (auto& iter : m_vecPlayerPart)
	{
		if (iter != nullptr)
			iter->Render();
	}

#ifdef _DEBUG
	m_pNavigationCom[g_curStage]->Render();
#endif

	m_pSpBar->Render();
	m_pHpBar->Render();

	return S_OK;
}

HRESULT CPlayer::Set_State(PLAYERSTATE _eState)
{
	m_eCurState = _eState;
	m_pStateManager->Set_State(_eState);

	return S_OK;
}
_bool CPlayer::Get_IsAnimationFin()
{
	return dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->Get_IsAnimationFin();
}
_bool CPlayer::Get_CurAnimationIsLoop()
{
	return dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->Get_CurAnimationIsLoop();
}
string CPlayer::Get_CurAnimationName()
{
	return dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->Get_CurAnimationName();
}
string CPlayer::Get_NextAnimationName()
{
	return dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->Get_NextAnimationName();
}
_uint CPlayer::Get_CurFrameIndex()
{
	return dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->Get_CurFrameIndex();
}
const char* CPlayer::Get_CurSocketBonName()
{
	return dynamic_cast<CPlayer_Weapon*>(m_vecPlayerPart[PART_WEAPON])->Get_SoketBoneName();
}
void CPlayer::Play_Animation_All(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange)
{
	dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
	dynamic_cast<CPlayer_Head*>(m_vecPlayerPart[PART_HEAD])->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
	dynamic_cast<CPlayer_Hand*>(m_vecPlayerPart[PART_HAND])->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
	dynamic_cast<CPlayer_Foot*>(m_vecPlayerPart[PART_FOOT])->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
}

void CPlayer::Play_Animation(PLAYER_PARTS _ePart, _bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange)
{
	if(_ePart == PART_BODY)
		dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
	else if (_ePart == PART_HEAD)
		dynamic_cast<CPlayer_Head*>(m_vecPlayerPart[PART_HEAD])->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
	else if (_ePart == PART_HAND)
		dynamic_cast<CPlayer_Hand*>(m_vecPlayerPart[PART_HAND])->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
	else if (_ePart == PART_FOOT)
		dynamic_cast<CPlayer_Foot*>(m_vecPlayerPart[PART_FOOT])->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
}

void CPlayer::Set_SoketBone(const char* _pBoneName)
{
	dynamic_cast<CPlayer_Weapon*>(m_vecPlayerPart[PART_WEAPON])
		->Set_SoketBone(dynamic_cast<CPlayerPart_Base*>(m_vecPlayerPart[PART_BODY])
			->Get_SocketBonePtr(_pBoneName));
}
_vector CPlayer::Get_PlayerCamLook()
{
	return dynamic_cast<CPlayer_CameraPart*>(m_vecPlayerPart[PART_CAMERA])->Get_PlayerCamLook();
}
void CPlayer::Set_PlayerCam(string _strAnimationName, _uint _iChangeIndex, _bool _bIsLoop)
{
	if (m_eCurCamMode == CAM_1ST)
		m_eCurCamMode = CAM_3ST;
	else if (m_eCurCamMode == CAM_3ST)
		m_eCurCamMode = CAM_1ST;

	/* Body Set */
	CPlayer_Body* pBody = dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY]);
	pBody->Set_MeshType(m_eCurCamMode);
	/* Hand Set */
	CPlayer_Hand* pHand = dynamic_cast<CPlayer_Hand*>(m_vecPlayerPart[PART_HAND]);
	pHand->Set_MeshType(m_eCurCamMode);

	/* Camera Set */
	if (m_eCurCamMode == CAM_1ST)
	{
		dynamic_cast<CPlayer_CameraPart*>(m_vecPlayerPart[PART_CAMERA])->Set_SoketBone(
			pBody->Get_SocketBonePtr("Camera1st [Cam1]"));
		
		dynamic_cast<CPlayer_Weapon*>(m_vecPlayerPart[PART_WEAPON])->Set_ViewType(CSkyrimWeapon::VIEW_1ST);
	}
	else if (m_eCurCamMode == CAM_3ST)
	{
		dynamic_cast<CPlayer_CameraPart*>(m_vecPlayerPart[PART_CAMERA])->Set_SoketBone(
			pBody->Get_SocketBonePtr("Camera3rd [Cam3]"));
		m_pTransformCom->SetLook(dynamic_cast<CPlayer_CameraPart*>(m_vecPlayerPart[PART_CAMERA])->Get_PlayerCamLook());
		
		dynamic_cast<CPlayer_Weapon*>(m_vecPlayerPart[PART_WEAPON])->Set_ViewType(CSkyrimWeapon::VIEW_3ST);
	}

	dynamic_cast<CPlayer_CameraPart*>(m_vecPlayerPart[PART_CAMERA])->Set_PivotMatrix(
		pBody->Get_SocketPivotMatrix());

	/* 무기 */
	if (m_eEquipState == EQUIP_ONEHAND)
		Set_SoketBone("WEAPON");
	else if (m_eEquipState == EQUIP_UNEQUIP)
		Set_SoketBone("WeaponSword");

	/* 일반 상태 달리기가 이름이 다름. */
	if (m_eCurCamMode == CAM_1ST && m_eCurState == CPlayer::UNEQUIP_RUN_F)
	{
		Play_Animation_All(true, "1hm_runforward", dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->Get_CurFrameIndex());
	}
	else if (m_eCurCamMode == CAM_3ST && m_eCurState == CPlayer::UNEQUIP_RUN_F)
	{
		Play_Animation_All(true, "mt_runforward", dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->Get_CurFrameIndex());
	}
	else
	{
		Play_Animation_All(_bIsLoop, _strAnimationName, _iChangeIndex);
	}




}

void CPlayer::Set_CurCell()
{
	m_pNavigationCom[g_curStage]->Set_CurCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}

void CPlayer::CheckHit_Onehand(_uint _iSourFrame, _uint _iDestFrame)
{
	dynamic_cast<CPlayer_Weapon*>(m_vecPlayerPart[PART_WEAPON])->CheckHit_Onehand(_iSourFrame, _iDestFrame);
}

void CPlayer::Set_IsInvenShow(_bool _bIsShow)
{
	m_pInven->Set_IsInvenShow(_bIsShow);
}

_bool CPlayer::Get_IsInvenShow()
{
	return m_pInven->Get_IsInvenShow();
}

void CPlayer::Use_Item(CSkyrimItem* _pItem)
{
	if (_pItem->Get_ItemType() == CSkyrimItem::ITEM_WEAPON)
	{
		// dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_WEAPON])->Change_Equip(_pItem);
	}
	else if (_pItem->Get_ItemType() == CSkyrimItem::ITEM_ARMOR)
	{
		if (dynamic_cast<CSkyrimArmor*>(_pItem)->Get_ArmorPartType() == CSkyrimArmor::ARMOR_TORSO)
			dynamic_cast<CPlayer_Body*>(m_vecPlayerPart[PART_BODY])->Change_Equip(_pItem);
		
		else if (dynamic_cast<CSkyrimArmor*>(_pItem)->Get_ArmorPartType() == CSkyrimArmor::ARMOR_GLOVE)
			dynamic_cast<CPlayer_Hand*>(m_vecPlayerPart[PART_HAND])->Change_Equip(_pItem);
		
		else if (dynamic_cast<CSkyrimArmor*>(_pItem)->Get_ArmorPartType() == CSkyrimArmor::ARMOR_BOOT)
			dynamic_cast<CPlayer_Foot*>(m_vecPlayerPart[PART_FOOT])->Change_Equip(_pItem);
	}
}

HRESULT CPlayer::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom), &TransformDesc))
		return E_FAIL;

	/* Com_Navigation */
	CNavigation::DESC_NAVIGATION		NavigationDesc;
	NavigationDesc.iCurIndex = 0;
	
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Navigation_WhiteRun"),
		TEXT("Com_Navigation_WhiteRun"), (CComponent**)&m_pNavigationCom[STAGE_WHITERUN], &NavigationDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Navigation_Dungeon"),
		TEXT("Com_Navigation_Dungeon"), (CComponent**)&m_pNavigationCom[STAGE_DUNGEON], &NavigationDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Navigation_Dungeon"),
		TEXT("Com_Navigation_Castle"), (CComponent**)&m_pNavigationCom[STAGE_CASTLE], &NavigationDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Part()
{	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, -1.3f, 12.f, 1.f));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPart = nullptr;

	/* For. Player Body */
	CPlayer_Body::PART_DESC BodyPartDesc;
	BodyPartDesc.pParent = this;
	BodyPartDesc.pParentTransform = m_pTransformCom;

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Player_BodyPart"), &BodyPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecPlayerPart.push_back(pPart);

	/* Root Bone 저장. */
	m_pRootBone = dynamic_cast<CPlayerPart_Base*>(m_vecPlayerPart[PART_BODY])->Get_SocketBonePtr("NPC Root [Root]");

	/* For. Player Head*/
	CPlayer_Head::PART_DESC HeadPartDesc;
	HeadPartDesc.pParent = this;
	HeadPartDesc.pParentTransform = m_pTransformCom;

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Player_HeadPart"), &HeadPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecPlayerPart.push_back(pPart);

	/* For. Player Hand */
	CPlayer_Hand::PART_DESC HandPartDesc;
	HandPartDesc.pParent = this;
	HandPartDesc.pParentTransform = m_pTransformCom;

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Player_HandPart"), &HandPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecPlayerPart.push_back(pPart);

	/* For. Player Foot */
	CPlayer_Foot::PART_DESC FootPartDesc;
	FootPartDesc.pParent = this;
	FootPartDesc.pParentTransform = m_pTransformCom;

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Player_FootPart"), &FootPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecPlayerPart.push_back(pPart);

	/* For. Weapon */
	CPlayer_Weapon::WEAPON_DESC WeaponPartDesc;
	WeaponPartDesc.pParent = this;
	WeaponPartDesc.pParentTransform = m_pTransformCom;
	WeaponPartDesc.pSocketBone = dynamic_cast<CPlayerPart_Base*>(m_vecPlayerPart[PART_BODY])->Get_SocketBonePtr("WeaponSword");
	WeaponPartDesc.matSocketPivot = dynamic_cast<CPlayerPart_Base*>(m_vecPlayerPart[PART_BODY])->Get_SocketPivotMatrix();

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Player_Weapon"), &WeaponPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecPlayerPart.push_back(pPart);

	/* For. Player Camera */
	CPlayer_CameraPart::PLAYER_CAMERAPART_DESC CameraPartDesc;
	CameraPartDesc.pParent = this;
	CameraPartDesc.pParentTransform = m_pTransformCom;
	CameraPartDesc.pBodyTransform = dynamic_cast<CTransform*>(m_vecPlayerPart[PART_BODY]->Get_Component(TEXT("Com_Transform")));
	//CameraPartDesc.pSocketBone = dynamic_cast<CPlayerPart_Base*>(m_vecPlayerPart[PART_BODY])->Get_SocketBonePtr("Camera3rd [Cam3]");
	CameraPartDesc.pSocketBone = dynamic_cast<CPlayerPart_Base*>(m_vecPlayerPart[PART_BODY])->Get_SocketBonePtr("Camera1st [Cam1]");
	CameraPartDesc.matSocketPivot = dynamic_cast<CPlayerPart_Base*>(m_vecPlayerPart[PART_BODY])->Get_SocketPivotMatrix();

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Player_CameraPart"), &CameraPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecPlayerPart.push_back(pPart);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CPlayer::Ready_State()
{
	m_fRunSpeed = 2.5f;
	m_fWalkSpeed = 1.5f;
	m_fHp = 100.f; 
	m_fSp = 100.f;
	m_iAtk = 25;

	m_pStateManager = CStateManager_Player::Create(this, m_pTransformCom, m_pNavigationCom[g_curStage]);

	return S_OK;
}

HRESULT CPlayer::Ready_PlayerUI()
{	
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* SpBar */
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Player_SpBar"), TEXT("ProtoType_GameObject_UI_SpBar"))))
		return E_FAIL;
	m_pSpBar = dynamic_cast<CSkyrimUI_SpBar*>
		(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Player_SpBar"), TEXT("UI_Spbar")));
	/* HpBar */
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Player_HpBar"), TEXT("ProtoType_GameObject_UI_HpBar"))))
		return E_FAIL;
	m_pHpBar = dynamic_cast<CSkyrimUI_HpBar*>
		(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Player_HpBar"), TEXT("UI_Hpbar")));

	Safe_Release(pGameInstance);

	return S_OK;
}

void CPlayer::Player_Recovery(_float _fTimeDelta)
{
#pragma region Hp
	if (m_fHp < 100.f)
	{
		if (!m_bIsReadyRecoveryHp)
		{
			m_fRecoveryHpCoolTime += _fTimeDelta;
			if (m_bisHit)
			{
				CGameInstance* pGameInstance = CGameInstance::GetInstance();
				Safe_AddRef(pGameInstance);

				CGameObject* pBloodFlare = pGameInstance->Find_CloneObject(g_curLevel, TEXT("Layer_Effect"), TEXT("Effect_BloodFlare"));

				dynamic_cast<CEffect_BloodFlare*>(pBloodFlare)->Set_Alpha();

				Safe_Release(pGameInstance);

				m_fRecoveryHpCoolTime = 0.f;
				m_bisHit = false;
			}

			if (m_fRecoveryHpCoolTime > 4.f)
			{
				m_fRecoveryHpCoolTime = 4.f - m_fRecoveryHpCoolTime;
				m_bIsReadyRecoveryHp = true;
			}
		}

		if (m_bIsReadyRecoveryHp && m_fHp < 100)
		{
			if (m_bisHit)
			{
				CGameInstance* pGameInstance = CGameInstance::GetInstance();
				Safe_AddRef(pGameInstance);

				CGameObject* pBloodFlare = pGameInstance->Find_CloneObject(g_curLevel, TEXT("Layer_Effect"), TEXT("Effect_BloodFlare"));

				dynamic_cast<CEffect_BloodFlare*>(pBloodFlare)->Set_Alpha();

				Safe_Release(pGameInstance);

				m_bIsReadyRecoveryHp = false;
				m_bisHit = false;
				m_fRecoveryHpCoolTime = 0.f;
			}

			m_fHp += 3.f * _fTimeDelta;
			if (m_fHp >= 100.f)
			{
				m_fHp = 100.f;
				m_bIsReadyRecoveryHp = false;
			}
		}
	}

	if (m_fHp < 0.f)
		m_fHp = 0.f;

#pragma endregion

#pragma region SP
	if (!m_bIsReadyRecoverySp)
	{
		m_fRecoverySpCoolTime += _fTimeDelta;
		if (m_fRecoverySpCoolTime > 2.f)
		{
			m_fRecoverySpCoolTime = 2.f - m_fRecoverySpCoolTime;
			m_bIsReadyRecoverySp = true;
		}
	}

	if (m_bIsReadyRecoverySp && m_fSp < 100)
	{
		m_fSp += 3.f * _fTimeDelta;
		if (m_fSp >= 100.f)
		{
			m_fSp = 100.f;
			m_bIsReadyRecoverySp = false;
		}
	}

	if (m_fSp < 0.f)
		m_fSp = 0.f;
#pragma endregion

	m_pSpBar->Tick(m_fSp, _fTimeDelta);
	m_pHpBar->Tick(m_fHp, _fTimeDelta);
}

HRESULT CPlayer::Bind_ShaderResource()
{
	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlayer* pInstance = new CPlayer(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CPlayer ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* _pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CPlayer Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Create_Spark()
{
	dynamic_cast<CPlayer_Weapon*>(m_vecPlayerPart[PART_WEAPON])->Create_Spark();
}

void CPlayer::Free()
{
	__super::Free();

#ifdef _DEBUG
	// 주석 하냐 안하냐에 따라 릴리즈, 디버그에서 종료 시 에러남.
	for (_int i = 0; i < m_vecPlayerPart.size(); ++i)
	{
		if (m_vecPlayerPart[i] != nullptr)
			Safe_Release(m_vecPlayerPart[i]);
	}
	m_vecPlayerPart.clear();
#endif

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pStateManager);

	for (_int i = 0; i < STAGE_END; ++i)
	{
		Safe_Release(m_pNavigationCom[i]);
	}
}
