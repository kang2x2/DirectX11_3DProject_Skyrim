#pragma once

#include "State_Npc.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;
class CCollider;

END

BEGIN(Client)

class CState_GuardSupply abstract : public CState_Npc
{
protected:
	CState_GuardSupply();
	virtual ~CState_GuardSupply() = default;

public:
	virtual HRESULT Initialize(CGameObject* _pNpc, CGameObject* _pPlayer, CTransform* _pNpcTransform, CNavigation* _pNpcNavigation, vector<CCollider*> _pVecColCom);

public:
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);

protected:
	class CNPC_GuardSupply* m_pNpc = nullptr;

public:
	virtual void Free() override;

};

END

