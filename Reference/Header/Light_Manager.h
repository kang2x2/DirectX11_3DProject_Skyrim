#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	LIGHT_DESC* Get_LightDesc(_uint _iLightIndex);
	_uint		Get_CurLightIndex() { return m_ltLight.size(); }

	HRESULT Add_Light(const LIGHT_DESC& _LightDesc);
	void    Delete_Light(_uint _iLightIndex);
	HRESULT Render(class CShader* _pShader, class CVIBuffer_Rect* _pBuffer);

	void Set_Diffuse(_float4 _vDiffuse, _uint _iLightIndex);
	void Set_Ambient(_float4 _vAmbient, _uint _iLightIndex);
	void Set_Specular(_float4 _vSpecular, _uint _iLightIndex);
	void Set_Direction(_float4 _vDir, _uint _iLightIndex);
	void Set_Range(_float _fRange, _uint _iLightIndex);

	void Light_Clear();

private:
	list<class CLight*> m_ltLight;

public:
	virtual void Free() override;
};

END

