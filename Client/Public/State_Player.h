#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CTransform;
class CNavigation;

END

BEGIN(Client)

class CState_Player abstract : public CBase
{
protected:
	CState_Player();
	virtual ~CState_Player() = default;

public:
	virtual HRESULT Initialize(class CGameObject* _pPlayer, class CTransform* _pPlayerTransform, class CNavigation* _pPlayerNavigation);

public:
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);

	virtual void Key_Input(_float _fTimeDelta);

protected:
	virtual void Player_SetLook(_float _fXDegree, _float _fYDegree = 0.f);

protected:
	class CPlayer* m_pPlayer = nullptr;
	CTransform*  m_pPlayerTransform = nullptr;
	CNavigation* m_pPlayerNavigation = nullptr;

	_float	m_fSoundSpeed = 0.f;

public:
	virtual void Free() override;
};

END

