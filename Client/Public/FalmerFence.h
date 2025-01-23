#pragma once

#include "PlaceableObject.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CFalmerFence final : public CPlaceableObject
{
private:
	CFalmerFence(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CFalmerFence(const CFalmerFence& rhs);
	virtual ~CFalmerFence() = default;

public:
	virtual HRESULT Initialize_ProtoType() override; // 원본
	virtual HRESULT Initialize_Clone(void* pArg) override; // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg) override; // 사본
	virtual void	Tick(_float _fTimeDelta) override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom = nullptr;

private:
	HRESULT Ready_Component(_uint _iLevel);

public:
	static CFalmerFence* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;
};

END
