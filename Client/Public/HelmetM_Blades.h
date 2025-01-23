#pragma once

#include "SkyrimArmor.h"

BEGIN(Client)

class CHelmetM_Blades final : public CSkyrimArmor
{
private:
	CHelmetM_Blades(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CHelmetM_Blades(const CHelmetM_Blades& rhs);
	virtual ~CHelmetM_Blades() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Component();

public:
	static  CHelmetM_Blades* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;

};

END

