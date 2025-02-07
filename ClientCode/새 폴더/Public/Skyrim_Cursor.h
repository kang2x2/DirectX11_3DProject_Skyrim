#pragma once

#include "SkyrimUI.h"

BEGIN(Engine)

class CTexture;
class CRenderer;
class CShader;
class CVIBuffer_Rect;
class CTransform;

END

class CSkyrim_Cursor final : public CSkyrimUI
{
private:
	CSkyrim_Cursor(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrim_Cursor(const CSkyrim_Cursor& rhs);
	virtual ~CSkyrim_Cursor() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

public:
	static CSkyrim_Cursor* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CSkyrimUI* Clone(void* pArg) override;
	virtual void Free() override;
};

