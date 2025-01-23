#pragma once

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

class CCarlotta_Skeleton final : public CCarlottaPart_Base
{
private:
	CCarlotta_Skeleton(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCarlotta_Skeleton(const CCarlotta_Skeleton& rhs);
	virtual ~CCarlotta_Skeleton() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_AnimationIndex(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex);
	_uint		Get_CurFrameIndex();
	_bool		Get_CurAnimationIsLoop();
	_bool		Get_IsAnimationFin();
	string		Get_CurAnimationName();


private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

private:
	CCollider* m_pColliderCom = nullptr;

public:
	static CCarlotta_Skeleton* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

