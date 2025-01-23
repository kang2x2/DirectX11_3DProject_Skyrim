#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	LIGHT_DESC* Get_LightDesc()
	{
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHT_DESC& _LightDesc);
	HRESULT	Render(class CShader* _pShader, class CVIBuffer_Rect* _pBuffer);

	void Set_Diffuse(_float4 _vDiffuse);
	void Set_Ambient(_float4 _vAmbient);
	void Set_Specular(_float4 _vSpecular);
	void Set_Direction(_float4 _vDir);
	void Set_Range(_float _fRange);
private:
	LIGHT_DESC		m_LightDesc;

public:
	static CLight* Create(const LIGHT_DESC& _LightDesc);
	virtual void Free() override;


};

END
