#pragma once

#include "SkyrimArmor.h"

BEGIN(Client)

class CGlovesM_Blades final : public CSkyrimArmor
{
private:
	CGlovesM_Blades(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CGlovesM_Blades(const CGlovesM_Blades& rhs);
	virtual ~CGlovesM_Blades() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Component();

public:
	static  CGlovesM_Blades* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;

};

END

