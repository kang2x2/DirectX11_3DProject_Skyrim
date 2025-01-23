#pragma once

#include "Monster.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CSpider final : public CMonster
{
public:
	enum SPIDER_PARTS { PART_MOUTH, PART_END };

	enum SPIDER_COLTYPE {
		SPIDER_COL_AABB,
		SPIDER_COL_DETECTION, SPIDER_COL_MISSDETECTION,
		SPIDER_COL_ATKROUND, SPIDER_COL_END
	};

	enum SPIDER_STATE {
		SPIDER_IDLE, SPIDER_GETUP,
		SPIDER_CHASE, SPIDER_BACKWARD,
		SPIDER_BITE, SPIDER_CHARGE, SPIDER_SPIT,
		SPIDER_STAGGER_B, SPIDER_STAGGER_C,
		SPIDER_DEAD,
		SPIDER_END
	};

	typedef struct SpiderSpeedDesc
	{
		_float fChargeSpeed = 2.5f;
	}SPIDER_SPEEDDESC;

private:
	CSpider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSpider(const CSpider& rhs);
	virtual ~CSpider() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_State(SPIDER_STATE _eState);

	SPIDER_SPEEDDESC   Get_SpiderSpeedDesc() { return m_tSpeedDesc; }

	/* 파츠 관련 */
	CGameObject* Get_Part(SPIDER_PARTS _ePart) { return m_vecMonsterPart[_ePart]; }


public:
	HRESULT Ready_Part();
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Ready_State();

private:
	vector<class CGameObject*>		m_vecMonsterPart;

	vector<CCollider*> m_pVecCollider;

	class CStateManager_Spider* m_pStateManager = nullptr;

	SPIDER_SPEEDDESC				m_tSpeedDesc;

public:
	static  CSpider* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;
};

END

