#pragma once

#include "Skyrim_NPC.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CNPC_Guard final : public CSkyrim_NPC
{
public:
	enum GUARD_PARTS { PART_WEAPON, PART_END };

	enum GUARD_COLTYPE {
		GUARDCAPTAIN_COL_AABB,
		GUARDCAPTAIN_COL_DETECTION,
		GUARDCAPTAIN_COL_DIALOG,
		GUARDCAPTAIN_COL_END
	};

	enum GUARD_STATE {
		GUARDCAPTAIN_IDLE,
		GUARDCAPTAIN_END
	};

private:
	CNPC_Guard(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNPC_Guard(const CNPC_Guard& rhs);
	virtual ~CNPC_Guard() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_State(GUARD_STATE _eState);

	/* 파츠 관련 */
	CGameObject* Get_Part(GUARD_PARTS _ePart) { return m_vecNpcPart[_ePart]; }
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

public:
	static  CNPC_Guard* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;

};

END

