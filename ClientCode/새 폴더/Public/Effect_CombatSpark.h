#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform; 
class CCollider;
class CVIBuffer_Rect_Instance;

END

BEGIN(Client)

class CEffect_CombatSpark final : public CGameObject
{
private:
	CEffect_CombatSpark(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_CombatSpark(const CEffect_CombatSpark& rhs);
	virtual ~CEffect_CombatSpark() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_vector _vWeaponPos, _float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void	Set_Spark()
	{
		m_bIsSpark = true;
		m_fSparkTime = 0.f;
	}

private:
	CRenderer* m_pRendererCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CVIBuffer_Rect_Instance* m_pVIBufferCom = nullptr;

	_float m_fRange = 0.f;
	_uint  m_iSelfIndex = 0;
	_bool  m_bIsSpark = false;
	_float m_fSparkTime = 0.f;

	CTransform* m_pOwnerTransform = nullptr;

private:
	HRESULT Ready_Component();
	HRESULT Ready_Light();
	HRESULT Bind_ShaderResource();

public:
	LIGHT_DESC* Get_LightDesc();
	void Set_LightDesc(LIGHT_DESC _lightDesc);

public:
	static CEffect_CombatSpark* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

