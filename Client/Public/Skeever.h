#pragma once

#include "Monster.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CSkeever final : public CMonster
{
public:
	enum SKEEVER_PARTS { PART_WEAPON, PART_END };

	enum SKEEVER_COLTYPE {
		 SKEEVER_COL_AABB,
		 SKEEVER_COL_DETECTION, SKEEVER_COL_MISSDETECTION,
		 SKEEVER_COL_ATKROUND, SKEEVER_COL_END
	};

	enum SKEEVER_STATE { SKEEVER_IDLE, 
		SKEEVER_WARNING, SKEEVER_DETECTION, SKEEVER_RETURN,
		SKEEVER_CHASE,
		SKEEVER_ATK, SKEEVER_CHARGE,
		SKEEVER_STAGGER,
		SKEEVER_DEAD,
		SKEEVER_END };

	typedef struct SkeeverSpeedDesc
	{
		_float fChargeSpeed = 3.5f;
	}SKEEVER_SPEEDDESC;

private:
	CSkeever(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkeever(const CSkeever& rhs);
	virtual ~CSkeever() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_State(SKEEVER_STATE _eState);
	SKEEVER_SPEEDDESC   Get_FalmerUESpeedDesc() { return m_tSpeedDesc; }
	/* 파츠 관련 */
	CGameObject* Get_Part(SKEEVER_PARTS _ePart) { return m_vecMonsterPart[_ePart]; }

public:
	HRESULT Ready_Part();
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Ready_State();


private:
	vector<class CGameObject*>		m_vecMonsterPart;
	vector<CCollider*>				m_pVecCollider;

	class CStateManager_Skeever*	m_pStateManager = nullptr;
	SKEEVER_SPEEDDESC				m_tSpeedDesc;


public:
	static  CSkeever* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;

};

END

