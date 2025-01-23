#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRenderer;
class CShader;
class CTransform;
class CCollider;
class CTexture;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CSceneChangeCol final : public CGameObject
{
public:
	typedef struct tagSceneChangeColDesc
	{
		STAGEID eDestStage = STAGE_END;
		_vector	vPos = {};
		wstring strDestText = TEXT("");
	}SCENECHANGECOL_DESC;

private:
	CSceneChangeCol(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSceneChangeCol(const CSceneChangeCol& rhs);
	virtual ~CSceneChangeCol() = default;

public:
	virtual HRESULT Initialize_ProtoType(); 
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;

	CShader*		m_pShaderE = nullptr;
	CTransform*		m_pTransformE = nullptr;
	CTexture*		m_pTextureE = nullptr;
	CVIBuffer_Rect* m_pVIBufferE = nullptr;

	STAGEID		m_eDestStage = STAGE_END;
	wstring		m_strDestText = TEXT("");

	_bool		m_bIsReadyChange = false; // Render 타는거 방지

public:
	static CSceneChangeCol* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

