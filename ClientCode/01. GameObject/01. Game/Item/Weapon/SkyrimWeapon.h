#pragma once

#include "SkyrimItem.h"

BEGIN(Engine)
class CBone;
END

BEGIN(Client)

class CSkyrimWeapon abstract : public CSkyrimItem
{
public:
	enum WEAPON_VIEWTYPE { VIEW_3ST, VIEW_1ST, VIEW_END };

protected:
	CSkyrimWeapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimWeapon(const CSkyrimWeapon& rhs);
	virtual ~CSkyrimWeapon() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* _pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Set_ViewType(WEAPON_VIEWTYPE _eType) { m_eViewType = _eType; }

protected:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

protected:
	CModel* m_pModelComAry[VIEW_END] = { nullptr, nullptr };
	CRenderer* m_pRendererCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	CBone* m_pSocketBone = nullptr;
	_float4x4	m_matSocketPivot;

	WEAPON_VIEWTYPE m_eViewType = VIEW_3ST;
public:
	virtual CGameObject* Clone(void* _pArg) = 0;
	virtual void Free() override;
};

END

