#pragma once

#include "SkyrimArmor.h"

BEGIN(Client)

class CBootsF_Merchant01 final : public CSkyrimArmor
{
private:
	CBootsF_Merchant01(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CBootsF_Merchant01(const CBootsF_Merchant01& rhs);
	virtual ~CBootsF_Merchant01() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Component();

public:
	static  CBootsF_Merchant01* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;

};

END

