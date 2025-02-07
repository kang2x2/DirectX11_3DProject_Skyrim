#include "Light.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& _LightDesc)
{
	memmove(&m_LightDesc, &_LightDesc, sizeof(_LightDesc));

	return S_OK;
}

HRESULT CLight::Render(CShader* _pShader, CVIBuffer_Rect* _pBuffer)
{
	_uint iPassIndex = 0;

	if (LIGHT_DESC::LIGHT_DIRECTIONAL == m_LightDesc.eLightType)
	{
		if (FAILED(_pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vLightDir, sizeof(_float4))))
			return E_FAIL;

		iPassIndex = 1;
	}
	else if (LIGHT_DESC::LIGHT_POINT == m_LightDesc.eLightType)
	{
		if (FAILED(_pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vLightPos, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(_pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fLightRange, sizeof(_float))))
			return E_FAIL;

			iPassIndex = 2;
	}

	if (FAILED(_pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(_pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(_pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(_pShader->Begin(iPassIndex)))
		return E_FAIL;

	if (FAILED(_pBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

void CLight::Set_Diffuse(_float4 _vDiffuse)
{
	m_LightDesc.vDiffuse = _vDiffuse;
}

void CLight::Set_Ambient(_float4 _vAmbient)
{
	m_LightDesc.vAmbient = _vAmbient;
}

void CLight::Set_Specular(_float4 _vSpecular)
{
	m_LightDesc.vSpecular = _vSpecular;
}

void CLight::Set_Direction(_float4 _vDir)
{
	m_LightDesc.vLightDir = _vDir;
}

void CLight::Set_Range(_float _fRange)
{
	m_LightDesc.fLightRange = _fRange;
}

CLight* CLight::Create(const LIGHT_DESC& _LightDesc)
{
	CLight* pInstance = new CLight();

	if (FAILED(pInstance->Initialize(_LightDesc)))
	{
		MSG_BOX("Fail Create : CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	__super::Free();
}
