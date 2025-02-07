#pragma once

#include "Monster.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CFalmer_OneHand final : public CMonster
{
public:
	enum FALMEROH_PARTS { PART_WEAPON, PART_SHIELD, PART_END };

	enum FALMEROH_COLTYPE {
		FALMEROH_COL_AABB,
		FALMEROH_COL_DETECTION, FALMEROH_COL_MISSDETECTION,
		FALMEROH_COL_ATKROUND, FALMEROH_COL_END
	};

	enum FALMEROH_STATE {
		FALMEROH_IDLE, FALMEROH_EQUIP, FALMEROH_UNEQUIP,
		FALMEROH_WARNING, FALMEROH_DETECTION, FALMEROH_RETURN,
		FALMEROH_CHASE,
		FALMEROH_ATK, FALMEROH_ATK2, FALMEROH_PATK, FALMEROH_CHARGE,
		FALMEROH_STAGGERL, FALMEROH_STAGGERL2, FALMEROH_STAGGERH, FALMEROH_STAGGERH2,
		FALMEROH_DEAD,
		FALMEROH_END
	};

	typedef struct FalmerOHSpeedDesc
	{
		_float fChargeSpeed = 3.5f;
	}FALMEROH_SPEEDDESC;


private:
	CFalmer_OneHand(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CFalmer_OneHand(const CFalmer_OneHand& rhs);
	virtual ~CFalmer_OneHand() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_State(FALMEROH_STATE _eState);

	FALMEROH_SPEEDDESC   Get_FalmerOHSpeedDesc() { return m_tSpeedDesc; }

	/* 파츠 관련 */
	CGameObject* Get_Part(FALMEROH_PARTS _ePart) { return m_vecMonsterPart[_ePart]; }
	void		 Set_WeaponSocket(const char* _strBoneName);

public:
	HRESULT Ready_Part();
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Ready_State();

private:
	vector<class CGameObject*>		m_vecMonsterPart;

	vector<CCollider*> m_pVecCollider;

	class CStateManager_FalmerOH* m_pStateManager = nullptr;

	FALMEROH_SPEEDDESC				m_tSpeedDesc;

public:
	static  CFalmer_OneHand* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;
};

END

