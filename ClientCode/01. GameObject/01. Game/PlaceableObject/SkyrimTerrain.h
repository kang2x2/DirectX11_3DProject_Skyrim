#pragma once

#include "PlaceableObject.h"

BEGIN(Engine)
class CNavigation;
class CCollider;
END

BEGIN(Client)

class CSkyrimTerrain final : public CPlaceableObject
{
private:
	CSkyrimTerrain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimTerrain(const CSkyrimTerrain& rhs);
	virtual ~CSkyrimTerrain() = default;

public:
	virtual HRESULT Initialize_ProtoType() override; // 원본
	virtual HRESULT Initialize_Clone(void* pArg) override; // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg) override; // 사본
	virtual void	PriorityTick(_float _fTimeDelta) override;
	virtual void	Tick(_float _fTimeDelta) override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CNavigation*	m_pNavigationCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;

private:
	_matrix* pMatPivot;

private:
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Ready_Cell();

public:
	static CSkyrimTerrain* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual CGameObject* Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg) override;
	virtual void Free() override;
};

END

