#pragma once

#include "Skyrim_NPC.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CNPC_Carlotta final : public CSkyrim_NPC
{
public:
	enum CARLOTTA_PARTS { PART_WEAPON, PART_END };

	enum CARLOTTA_COLTYPE {
		CARLOTTA_COL_AABB,
		CARLOTTA_COL_DIALOG, CARLOTTA_COL_ACTIVE, CARLOTTA_COL_END
	};

	enum CARLOTTA_STATE {
		CARLOTTA_IDLE, CARLOTTA_WALK, CARLOTTA_RUN, CARLOTTA_DIALOG,
		CARLOTTA_COUNTER_ENTER, CARLOTTA_COUNTER_IDLE, CARLOTTA_COUNTER_DIALOG, CARLOTTA_COUNTER_EXIT,
		CARLOTTA_SWEEPING_ENTER, CARLOTTA_SWEEPING_IDLE, CARLOTTA_SWEEPING_DIALOG, CARLOTTA_SWEEPING_EXIT,
		CARLOTTA_END
	};

private:
	CNPC_Carlotta(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNPC_Carlotta(const CNPC_Carlotta& rhs);
	virtual ~CNPC_Carlotta() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring & _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_State(CARLOTTA_STATE _eState);

	/* 파츠 관련 */
	CGameObject* Get_Part(CARLOTTA_PARTS _ePart) { return m_vecNpcPart[_ePart]; }
	void		 Set_WeaponSocket(const char* _strBoneName);


public:
	HRESULT Ready_Part();
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Ready_State();
	HRESULT Ready_Talk();

private:
	vector<class CGameObject*>		m_vecNpcPart;

	vector<CCollider*> m_pVecCollider;

	// class CStateManager_FalmerOH* m_pStateManager = nullptr;

public:
	static  CNPC_Carlotta* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring & _strModelComTag, void* _pArg) override;
	virtual void Free() override;

};

END

