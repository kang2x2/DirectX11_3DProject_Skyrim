#pragma once

#include "Skyrim_NPC.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CNPC_GuardSupply final : public CSkyrim_NPC
{
public:
	enum GUARDSUPPLY_COLTYPE {
		GUARDSUPPLY_COL_AABB,
		GUARDSUPPLY_COL_DIALOG,
		GUARDSUPPLY_COL_END
	};

	enum GUARDSUPPLY_STATE {
		GUARDSUPPLY_IDLE,
		GUARDSUPPLY_TALK,
		GUARDSUPPLY_END
	};

private:
	CNPC_GuardSupply(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNPC_GuardSupply(const CNPC_GuardSupply& rhs);
	virtual ~CNPC_GuardSupply() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_State(GUARDSUPPLY_STATE _eState);
	virtual void	Set_Idle();

public:
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Ready_State();
	HRESULT Ready_Talk();

private:
	vector<CCollider*> m_pVecCollider;

	class CStateManager_GuardSupply* m_pStateManager = nullptr;

public:
	static  CNPC_GuardSupply* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;

};

END

