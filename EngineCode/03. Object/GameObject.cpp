#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CGameObject::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CGameObject::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	return S_OK;
}

void CGameObject::PriorityTick(_float _fTimeDelta)
{
}

void CGameObject::Tick(_float _fTimeDelta)
{
}

void CGameObject::LateTick(_float _fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent* CGameObject::Get_Component(const wstring& _strComponentName)
{
	for (auto& iter : m_mapComponent)
	{
		if (iter.first == _strComponentName)
			return iter.second;
	}

	return nullptr;
}

void CGameObject::Set_ObjFileDesc(const wstring& _strLayerTag, const wstring& _strProtoObjTag, const wstring& _strProtoModelComTag)
{
	m_tObjFileDesc.m_strLayerTag = _strLayerTag;
	m_tObjFileDesc.m_strProtoObjTag = _strProtoObjTag;
	m_tObjFileDesc.m_strProtoModelComTag = m_strModelComTag;
}

HRESULT CGameObject::Add_CloneComponent(_uint _iLevelIndex, const wstring& _strProtoTypeTag, const wstring& _strComponentTag, CComponent** _ppOut, void* pArg)
{
	// 이미 등록 되어있는 컴포넌트 그룹이라면 실패. 
	if (Find_ProtoTypeComponent(_strComponentTag) != nullptr)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 사본 생성.
	CComponent* pComponent = pGameInstance->Clone_Component(_iLevelIndex, _strProtoTypeTag, pArg);
	if (pComponent == nullptr)
		return E_FAIL;

	// 검색이 가능하게 map에 삽입
	m_mapComponent.emplace(_strComponentTag, pComponent);

	// 자식의 멤버 변수에 저장(현재 컴포넌트들을 자식이 가지고 있게 관리하고 있다.)
	*_ppOut = pComponent;

	Safe_AddRef(pComponent);
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CGameObject::Add_CloneComponent(const wstring& _strComponentTag, CComponent* _pCopyTargetCom, CComponent** _ppOut)
{
	// 이미 등록 되어있는 컴포넌트 그룹이라면 실패. 
	if (Find_ProtoTypeComponent(_strComponentTag) != nullptr)
		return E_FAIL;

	// 검색이 가능하게 map에 삽입
	*_ppOut = _pCopyTargetCom;
	m_mapComponent.emplace(_strComponentTag, _pCopyTargetCom);
	Safe_AddRef(_pCopyTargetCom);

	return S_OK;
}

CComponent* CGameObject::Find_ProtoTypeComponent(const wstring& _strProtoTypeTag)
{
	auto iter = m_mapComponent.find(_strProtoTypeTag);

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

CGameObject* CGameObject::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	return nullptr;
}

CGameObject* CGameObject::LightClone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg, LIGHT_DESC* _LightDesc)
{
	return nullptr;
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& iter : m_mapComponent)
		Safe_Release(iter.second);

	m_mapComponent.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
