#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CSkyrimHelmet abstract : public CGameObject
{
protected:
	CSkyrimHelmet(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimHelmet(const CSkyrimHelmet& rhs);
	virtual ~CSkyrimHelmet() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* _pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	virtual CGameObject* Clone(void* _pArg) = 0;
	virtual void Free() override;
};

END

