#pragma once

#include "Skyrim_NPC.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CNPC_GuardCaptain final : public CSkyrim_NPC
{
public:
	enum GUARDCAPTAIN_PARTS { PART_WEAPON, PART_END };

	enum GUARDCAPTAIN_COLTYPE {
		GUARDCAPTAIN_COL_AABB,
		GUARDCAPTAIN_COL_DETECTION,
		GUARDCAPTAIN_COL_DIALOG,
		GUARDCAPTAIN_COL_END
	};

	enum GUARDCAPTAIN_STATE {
		GUARDCAPTAIN_IDLE,
		GUARDCAPTAIN_EQUIP, GUARDCAPTAIN_UNEQUIP,
		GUARDCAPTAIN_WALK, GUARDCAPTAIN_UNEQUIPWALK,
		GUARDCAPTAIN_WARNING, GUARDCAPTAIN_TALK,
		GUARDCAPTAIN_END
	};

private:
	CNPC_GuardCaptain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNPC_GuardCaptain(const CNPC_GuardCaptain& rhs);
	virtual ~CNPC_GuardCaptain() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_State(GUARDCAPTAIN_STATE _eState);

	/* 파츠 관련 */
	CGameObject* Get_Part(GUARDCAPTAIN_PARTS _ePart) { return m_vecNpcPart[_ePart]; }
	void		 Set_WeaponSocket(const char* _strBoneName);

	virtual void	Set_Idle();

public:
	HRESULT Ready_Part();
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Ready_State();
	HRESULT Ready_Talk();

private:
	vector<class CGameObject*>		m_vecNpcPart;

	vector<CCollider*> m_pVecCollider;

	class CStateManager_GuardCaptain* m_pStateManager = nullptr;

public:
	static  CNPC_GuardCaptain* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;

};

END

