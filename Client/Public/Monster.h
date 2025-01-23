#pragma once

#include "Client_Defines.h"
#include "CreatureObject.h"

BEGIN(Engine)

class CModel;
class CTransform;
class CShader;
class CRenderer;
class CTexture;
class CNavigation;

END

BEGIN(Client)

class CMonster abstract : public CCreatureObject
{
protected:
	CMonster(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

public:
	void			Play_Animation(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex = 0, _bool _bIsReset = false, _bool _bIsQuickChange = false);

	_bool			Get_IsAnimationFin();
	string			Get_CurAnimationName();
	_uint			Get_CurFrameIndex();

	_vector			Get_OriginPos() { return m_vOriginPos; }

	_bool			Get_IsDissloving() { return m_bIsDissloving; }
	void			Set_IsDissloving(_bool _isDissloving) { m_bIsDissloving = _isDissloving; }
	void			Set_AnimationStop(_bool _bIsStop);

protected:
	_matrix*		pMatPivot;

	CModel*      m_pModelCom = nullptr;
	CShader*     m_pShaderCom = nullptr;
	CRenderer*   m_pRendererCom = nullptr;
	CTransform*  m_pTransformCom = nullptr;
	CTexture*	 m_pTextureCom = nullptr;
	CNavigation* m_pNavigationCom = nullptr;

	_vector			m_vOriginPos = {};

	class CPlayer* m_pPlayer = nullptr;
	class CSkyrimUI_MonsterHpBar* m_pHpBar = nullptr;

	_bool		m_bIsDissloving = false;
	_float		m_fCurDissloveTime = 0.f;
	_float		m_fDissloveTime = 0.f;

public:
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

