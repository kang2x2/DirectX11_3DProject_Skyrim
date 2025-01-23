#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStatePlayerOH_Block final : public CState_Player
{
private:
	CStatePlayerOH_Block();
	virtual ~CStatePlayerOH_Block() = default;

public:
	virtual		 HRESULT Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation);
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);

	virtual void Key_Input(_float _fTimeDelta);

private:
	_float		m_fCounterTime = 0.f;

public:
	static CStatePlayerOH_Block* Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation);
	virtual void Free() override;
};

END
