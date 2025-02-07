#include "Light_Manager.h"
#include "Light.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

LIGHT_DESC* CLight_Manager::Get_LightDesc(_uint _iLightIndex)
{
	// 예외처리
	if (_iLightIndex >= m_ltLight.size())
		return nullptr;

	// 순회(index까지)
	auto iter = m_ltLight.begin();

	for (size_t i = 0; i < _iLightIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& _LightDesc)
{
	CLight* pLight = CLight::Create(_LightDesc);

	if (pLight == nullptr)
		return E_FAIL;

	m_ltLight.push_back(pLight);

	return S_OK;
}

void CLight_Manager::Delete_Light(_uint _iLightIndex)
{
	if (_iLightIndex >= m_ltLight.size())
	{
		MSG_BOX("범위를 벗어나는 Light Index.");
		return;
	}

	auto iter = m_ltLight.begin();

	for (size_t i = 0; i < _iLightIndex; ++i)
		++iter;

	Safe_Release(*iter);
}

HRESULT CLight_Manager::Render(CShader* _pShader, CVIBuffer_Rect* _pBuffer)
{
	for (auto& iter : m_ltLight)
	{
		if (iter->Get_LightDesc()->eLightType == LIGHT_DESC::LIGHT_POINT)
		{
			if (CGameInstance::GetInstance()->IsIn_Frustum_World(XMLoadFloat4(&iter->Get_LightDesc()->vLightPos), iter->Get_LightDesc()->fLightRange))
			{
				iter->Render(_pShader, _pBuffer);
			}
		}
		else
		{
			iter->Render(_pShader, _pBuffer);
		}
	}

	return S_OK;
}

void CLight_Manager::Set_Diffuse(_float4 _vDiffuse, _uint _iLightIndex)
{
	if (_iLightIndex >= m_ltLight.size())
	{
		MSG_BOX("범위를 벗어나는 Light Index.");
		return;
	}

	auto iter = m_ltLight.begin();

	for (size_t i = 0; i < _iLightIndex; ++i)
		++iter;

	return (*iter)->Set_Diffuse(_vDiffuse);
}

void CLight_Manager::Set_Ambient(_float4 _vAmbient, _uint _iLightIndex)
{
	if (_iLightIndex >= m_ltLight.size())
	{
		MSG_BOX("범위를 벗어나는 Light Index.");
		return;
	}

	auto iter = m_ltLight.begin();

	for (size_t i = 0; i < _iLightIndex; ++i)
		++iter;

	return (*iter)->Set_Ambient(_vAmbient);

}

void CLight_Manager::Set_Specular(_float4 _vSpecular, _uint _iLightIndex)
{
	if (_iLightIndex >= m_ltLight.size())
	{
		MSG_BOX("범위를 벗어나는 Light Index.");
		return;
	}

	auto iter = m_ltLight.begin();

	for (size_t i = 0; i < _iLightIndex; ++i)
		++iter;

	return (*iter)->Set_Specular(_vSpecular);

}

void CLight_Manager::Set_Direction(_float4 _vDir, _uint _iLightIndex)
{
	if (_iLightIndex >= m_ltLight.size())
	{
		MSG_BOX("범위를 벗어나는 Light Index.");
		return;
	}

	auto iter = m_ltLight.begin();

	for (size_t i = 0; i < _iLightIndex; ++i)
		++iter;

	return (*iter)->Set_Direction(_vDir);

}

void CLight_Manager::Set_Range(_float _fRange, _uint _iLightIndex)
{
	if (_iLightIndex >= m_ltLight.size())
	{
		MSG_BOX("범위를 벗어나는 Light Index.");
		return;
	}

	auto iter = m_ltLight.begin();

	for (size_t i = 0; i < _iLightIndex; ++i)
		++iter;

	return (*iter)->Set_Range(_fRange);

}

void CLight_Manager::Light_Clear()
{
	for (auto& iter : m_ltLight)
		Safe_Release(iter);

	m_ltLight.clear();
}

void CLight_Manager::Free()
{
	__super::Free();

	for (auto& iter : m_ltLight)
		Safe_Release(iter);

	m_ltLight.clear();
}
