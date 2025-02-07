#pragma once

#include "Client_Defines.h"
#include "CarlottaPart_Base.h"

BEGIN(Engine)

class CBone;
class CModel;
class CRenderer;
class CShader;
class CCollider;
class CTransform;

END

BEGIN(Client)

class CCarlotta_Body final : public CCarlottaPart_Base
{
private:
	CCarlotta_Body(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCarlotta_Body(const CCarlotta_Body& rhs);
	virtual ~CCarlotta_Body() = default;

public:
	_bool Get_IsAnimationFin();
	string Get_CurAnimationName();

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_AnimationIndex(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange);
	_uint		Get_CurFrameIndex();
	_bool		Get_CurAnimationIsLoop();
private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

private:
	CCollider* m_pColliderCom = nullptr;

public:
	static CCarlotta_Body* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

