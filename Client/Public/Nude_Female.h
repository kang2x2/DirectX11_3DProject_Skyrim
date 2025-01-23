#pragma once

#include "SkyrimArmor.h"

BEGIN(Engine)

class CModel;
class CRenderer;
class CShader;
class CTransform;
class CBone;

END


BEGIN(Client)

class CNude_Female final : public CSkyrimArmor
{
private:
	CNude_Female(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNude_Female(const CNude_Female& rhs);
	virtual ~CNude_Female() = default;

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
	static  CNude_Female* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

