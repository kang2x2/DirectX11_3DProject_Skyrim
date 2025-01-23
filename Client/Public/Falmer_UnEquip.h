#pragma once

#include "Monster.h"

BEGIN(Engine)

class CCollider;
class CTexture;

END

BEGIN(Client)

class CFalmer_UnEquip final : public CMonster
{
public:
	enum FALMERUE_PARTS { PART_WEAPON, PART_END };

	enum FALMERUE_COLTYPE	{ FALMERUE_COL_AABB, 
		FALMERUE_COL_DETECTION, FALMERUE_COL_MISSDETECTION,
		FALMERUE_COL_ATKROUND, FALMERUE_COL_END };
	
	enum FALMERUE_STATE { FALMERUE_SQUAT, FALMERUE_SQUAT_OUTRO, FALMERUE_SQUAT_INTRO,
		FALMERUE_WARNING, FALMERUE_DETECTION, FALMERUE_RETURN,
		FALMERUE_CHASE, 
		FALMERUE_ATK, FALMERUE_ATK2, FALMERUE_CHARGE,
		FALMERUE_STAGGERL, FALMERUE_STAGGERL2, FALMERUE_STAGGERH,
		FALMERUE_DEAD,
		FALMERUE_END };

	typedef struct FalmerUESpeedDesc
	{
		_float fChargeSpeed = 3.5f;
	}FALMERUE_SPEEDDESC;

private:
	CFalmer_UnEquip(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CFalmer_UnEquip(const CFalmer_UnEquip& rhs);
	virtual ~CFalmer_UnEquip() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_State(CFalmer_UnEquip::FALMERUE_STATE _eState);

	FALMERUE_SPEEDDESC   Get_FalmerUESpeedDesc() { return m_tSpeedDesc; }

	/* 파츠 관련 */
	CGameObject* Get_Part(FALMERUE_PARTS _ePart) { return m_vecMonsterPart[_ePart]; }


public:
	HRESULT Ready_Part();
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Ready_State();

private:
	vector<class CGameObject*>		m_vecMonsterPart;

	vector<CCollider*> m_pVecCollider;
	
	class CStateManager_FalmerUE* m_pStateManager = nullptr;

	FALMERUE_SPEEDDESC				m_tSpeedDesc;

public:
	static  CFalmer_UnEquip* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;

};

END

