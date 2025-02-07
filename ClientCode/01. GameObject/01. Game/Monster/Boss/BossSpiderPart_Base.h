#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CBone;
class CModel;
class CRenderer;
class CShader;
class CTransform;
class CTexture;

END

BEGIN(Client)

class CBossSpiderPart_Base abstract : public CGameObject
{
public:
	typedef struct tagPartDesc
	{
		CGameObject* pParent;
		CTransform*  pParentTransform;
	}PART_DESC;

protected:
	CBossSpiderPart_Base(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CBossSpiderPart_Base(const CBossSpiderPart_Base& rhs);
	virtual ~CBossSpiderPart_Base() = default;

public:
	CBone* Get_SocketBonePtr(const char* _pBoneName);
	_float4x4 Get_SocketPivotMatrix();

	CTransform* Get_ParentTransform() { return m_pParentTransform; }

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* _pArg); // 사본
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT Compute_RenderMatrix(_fmatrix ChildMatrix);

protected:
	CModel* m_pModelCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

	CGameObject* m_pParent = nullptr;
	CTransform* m_pParentTransform = nullptr;
	_float4x4		m_matWorld;

public:
	virtual CGameObject* Clone(void* _pArg) = 0;
	virtual void Free() override;

};

END

