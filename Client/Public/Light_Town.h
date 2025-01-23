#pragma once

#include "Skyrim_Light.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

/* hpp에 인덱스 추가해야 함. */
class CLight_Town final : public CSkyrim_Light
{
private:
	CLight_Town(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLight_Town(const CLight_Town& rhs);
	virtual ~CLight_Town() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Light();

public:
	static CLight_Town* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

