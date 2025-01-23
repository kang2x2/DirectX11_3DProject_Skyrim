#pragma once

#include "SkyrimArmor.h"

BEGIN(Client)

class CTorsoM_Blades final : public CSkyrimArmor
{
private:
	CTorsoM_Blades(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTorsoM_Blades(const CTorsoM_Blades& rhs);
	virtual ~CTorsoM_Blades() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Component();

public:
	static  CTorsoM_Blades* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;

};

END

