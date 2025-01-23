#pragma once

#include "SkyrimArmor.h"

BEGIN(Engine)

class CBone;
class CModel;
class CRenderer;
class CShader;
class CTransform;

END

BEGIN(Client)

class CArmor_Glass final : public CSkyrimArmor
{
private:
	CArmor_Glass(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CArmor_Glass(const CArmor_Glass& rhs);
	virtual ~CArmor_Glass() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	CModel* m_pModelCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	CBone* m_pSocketBone = nullptr;
	_float4x4	m_matSocketPivot;

public:
	static  CArmor_Glass* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

