#include "Object_Manager.h"

#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Reserve_Manager(_uint _iLevelCount)
{
	if (m_mapLayer != nullptr)
		return E_FAIL;

	m_mapLayer = new map<const wstring, class CLayer*>[_iLevelCount];

	m_iLevelNum = _iLevelCount;

	return S_OK;
}

HRESULT CObject_Manager::Add_ProtoObject(const wstring& _strProtoTypeTag, CGameObject* _pGameObject)
{
	// 새로 추가하는 것이기 때문에 이미 존재하여 반환값이 nullptr이 아니라면 실패.
	if (Find_ProtoObject(_strProtoTypeTag) != nullptr)
	{
		MSG_BOX("이미 추가된 오브젝트.");
		return E_FAIL;
	}

	m_mapProtoTypeObj.emplace(_strProtoTypeTag, _pGameObject);

	return S_OK;
}

HRESULT CObject_Manager::Add_CloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strProtoTypeTag, void* pArg)
{
	// 복제하려는 원본을 찾는다.
	CGameObject* pProtoTypeObject = Find_ProtoObject(_strProtoTypeTag);
	if (pProtoTypeObject == nullptr) // 복제하려는 원본이 없으면 실패.
	{
		MSG_BOX("복제하려는 원본이 존재하지 않음.");
		return E_FAIL;
	}

	// 찾은 원본을 복제하여 사본 생성.
	CGameObject* pCloneObject = pProtoTypeObject->Clone(pArg);
	if (pCloneObject == nullptr) // 복사에 실패 했을 시
	{
		MSG_BOX("오브젝트 복사 실패.");
		return E_FAIL;
	}

	CLayer* pLayer = Find_Layer(_iLevelIndex, _strLayerTag);

	// 사본을 추가하려는 레이어가 존재하지 않음.(새로 추가)
	if (pLayer == nullptr)
	{
		// 레이어 새로 생성
		pLayer = CLayer::Create();

		// 레이어에 오브젝트 추가
		pCloneObject->Set_ObjFileDesc(_strLayerTag, _strProtoTypeTag);
		pLayer->Add_CloneObject(pCloneObject);

		// 레이어를 새로운 레이어로 해당 레벨의 map에 추가.
		m_mapLayer[_iLevelIndex].emplace(_strLayerTag, pLayer);
	}
	else
	{
		pCloneObject->Set_ObjFileDesc(_strLayerTag, _strProtoTypeTag);
		pLayer->Add_CloneObject(pCloneObject);
	}

	return S_OK;
}

HRESULT CObject_Manager::Add_CloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strProtoTypeTag, const wstring& _strModelComTag, void* pArg)
{
	// 복제하려는 원본을 찾는다.
	CGameObject* pProtoTypeObject = Find_ProtoObject(_strProtoTypeTag);
	if (pProtoTypeObject == nullptr) // 복제하려는 원본이 없으면 실패.
	{
		MSG_BOX("복제하려는 원본이 존재하지 않음.");
		return E_FAIL;
	}

	// 찾은 원본을 복제하여 사본 생성.
	CGameObject* pCloneObject = pProtoTypeObject->Clone(_iLevelIndex, _strModelComTag, pArg);
	if (pCloneObject == nullptr) // 복사에 실패 했을 시
	{
		MSG_BOX("오브젝트 복사 실패.");
		return E_FAIL;
	}

	CLayer* pLayer = Find_Layer(_iLevelIndex, _strLayerTag);

	// 사본을 추가하려는 레이어가 존재하지 않음.(새로 추가)
	if (pLayer == nullptr)
	{
		// 레이어 새로 생성
		pLayer = CLayer::Create();

		// 레이어에 오브젝트 추가
		pCloneObject->Set_ObjFileDesc(_strLayerTag, _strProtoTypeTag, _strModelComTag);
		pLayer->Add_CloneObject(pCloneObject);

		// 레이어를 새로운 레이어로 해당 레벨의 map에 추가.
		m_mapLayer[_iLevelIndex].emplace(_strLayerTag, pLayer);
	}
	else
	{
		pCloneObject->Set_ObjFileDesc(_strLayerTag, _strProtoTypeTag, _strModelComTag);
		pLayer->Add_CloneObject(pCloneObject);
	}

	return S_OK;

}

CGameObject* CObject_Manager::Add_InstanceCloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strPrototypeTag, void* _pArg)
{
	CGameObject* pProtoTypeObject = Find_ProtoObject(_strPrototypeTag);
	if (nullptr == pProtoTypeObject)
	{
		MSG_BOX("원본 오브젝트가 존재하지 않음.(함수3)");
		return nullptr;
	}

	CGameObject* pCloneObject = pProtoTypeObject->Clone(_pArg);
	if (nullptr == pCloneObject)
	{
		MSG_BOX("오브젝트 복사 실패.(함수3)");
		return nullptr;
	}

	return pCloneObject;
}

CGameObject* CObject_Manager::Add_ClonePartObject(const wstring& _strPrototypeTag, void* _pArg)
{
	CGameObject* pProtoTypeObject = Find_ProtoObject(_strPrototypeTag);
	if (nullptr == pProtoTypeObject)
	{
		MSG_BOX("원본 파츠가 존재하지 않음.");
		return nullptr;
	}

	CGameObject* pCloneObject = pProtoTypeObject->Clone(_pArg);
	if (nullptr == pCloneObject)
	{
		MSG_BOX("파츠 복사 실패.");
		return nullptr;
	}

	return pCloneObject;
}

HRESULT CObject_Manager::Delete_CloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strName)
{
	auto objLayer = m_mapLayer[_iLevelIndex].find(_strLayerTag);

	if (objLayer == m_mapLayer[_iLevelIndex].end())
		return E_FAIL;

	objLayer->second->Delete_CloneObject(_strName);

	return S_OK;
}

void CObject_Manager::PriorityTick(_float _fTimeDelta)
{
	for (size_t i = 0; i < m_iLevelNum; ++i)
	{
		for (auto& iter : m_mapLayer[i])
			iter.second->PriorityTick(_fTimeDelta);
	}
}

void CObject_Manager::Tick(_float _fTimeDelta)
{
	for (size_t i = 0; i < m_iLevelNum; ++i)
	{
		for (auto& iter : m_mapLayer[i])
			iter.second->Tick(_fTimeDelta);
	}
}

void CObject_Manager::LateTick(_float _fTimeDelta)
{
	for (size_t i = 0; i < m_iLevelNum; ++i)
	{
		for (auto& iter : m_mapLayer[i])
		{
			iter.second->LateTick(_fTimeDelta);
		}
	}
}

void CObject_Manager::ClearTick()
{
	for (size_t i = 0; i < m_iLevelNum; ++i)
	{
		for (auto& iter : m_mapLayer[i])
		{
			iter.second->ClearTick();
		}
	}
}

void CObject_Manager::Clear(_uint _iLevelIndex)
{
	for (auto& iter : m_mapLayer[_iLevelIndex])
	{
		Safe_Release(iter.second);
	}
	m_mapLayer[_iLevelIndex].clear();
}

HRESULT CObject_Manager::Add_LightObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strProtoTypeTag, const wstring& _strModelComTag, void* _pArg, LIGHT_DESC* _pLightDesc)
{
	// 복제하려는 원본을 찾는다.
	CGameObject* pProtoTypeObject = Find_ProtoObject(_strProtoTypeTag);
	if (pProtoTypeObject == nullptr) // 복제하려는 원본이 없으면 실패.
	{
		MSG_BOX("복제하려는 원본이 존재하지 않음.");
		return E_FAIL;
	}

	// 찾은 원본을 복제하여 사본 생성.
	CGameObject* pCloneObject = pProtoTypeObject->LightClone(_iLevelIndex, _strModelComTag, _pArg, _pLightDesc);
	if (pCloneObject == nullptr) // 복사에 실패 했을 시
	{
		MSG_BOX("오브젝트 복사 실패.");
		return E_FAIL;
	}

	CLayer* pLayer = Find_Layer(_iLevelIndex, _strLayerTag);

	// 사본을 추가하려는 레이어가 존재하지 않음.(새로 추가)
	if (pLayer == nullptr)
	{
		// 레이어 새로 생성
		pLayer = CLayer::Create();

		// 레이어에 오브젝트 추가
		pCloneObject->Set_LightFileDesc(*(LIGHT_DESC*)_pLightDesc);
		pCloneObject->Set_ObjFileDesc(_strLayerTag, _strProtoTypeTag, _strModelComTag);
		pLayer->Add_CloneObject(pCloneObject);

		// 레이어를 새로운 레이어로 해당 레벨의 map에 추가.
		m_mapLayer[_iLevelIndex].emplace(_strLayerTag, pLayer);
	}
	else
	{
		pCloneObject->Set_LightFileDesc(*(LIGHT_DESC*)_pLightDesc);
		pCloneObject->Set_ObjFileDesc(_strLayerTag, _strProtoTypeTag, _strModelComTag);
		pLayer->Add_CloneObject(pCloneObject);
	}

	return S_OK;
}

CGameObject* CObject_Manager::Find_CloneObject(_uint _iLevelIndex, const wstring& _strLayerTag, const wstring& _strName)
{
	// 매개로 받은 레벨의 레이어만 검사
	for (auto& iter : m_mapLayer[_iLevelIndex])
	{
		if (iter.first == _strLayerTag)
		{
			// 매개로 받은 레이어의 이름을 찾았다면
			// 매개로 받은 오브젝트의 이름을 레이어의 Find_Object로 넘긴다.
 			return iter.second->Find_CloneObject(_strName);
		}
	}

	return nullptr;
}

CGameObject* CObject_Manager::Find_ProtoObject(const wstring& _strPrototypeTag)
{
	auto iter = m_mapProtoTypeObj.find(_strPrototypeTag);

	if (iter == m_mapProtoTypeObj.end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint _iLevelIndex, const wstring& _strLayerTag)
{
	if (_iLevelIndex >= m_iLevelNum)
		return nullptr;

	auto iter = m_mapLayer[_iLevelIndex].find(_strLayerTag);

	if (iter == m_mapLayer[_iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	__super::Free();

	// 레이어 맵 비우기 (사본 정리)
	for (size_t i = 0; i < m_iLevelNum; ++i)
	{
		for (auto& iter : m_mapLayer[i])
		{
			Safe_Release(iter.second);
		}
		m_mapLayer[i].clear();
	}
	Safe_Delete_Array(m_mapLayer);

	// 원본 객체 맵 비우기 (원본 정리)
	for (auto& iter : m_mapProtoTypeObj)
	{
		Safe_Release(iter.second);
	}
	m_mapProtoTypeObj.clear();

}
