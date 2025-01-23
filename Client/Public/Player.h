#pragma once

#include "Client_Defines.h"
#include "CreatureObject.h"

BEGIN(Engine)

class CBone;
class CRenderer;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CPlayer final : public CCreatureObject
{
public:
	enum PLAYER_PARTS { PART_BODY, PART_HEAD, PART_HAND, PART_FOOT, PART_WEAPON, PART_CAMERA, PART_END };
	enum PLAYERCAMERA { CAM_3ST, CAM_1ST, CAM_END };
	enum PLAYER_EQUIPSTATE { EQUIP_UNEQUIP, EQUIP_ONEHAND, EQUIP_BOW, EQUIP_MAGIC, EQUIP_END };

	enum PLAYERSTATE {
		UNEQUIP_IDLE,
		UNEQUIP_RUN_F, UNEQUIP_RUN_B, UNEQUIP_RUN_L, UNEQUIP_RUN_R,
		UNEQUIP_SPRINT,

		ONEHAND_IDLE, ONEHAND_EQUIP, ONEHAND_UNEQUIP,
		ONEHAND_RUN_F, ONEHAND_RUN_B, ONEHAND_RUN_L, ONEHAND_RUN_R,
		ONEHAND_SPRINT,

		ONEHAND_LATTACK, ONEHAND_RATTACK, ONEHAND_PATTACK,

		ONEHAND_LWATTACKL, ONEHAND_LWATTACKR,
		ONEHAND_RWATTACKL, ONEHAND_RWATTACKR,
		ONEHAND_FWATTACKL, ONEHAND_FWATTACKR,
		ONEHAND_BWATTACKL, ONEHAND_BWATTACKR,
		
		ONEHAND_RUNPOWERATTACK, ONEHAND_BASH,

		ONEHAND_BLOCK, ONEHAND_ANTICIPATE, ONEHAND_BLOCKHIT,

		PLAYER_1STSTAGGER, PLAYER_3STSTAGGER,

		PLAYERSTATE_END
	};

public:
	typedef struct PlayerDesc
	{
		_float fSprintSpeed = 4.5f;
	}PLAYER_DESC;

private:
	CPlayer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	/* 상태 관련 */
	HRESULT Set_State(PLAYERSTATE _eState);
	PLAYERSTATE	Get_CurState() { return m_eCurState; }

	PLAYER_EQUIPSTATE Get_PlayerEquipState() { return m_eEquipState; }
	void			  Set_PlayerEquipState(PLAYER_EQUIPSTATE _eState) { m_eEquipState = _eState; }

	PLAYER_DESC		  Get_PlayerDesc() { return m_tPlayerDesc; }
	
	/* Hp 관련 */
	_bool			  Get_ReadyRecoveryHp() { return m_bIsReadyRecoveryHp; }
	void			  Set_ReadyRecoveryHp(_bool _isRecovery) { m_bIsReadyRecoveryHp = _isRecovery; }

	/* Sp 관련 */
	_float			  Get_PlayerSp() { return m_fSp; }
	void			  Set_PlayerSp(_float _fSp) { m_fSp += _fSp; }
	_bool			  Get_ReadyRecoverySp() { return m_bIsReadyRecoverySp; }
	void			  Set_ReadyRecoverySp(_bool _isRecovery) { m_bIsReadyRecoverySp = _isRecovery; }

	/* 애니메이션 관련 */
	_bool			Get_IsAnimationFin();
	_bool			Get_CurAnimationIsLoop();
	string			Get_CurAnimationName();
	string			Get_NextAnimationName();
	_uint			Get_CurFrameIndex();
	const char*		Get_CurSocketBonName();
	void			Play_Animation_All(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex = 0, _bool _bIsReset = false, _bool _bIsQuickChange = false);
	void			Play_Animation(PLAYER_PARTS _ePart, _bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex = 0, _bool _bIsReset = false, _bool _bIsQuickChange = false);
	void			Set_SoketBone(const char* _pBoneName);

	/* 플레이어 카메라 관련*/
	PLAYERCAMERA Get_CamMode() { return m_eCurCamMode; }
	_vector		 Get_PlayerCamLook();
	void		 Set_PlayerCam(string _strAnimationName, _uint _iChangeIndex = 0, _bool _bIsLoop = false);

	/* 파츠 관련 */
	CGameObject* Get_Part(PLAYER_PARTS _ePart) { return m_vecPlayerPart[_ePart]; }

	/* 셀 관련 */
	void			  Set_CurCell();
	
	/* 충돌 관련*/
	void			  Create_Spark();
	void			  CheckHit_Onehand(_uint _iSourFrame, _uint _iDestFrame);
	
	/* 공격하는 상태인지? */
	void			  Set_IsAttack(_bool _bIsAttack) { m_bIsAttack = _bIsAttack; }
	_bool			  Get_IsAttack() { return m_bIsAttack; }
	
	/* 카운터 관련 */
	void			  Set_IsReadyCounter(_bool _bIsReadyCounter) { m_bIsReadyCounter = _bIsReadyCounter; }
	_bool			  Get_IsReadyCounter() { return m_bIsReadyCounter; }
	void			  Set_IsCounter(_bool _bIsCounter) { 
		m_bIsCounter = _bIsCounter; 
		m_fCounterTime = 0.f;
	}
	_bool			  Get_IsCounter() { return m_bIsCounter; }
	void			  Set_IsSuccesCounter(_bool _bIsSucces) { 
		m_bIsSuccesCounter = _bIsSucces;
		m_fPauseTime = 0.f;
	}
	_bool			  Get_IsSuccesCounter() { return m_bIsSuccesCounter; }


	/* 인벤토리 */
	void			  Set_IsInvenShow(_bool _bIsShow);
	_bool			  Get_IsInvenShow();
	void			  Use_Item(class CSkyrimItem* _pItem);

	/* 전투 유무 판별 */
	void			  Set_CurBattleObj(_uint _iObj) { m_iCurBattleObj += _iObj; }
	void			  Set_IsHit(_bool _bIsHit) { m_bisHit = _bIsHit; }

private:
	vector<class CGameObject*>		m_vecPlayerPart;

	CBone*							m_pRootBone	= nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CNavigation*					m_pNavigationCom[STAGE_END];
	class CStateManager_Player*		m_pStateManager = nullptr;

	PLAYERCAMERA					m_eCurCamMode = CAM_3ST;
	_uint							m_iAnimKeyIndex = 0;

	PLAYER_EQUIPSTATE				m_eEquipState = EQUIP_END;
	PLAYER_DESC						m_tPlayerDesc;
	PLAYERSTATE						m_eCurState;

	/* 전투 유무 판별 */
	_uint							m_iCurBattleObj = 0;
	/* Hp 관련 */
	_bool							m_bisHit = false;
	_bool							m_bIsReadyRecoveryHp = false;
	_float							m_fRecoveryHpCoolTime = 0;

	/* Sp 관련 */
	_float							m_fSp = 0.f;
	_bool							m_bIsReadyRecoverySp = false;
	_float							m_fRecoverySpCoolTime = 0;

	class CInventory*				m_pInven = nullptr;
	class CSkyrimUI_SpBar*			m_pSpBar = nullptr;
	class CSkyrimUI_HpBar*			m_pHpBar = nullptr;

	_bool							m_bIsAttack = false;
	_bool							m_bIsReadyCounter = false;
	_bool							m_bIsSuccesCounter = false;
	_bool							m_bIsCounter = false;
	
	_float							m_fPauseTime = 0.f;
	_float							m_fCounterTime = 0.f;

private:
	HRESULT Ready_Part();
	HRESULT Ready_Component();
	HRESULT Ready_State();
	HRESULT Ready_PlayerUI();
	void    Player_Recovery(_float _fTimeDelta);

	HRESULT Bind_ShaderResource();

public:
	static CPlayer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

