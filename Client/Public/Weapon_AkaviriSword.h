#pragma once

#include "SkyrimWeapon.h"

BEGIN(Client)

class CWeapon_AkaviriSword final : public CSkyrimWeapon
{
private:
	CWeapon_AkaviriSword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CWeapon_AkaviriSword(const CWeapon_AkaviriSword& rhs);
	virtual ~CWeapon_AkaviriSword() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Component();

public:
	static  CWeapon_AkaviriSword* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

