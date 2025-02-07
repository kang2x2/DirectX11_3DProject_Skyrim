#include "Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Manager(_uint _iLevelIndex)
{
	if (m_mapProtoTypeObj != nullptr)
		return E_FAIL;

	m_iLevelIndex = _iLevelIndex;

	m_mapProtoTypeObj = new map<const wstring, class CComponent*>[_iLevelIndex];


	return S_OK;
}

HRESULT CComponent_Manager::Add_ProtoType_Component(_uint _iLevelIndex, const wstring& _strProtoTypeTag, CComponent* _pProtoTypeComponent)
{
	if (m_mapProtoTypeObj == nullptr ||
		Find_ProtoType(_iLevelIndex, _strProtoTypeTag) != nullptr)
	{
		MSG_BOX("이미 추가된 컴포넌트.");
		return E_FAIL;
	}

	m_mapProtoTypeObj[_iLevelIndex].emplace(_strProtoTypeTag, _pProtoTypeComponent);

	return S_OK;
}

CComponent* CComponent_Manager::Clone_Component(_uint _iLevelIndex, const wstring& _strProtoTypeTag, void* _pArg)
{
	// 컴포넌트 원본 탐색
	CComponent* pProtoTypeComponent = Find_ProtoType(_iLevelIndex, _strProtoTypeTag);
	if (pProtoTypeComponent == nullptr)
	{
		MSG_BOX("원본 컴포넌트가 존재하지 않음.");
		return nullptr;
	}
	// 컴포넌트 복사
	CComponent* pCloneComponent = pProtoTypeComponent->Clone(_pArg);
	if (pCloneComponent == nullptr)
	{
		MSG_BOX("컴포넌트 복사 실패.");
		return nullptr;
	}

	return pCloneComponent;
}

CComponent* CComponent_Manager::Find_ProtoType(_uint _iLevelIndex, const wstring& _strProtoTypeTag)
{
	if (_iLevelIndex >= m_iLevelIndex)
		return nullptr;

	auto iter = m_mapProtoTypeObj[_iLevelIndex].find(_strProtoTypeTag);

	if (iter == m_mapProtoTypeObj[_iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iLevelIndex; ++i)
	{
		for (auto& iter : m_mapProtoTypeObj[i])
		{
			Safe_Release(iter.second);
		}
		m_mapProtoTypeObj[i].clear();
	}

	Safe_Delete_Array(m_mapProtoTypeObj);
}
